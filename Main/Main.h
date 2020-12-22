#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ui_mainwindow.h>
#include "iostream"
#include  "CameraOperation.h"


namespace Ui {
    class MainWindow;
}

using namespace std;

class Main : public QMainWindow {
Q_OBJECT

public:
    MV_CC_DEVICE_INFO_LIST device_info;
    //设置系统配置路径
    string config_path = "./File_Config";


    int res, cam_sum;

    vector<CMvCamera> camera_obj_vector;


    string system_config_path;
    string system_config_obj;
    string save_path, read_path, user_password, admin_password, product_type, product_list, control_rejector, control_port;
    int run_mode{}, save_mode{}, save_type{};

    // ################################################################################################
    //获取相机数量
    int getCameraSum();

    //打开所有相机
    int openAllCamera();


    void getSystemParameter();

    void getCameraConfigInfo();

    void defineVariable();

    void initializeVariable();

    void initializeSetting();

    explicit Main(QWidget *parent = nullptr);

    ~Main() override;

private:
    Ui::MainWindow *ui;
};


#endif