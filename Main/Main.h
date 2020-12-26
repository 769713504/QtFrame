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


// Todo ------------------------------------------------  结构体定义  ----------------------------------------------------

//相机配置结构体
struct CAMERA_CONFIG_STRUCT {
    int exposure_time;
    int image_gain;
    int frame_rate;
    int *train;
};


class Main : public QMainWindow {
Q_OBJECT

public: // Todo -----------------------------------------  公有变量  -----------------------------------------------------

    //子窗口调用: 是否更改参数
    bool is_change_param{false};

    int cam_sum{}, now_show_num{};
    //相机配置结构体向量
    vector<CAMERA_CONFIG_STRUCT> camera_config_struct_vector;

private: // Todo ----------------------------------------  私有变量  -----------------------------------------------------

    bool is_run{false};

    int run_mode{}, save_mode{}, save_type{};
    // 剔除状态
    int control_rejector;
    // 图像显示控制数组: 0关闭 1原图 2灰度图
    int label_main_control_array[6]{1, 0, 0, 0, 0, 0};

    string config_dir_path{}, system_config_file_path{}, camera_config_dir_path{};

    char save_path[128], read_path[128], user_password[8], admin_password[8], product_type[64], product_list[256], control_port[8];

    // 相机对象向量
    vector<CMvCamera> camera_object_vector;

    MV_CC_DEVICE_INFO_LIST device_info_list;

    Ui::MainWindow *ui;

private slots: // Todo -----------------------------------  槽函数  -----------------------------------------------------

    // 变更当前相机
    void changNowCamera(int cam_num);

    // 重启程序
    void restartDevice();

    //  原图/灰度图
    void changNowImage();

    //相机配置
    void Window_CameraConfig();

    // 开始或停止运行
    void startOrStopRun();

    // 退出:关闭设备
    void closeDevice();

    void test();

public: // Todo -----------------------------------------  公有函数  -----------------------------------------------------

    explicit Main(QWidget *parent = nullptr);

    ~Main() override;

private: // Todo -----------------------------------------  私有函数  ----------------------------------------------------

    //获取相机对象Vector
    void getCameraConfigParamStructToVector();

    //显示当前编号的 参数列表
    void showNowCameraArgslist();

    //显示当前编号的图像
    void showImageToMainLabelByRadioButtonControl();

    //从缓存显示
    void displayFromCache();

    // 禁用单选按钮
    void enabledRadioButton();

    // 启用单选按钮
    void disabledRadioButton();

    // 显示与隐藏相机单选按钮: 按相机个数
    void hideRadioButton();

    // 开始运行
    void startRun();

    // 停止运行
    void stopRun();

    // 获取相机数量
    int getCameraSum();

    // 获取相机对象Vector
    void getCameraObjectToVector();

    // 打开所有相机
    void openAllCamera();

    // 循环开始取流
    void startAllGrabbing();

    // 停止所有相机抓图
    void stopAllGrabbing();

    // 关闭所有相机
    void closeAllCamera();

    // 获取系统参数
    void getSystemParam();

    // 读取相机配置
    void getCameraConfigInfo();

    // 定义变量
    void defineVariable();

    // 初始化变量
    void initializeVariable();

    // 初始化设置
    void initializeSetting();

    void __DirectlyRestart();

    void __ReadyQuit();

    void saveConfig();

    void updateCsv();

    // 检查密码的函数, 密码正确返回True
    bool confirmPassword(string utype = "user", string info = "权限验证");
};

#endif