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

    // ����������
    setComboBox();
    // ��ȡ��ǰ�������
    getNowCamArgs();

    //����¼�
    connect(ui->checkBox_0, SIGNAL(clicked()), SLOT(disableComboBox()));            // ��ѡ
    connect(ui->pushButton_1, SIGNAL(clicked()), SLOT(saveSet()));                  // �������
    connect(ui->pushButton_2, SIGNAL(clicked()), SLOT(cancelSet()));                // ȡ������
}

CameraConfig::~CameraConfig() {
    delete ui;
}

// ��ѡ
void CameraConfig::disableComboBox() {
    if (ui->checkBox_0->isChecked()) {
        ui->comboBox_0->setEnabled(false);
    } else {
        ui->comboBox_0->setEnabled(true);
    }
}

// �������
void CameraConfig::saveSet() {
    main_window->is_change_param = true;
    setCamConfigArgs();
    close();

}

// ȡ������
void CameraConfig::cancelSet() {
    close();
}

// ����������
void CameraConfig::setComboBox() {
    ui->comboBox_0->clear();
    for (int i = 0; i < main_window->cam_sum; i++) ui->comboBox_0->addItem(QString::number(i));
    ui->comboBox_0->setCurrentIndex(main_window->now_show_num);
}

// ��ȡ��ǰ�������
void CameraConfig::getNowCamArgs() {

    int exposure_time = main_window->camera_config_struct_vector.at(main_window->now_show_num).exposure_time;  // �ع�ʱ��
    int frame_rate = main_window->camera_config_struct_vector.at(main_window->now_show_num).frame_rate;        // ֡��
    int image_gain = main_window->camera_config_struct_vector.at(main_window->now_show_num).image_gain;        // ͼ������
    // д��spinBox
    ui->spinBox_1->setValue(exposure_time);
    ui->spinBox_2->setValue(frame_rate);
    ui->spinBox_3->setValue(image_gain);
    printf("��ȡ��������ɹ�~");
}

// �����������
void CameraConfig::setCamConfigArgs() {
    // ��ȡ����
    int exposuretime = ui->spinBox_1->value();          // �ع�ʱ��
    int framerate = ui->spinBox_2->value();             // ֡��
    int gain = ui->spinBox_3->value();                  // ͼ������
    // �жϸ�ѡ��״̬
    if (ui->checkBox_0->isChecked()) {
        for (int i = 0; i < main_window->cam_sum; i++) {
            main_window->camera_config_struct_vector.at(i).exposure_time = exposuretime;
            main_window->camera_config_struct_vector.at(i).frame_rate = framerate;
            main_window->camera_config_struct_vector.at(i).image_gain = gain;
        }
        printf("������������Ѹ��³ɹ�~");
    } else {
        main_window->camera_config_struct_vector.at(main_window->now_show_num).exposure_time = exposuretime;
        main_window->camera_config_struct_vector.at(main_window->now_show_num).frame_rate = framerate;
        main_window->camera_config_struct_vector.at(main_window->now_show_num).image_gain = gain;

        printf("���%d�����Ѹ��³ɹ�~", main_window->now_show_num);
    }
}

