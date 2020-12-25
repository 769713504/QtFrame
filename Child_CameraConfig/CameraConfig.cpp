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

}

// �������
void CameraConfig::saveSet() {

}

// ȡ������
void CameraConfig::cancelSet() {

}

// ����������
void CameraConfig::setComboBox() {
    ui->comboBox_0->clear();
    for (int i = 0; i < main_window->cam_sum; i++) ui->comboBox_0->addItem(QString::number(i));
    ui->comboBox_0->setCurrentIndex(main_window->now_show_num);
}

// ��ȡ��ǰ�������
void CameraConfig::getNowCamArgs() {

    int exposure_time = main_window->camera_config_vector.at(main_window->now_show_num).exposure_time;  // �ع�ʱ��
    int frame_rate = main_window->camera_config_vector.at(main_window->now_show_num).frame_rate;        // ֡��
    int image_gain = main_window->camera_config_vector.at(main_window->now_show_num).image_gain;        // ͼ������
    // д��spinBox
    ui->spinBox_1->setValue(exposure_time);
    ui->spinBox_2->setValue(frame_rate);
    ui->spinBox_3->setValue(image_gain);
    printf("��ȡ��������ɹ�~");
}
