#include <QMessageBox>
#include   "Main.h"


void Main::test() {
    cout << "测试" << endl;
}

int fun01() {
    cout << 999 << endl;
    return 0;
}

// 析构
Main::~Main() {
    delete ui;
}

// 构造函数
Main::Main(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    //引入GUI
    ui->setupUi(this);
    //设置系统配置路径
    config_path = "./File_Config/";

    ui->label_main;
    //读取系统参数
    getSystemParameter();
    //读取相机配置
    getCameraConfigInfo();
    //定义变量
    defineVariable();
    //从方法初始化变量
    initializeVariable();
    //初始化设置
    initializeSetting();


    //枚举设备: 获取相机数量(从设备信息中)
    cam_sum = getCameraSum();
    //获取设备容器
    camera_obj_vector = getCameraObjVector();
    //打开设备
    ret = openAllCamera();

    //单选按钮
    connect(ui->radioButton_0, &QPushButton::clicked, [=] { changNowCamera(0); });
    connect(ui->radioButton_1, &QPushButton::clicked, [=] { changNowCamera(1); });
    connect(ui->radioButton_2, &QPushButton::clicked, [=] { changNowCamera(2); });
    connect(ui->radioButton_3, &QPushButton::clicked, [=] { changNowCamera(3); });
    connect(ui->radioButton_4, &QPushButton::clicked, [=] { changNowCamera(4); });
    connect(ui->radioButton_5, &QPushButton::clicked, [=] { changNowCamera(5); });
    // 普通按钮
    connect(ui->pushButton_05, SIGNAL(clicked()), this, SLOT(startOrStopRun()));//循环抓取
}

//Todo 已完成功能 ######################################################################################################

//变更当前相机:param cam_num:相机编号
void Main::changNowCamera(int cam_num) {
    if (!cam_sum || cam_num == now_show_num) {
        return;
    }
    now_show_num = cam_num;
    //显示当前编号的 参数列表
    showNowCameraArgslist();
    //显示当前编号的图像
    showImageToMainLabelByRadioButtonControl();
    //从缓存显示
    displayFromCache();
    //    显示训练模式下的矩形区域
    //    label_main.updateRect(*(self.train_coordinates_list[self.now_show_num]))
}

//显示当前相机的参数列表 todo
void Main::showNowCameraArgslist() {
    //
    //    for (int cam_num ; cam_num<cam_sum;cam_num++){
    //        if (cam_num=now_show_num) {
    //            cout << '参数列表%s   显示' % i << endl;
    //
    //        }
    //
    //
    //    }
    //    for i, args_list_obj in enumerate(self.args_list_obj_list):
    //    if i == self.now_show_num:
    //    print('参数列表%s   显示' % i)
    //    args_list_obj.setVisible(True)
    //    else:
    //    args_list_obj.setVisible(False)

}


//点击相机单选按钮,显示图片到主标签(通过开关控制)
void Main::showImageToMainLabelByRadioButtonControl() {
    //置空
    for (int i = 0; i < cam_sum; i++)
        label_main_control_array[i] = 0;
    //写入
    if (ui->pushButton_01->isChecked())
        label_main_control_array[now_show_num] = 2;
    else
        label_main_control_array[now_show_num] = 1;
}

//从缓存显示 todo
void Main::displayFromCache() {


}

//运行或停止
void Main::startOrStopRun() {

    if (!cam_sum) {
        ui->pushButton_05->setChecked(false);
        QMessageBox::critical(this, "警告", "未检测到相机!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }

    if (ui->pushButton_05->isChecked()) {
        startRun();
        ui->pushButton_05->setText("停止运行");
        is_run = true;
    } else {
        stopRun();
        ui->pushButton_05->setText("运行程序");
        is_run = false;

    }
    // 保存CSV
    //        self.updateCsv()
}

//运行
void Main::startRun() {
    if (!is_run) {
        startAllGrabbing();
    }
}

//停止
void Main::stopRun() {
    if (is_run) {
        stopAllGrabbing();
    }
}

//获取相机数量
int Main::getCameraSum() {
    if (0 == CMvCamera::EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &device_info)) {
        return device_info.nDeviceNum;
    }
    return 0;
}

//获取相机对象Vector
vector<CMvCamera> Main::getCameraObjVector() {
    vector<CMvCamera> _camera_obj_vector;
    for (int cam_num = 0; cam_num < cam_sum; cam_num++) {
        CAMERA_PARAMS_LIST camera_params_list{cam_num, &device_info, ui->label_main, label_main_control_array};
        //创建相机对象
        CMvCamera camera_obj(&camera_params_list);
        _camera_obj_vector.push_back(camera_obj);
    }
    return _camera_obj_vector;
}

//打开所有相机
int Main::openAllCamera() {
    ret = 0;
    for (int cam_num = 0; cam_num < cam_sum; cam_num++) {
        ret += camera_obj_vector.at(cam_num).Open();
        cout << "Camera " << cam_num << " is on~" << endl;
    }
    return ret;
}

//全部开始抓取
int Main::startAllGrabbing() {
    cout << "camera sum" << cam_sum << endl;

    for (int cam_num = 0; cam_num < cam_sum; cam_num++) {
        cout << "camera" << cam_num << "  begin" << endl;
        camera_obj_vector.at(cam_num).StartGrabbing();
    }
}

//停止所有抓取
int Main::stopAllGrabbing() {
    for (int cam_num = 0; cam_num < cam_sum; cam_num++) {
        camera_obj_vector.at(cam_num).StopGrabbing();

        cout << "tingzhi  " << cam_num << endl;
    }
}

//关闭所有相机
int Main::closeAllCamera() {
    for (int cam_num = 0; cam_num < cam_sum; cam_num++) {
        camera_obj_vector.at(cam_num).Close();
    }
}

// Todo #############################################################################################################

// 读取系统参数
void Main::getSystemParameter() {

    char *lpPath = new char[128];

    system_config_path = config_path + "config.ini";
    strcpy(lpPath, system_config_path.data());

    run_mode = GetPrivateProfileInt("run", "mode", -66, lpPath);
    save_mode = GetPrivateProfileInt("save", "mode", -66, lpPath);
    save_type = GetPrivateProfileInt("save", "type", -66, lpPath);

    GetPrivateProfileString("save", "path", "", save_path, 128, lpPath);
    GetPrivateProfileString("read", "path", "", read_path, 128, lpPath);

    GetPrivateProfileString("password", "user", "", user_password, 16, lpPath);
    GetPrivateProfileString("password", "admin", "", admin_password, 16, lpPath);

    GetPrivateProfileString("product", "type", "", product_type, 128, lpPath);
    GetPrivateProfileString("product", "list", "", product_type, 256, lpPath);

    control_rejector = GetPrivateProfileInt("control", "rejector", -66, lpPath);
    GetPrivateProfileString("control", "port", "", control_port, 8, lpPath);

    delete[] lpPath;
}

//读取相机配置
void Main::getCameraConfigInfo() {
    getConfigPathVector();
    getConfigObjVector();
}

//
int Main::getConfigPathVector() {

}

//
int Main::getConfigObjVector() {

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







