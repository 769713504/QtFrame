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



//相机配置文件列表结构体
struct CONFIG_FILE_LIST {
    int exposure_time;
    int image_gain;
    int frame_rate;
    int *train;
};


class Main : public QMainWindow {
Q_OBJECT

    //声明槽函数
private slots:

    //相机配置
    void Window_CameraConfig();

    //重启程序
    void restartDevice();

    // 原图/灰度图
    void changNowImage();

    //开始或停止运行
    void startOrStopRun();

    //关闭设备
    void closeDevice();

    //变更当前相机
    void changNowCamera(int cam_num);

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
    string camera_config_dir;
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

    //相机配置参数Vector
    vector<CONFIG_FILE_LIST> camera_config_vector;


    //获取相机路径Vector
    int getConfigPathVector();

    //获取相机对象Vector
    vector<CONFIG_FILE_LIST> getConfigObjVector();

    //显示当前编号的 参数列表
    void showNowCameraArgslist();

    //显示当前编号的图像
    void showImageToMainLabelByRadioButtonControl();

    //从缓存显示
    void displayFromCache();

    //******************************************************************************************************************

    vector<CMvCamera> camera_obj_vector;


    // 禁用单选按钮
    void enabledRadioButton();

    // 启用单选按钮
    void disabledRadioButton();

    // 显示与隐藏相机单选按钮: 按相机个数
    void hideRadioButton();

    // 检查密码的函数, 密码正确返回True
    bool confirmPassword(string utype = "user", string info = "权限验证");

    // ################################################################################################
    void startRun();

    void stopRun();

    //获取相机数量
    int getCameraSum();


    //获取相机对象Vector
    vector<CMvCamera> getCameraObjVector();


    //打开所有相机
    void openAllCamera();

    //循环开始取流
    void startAllGrabbing();

    //停止所有相机抓图
    void stopAllGrabbing();

    //关闭所有相机
    void closeAllCamera();

    void getSystemParameter();

    void getCameraConfigInfo();

    void defineVariable();

    void initializeVariable();

    void initializeSetting();

    explicit Main(QWidget *parent = nullptr);

    ~Main() override;

private:
    Ui::MainWindow *ui;

    void __DirectlyRestart();

    void __ReadyQuit();

    void saveConfig();

    void updateCsv();
};

#endif