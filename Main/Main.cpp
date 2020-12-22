#include   "Main.h"

// 构造函数
Main::Main(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    //引入GUI
    ui->setupUi(this);
    //枚举设备: 获取相机数量(从设备信息中)
    cam_sum = getCameraSum();
    //打开设备
    res = openAllCamera();
}

// 析构
Main::~Main() {
    delete ui;
}

// 获取相机数量
int Main::getCameraSum() {
    if (0 == CMvCamera::EnumDevices(IN MV_GIGE_DEVICE | MV_USB_DEVICE, &device_info)) {
        return device_info.nDeviceNum;
    }
    return 0;
}

int Main::openAllCamera() {
    for (int cam_num = 0; cam_num < cam_sum; cam_num++) {
        //创建相机对象
        CMvCamera camera_obj(&device_info, cam_num);
        camera_obj_vector.push_back(camera_obj);
    }


    for (int i = 0; i < cam_sum; i++) {
        cout << "打开相机" << i << endl;
        //        camera_obj_vector.at(i).Open();
    }


    return 0;
}

bool openAllCamera() {


}

// 读取系统参数
void Main::getSystemParameter() {

}

//读取相机配置
void Main::getCameraConfigInfo() {

}

//定义变量
void Main::defineVariable() {

}

//初始化变量
void Main::initializeVariable() {

}

//初始化设置
void Main::initializeSetting() {


}







