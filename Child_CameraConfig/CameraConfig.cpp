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

}

// 保存参数
void CameraConfig::saveSet() {

}

// 取消操作
void CameraConfig::cancelSet() {

}

// 设置下拉表单
void CameraConfig::setComboBox() {
    ui->comboBox_0->clear();
    for (int i = 0; i < main_window->cam_sum; i++) ui->comboBox_0->addItem(QString::number(i));
    ui->comboBox_0->setCurrentIndex(main_window->now_show_num);
}

// 获取当前相机参数
void CameraConfig::getNowCamArgs() {

    int exposure_time = main_window->camera_config_vector.at(main_window->now_show_num).exposure_time;  // 曝光时间
    int frame_rate = main_window->camera_config_vector.at(main_window->now_show_num).frame_rate;        // 帧率
    int image_gain = main_window->camera_config_vector.at(main_window->now_show_num).image_gain;        // 图像增益
    // 写入spinBox
    ui->spinBox_1->setValue(exposure_time);
    ui->spinBox_2->setValue(frame_rate);
    ui->spinBox_3->setValue(image_gain);
    printf("获取相机参数成功~");
}
