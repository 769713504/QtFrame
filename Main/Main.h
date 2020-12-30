#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <map>

#include <QMainWindow>
#include <QtWidgets/QGroupBox>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QSpinBox>

#include <ui_mainwindow.h>
#include  "CameraOperation.h"
#include  "DetectionBase.h"

using namespace std;

namespace Ui {
    class MainWindow;
}

// Todo ------------------------------------------------  结构体定义  ----------------------------------------------------

//相机配置结构体
struct CAMERA_CONFIG_STRUCT {
    QString config_path;
    int exposure_time, image_gain, frame_rate;
    QPoint topLeft;
    QPoint bottomRight;
    int width, height;
};
// 图像缩放结构体
struct IMAGE_ZOOM_STRUCT {
    float zoom_width;
    float zoom_height;
};


class Main : public QMainWindow {
Q_OBJECT

public: // Todo -----------------------------------------  公有变量  -----------------------------------------------------

    //子窗口调用: 是否更改参数
    bool is_change_param{false}, is_Rejector{false};

    int cam_sum{}, now_show_num{};
    //相机配置结构体向量
    vector<CAMERA_CONFIG_STRUCT> camera_config_struct_vector;
    //视觉检测对象向量
    vector<DetectionBase *> visual_detection_object_vector;
    //所有相机 参数列表容器 的向量
    vector<vector<QSpinBox *> > allCamera_spinBoxVector_Vector;
    // 所有相机 缩放比例向量
    vector<IMAGE_ZOOM_STRUCT> allCamera_zoomRatio_mapVector;

    map<int, QString> *error_type_dict;

    map<QString, int> *params_dict;

    QString config_dir_path, system_config_file_path, camera_config_dir_path;
    //将要转 int
    QString run_mode, save_mode, save_type, save_path, read_path, control_rejector, control_port, simulate_camnum;
    //配置文件参数
    QString user_password, admin_password, product_title, product_list, product_type;

private: // Todo ----------------------------------------  私有变量  -----------------------------------------------------
    bool is_run{false};
    // 训练模式坐标
    int coord_array[4]{0};
    // 图像显示控制数组: 0关闭 1原图 2灰度图
    int label_main_control_array[6]{1, 0, 0, 0, 0, 0};
    // 相机对象向量
    vector<CMvCamera> camera_object_vector;
    // 参数列表向量
    vector<QGroupBox *> param_list_vector;

    //设备信息结构体
    MV_CC_DEVICE_INFO_LIST device_info_list;

    Ui::MainWindow *ui;

private slots: // Todo -----------------------------------  槽函数  -----------------------------------------------------

    void Window_ExceptionImage();

    void errorStop();

    void trainingMode();

    void rejectorSwitch();

    void monitorSwitch();

    // 改变参数字典
    void changeArgsDict(int cam_num = 0);

    // 变更当前相机
    void changNowCamera(int cam_num = 0);

    // 重启程序
    void restartDevice();

    //  原图/灰度图
    void changNowImage();

    //相机配置
    void Window_CameraConfig();

    // 系统配置
    void Window_SystemConfig();

    // 保存模式
    void Window_SaveMode();

    // 开始或停止运行
    void startOrStopRun();

    // 退出:关闭设备
    void closeDevice();

    void test();

public: // Todo -----------------------------------------  公有函数  -----------------------------------------------------

    explicit Main(QWidget *parent = nullptr);

    ~Main() override;

private: // Todo -----------------------------------------  私有函数  ----------------------------------------------------

    //重写关闭事件
    void closeEvent(QCloseEvent *event);

    //重写按键事件
    void keyPressEvent(QKeyEvent *event);

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
    void readSystemParam();

    // 读取相机配置
    void readCameraConfig_ToParamStructVector();

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

    void writeSystemConfig();

    void writeCameraConfig_FromParamStructVector();

    void deleteExceptionDirectory();

    void allCameraTriggerOnce(int num);

    void setAllCameraParam();

    void getArgsListObjList();

    void createArgsListDict();

    void createExceptionDirectory();

    void updateAllCoordinates();

    void startUpdataCsv();

    void setRejectorButton();

    void getVisualDetectionObjectVector();


    void trimCoordinates(int *array);

    void updateCameraConfig(int *array);

    void updateNowCoord(int *array);

    void getAllCameraZoomRatioVector();
};

#endif