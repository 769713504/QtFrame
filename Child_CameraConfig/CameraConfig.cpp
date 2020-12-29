//
// Created by xin on 2020/12/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_CameraConfig.h" resolved

#include "CameraConfig.h"
#include "ui_CameraConfig.h"

// #include "Main/Main.h"

CameraConfig::CameraConfig(Main *parent) : QDialog(parent), ui(new Ui::CameraConfig) {
    ui->setupUi(this);
    main_window = parent;
    main_window->is_change_param = false;

    // 设置下拉表单
    setComboBox();
    // 获取当前相机参数
    getNowCamArgs();

    //点击事件
    connect(ui->checkBox_0, SIGNAL(clicked()), SLOT(disableComboBox()));            // 复选
    connect(ui->pushButton_1, SIGNAL(clicked()), SLOT(saveSet()));                  // 保存参数
    connect(ui->pushButton_2, SIGNAL(clicked()), SLOT(cancelSet()));                // 取消操作
}

CameraConfig::~CameraConfig() {
    delete ui;
}

// 复选
void CameraConfig::disableComboBox() {
    if (ui->checkBox_0->isChecked()) {
        ui->comboBox_0->setEnabled(false);
    } else {
        ui->comboBox_0->setEnabled(true);
    }
}

// 保存参数
void CameraConfig::saveSet() {
    main_window->is_change_param = true;
    setCamConfigArgs();
    close();

}

// 取消操作
void CameraConfig::cancelSet() {
    close();
}

// 设置下拉表单
void CameraConfig::setComboBox() {
    ui->comboBox_0->clear();
    for (int i = 0; i < main_window->cam_sum; i++) ui->comboBox_0->addItem(QString::number(i));
    ui->comboBox_0->setCurrentIndex(main_window->now_show_num);
}

// 获取当前相机参数
void CameraConfig::getNowCamArgs() {

    int exposure_time = main_window->camera_config_struct_vector.at(main_window->now_show_num).exposure_time;  // 曝光时间
    int frame_rate = main_window->camera_config_struct_vector.at(main_window->now_show_num).frame_rate;        // 帧率
    int image_gain = main_window->camera_config_struct_vector.at(main_window->now_show_num).image_gain;        // 图像增益
    // 写入spinBox
    ui->spinBox_1->setValue(exposure_time);
    ui->spinBox_2->setValue(frame_rate);
    ui->spinBox_3->setValue(image_gain);
    printf("获取相机参数成功~");
}

// 设置相机参数
void CameraConfig::setCamConfigArgs() {
    // 获取参数
    int exposuretime = ui->spinBox_1->value();          // 曝光时间
    int framerate = ui->spinBox_2->value();             // 帧率
    int gain = ui->spinBox_3->value();                  // 图像增益
    // 判断复选框状态
    if (ui->checkBox_0->isChecked()) {
        for (int i = 0; i < main_window->cam_sum; i++) {
            main_window->camera_config_struct_vector.at(i).exposure_time = exposuretime;
            main_window->camera_config_struct_vector.at(i).frame_rate = framerate;
            main_window->camera_config_struct_vector.at(i).image_gain = gain;
        }
        printf("所有相机参数已更新成功~");
    } else {
        main_window->camera_config_struct_vector.at(main_window->now_show_num).exposure_time = exposuretime;
        main_window->camera_config_struct_vector.at(main_window->now_show_num).frame_rate = framerate;
        main_window->camera_config_struct_vector.at(main_window->now_show_num).image_gain = gain;

        printf("相机%d参数已更新成功~", main_window->now_show_num);
    }
}

