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

    //声明槽函数
private slots:

    //开始或停止运行
    void startOrStopRun();

    //变更当前相机
    void changNowCamera(int cam_num = 88);

    void test();

public:
    MV_CC_DEVICE_INFO_LIST device_info;
    int ret{}, cam_sum{};

    //线程向label_main传图的控制列表:1原图 2灰度图 0关闭
    int label_main_control_array[6]{1, 0, 0, 0, 0, 0};

    bool is_run{false};

    //****************************************读取系统配置文件 ************************************************************
    int now_show_num{0};
    string config_path{""};
    string system_config_path;
    //    int system_config_obj;
    int run_mode;
    int save_mode;
    int save_type;
    char save_path[128];
    char read_path[128];
    char user_password[8];
    char admin_password[8];
    char product_type[64];
    char product_list[256];
    int control_rejector;
    char control_port[8];

    //******************************************读取相机配置文件***********************************************************

    vector<string> camera_config_path_vector;
    vector<string> camera_config_obj_vector;

    //获取相机路径Vector
    int getConfigPathVector();

    //获取相机对象Vector
    int getConfigObjVector();


    //显示当前编号的 参数列表
    void showNowCameraArgslist();

    //显示当前编号的图像
    void showImageToMainLabelByRadioButtonControl();

    //从缓存显示
    void displayFromCache();

    //******************************************************************************************************************

    vector<CMvCamera> camera_obj_vector;


    // ################################################################################################
    void startRun();

    void stopRun();

    //获取相机数量
    int getCameraSum();


    //获取相机对象Vector
    vector<CMvCamera> getCameraObjVector();

    //打开所有相机
    int openAllCamera();

    //循环开始取流
    int startAllGrabbing();

    //停止所有相机抓图
    int stopAllGrabbing();

    //关闭所有相机
    int closeAllCamera();

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