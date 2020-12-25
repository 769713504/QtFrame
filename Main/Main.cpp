#include <QMessageBox>
#include <QInputDialog>
#include   "Main.h"
#include   "Child_CameraConfig/CameraConfig.h"

void Main::test() {

    confirmPassword("admin", "权限验证");
    cout << "Test~" << endl;
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
    //枚举设备: 获取相机数量(从设备信息中)
    cam_sum = getCameraSum();
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



    //获取设备容器
    camera_obj_vector = getCameraObjVector();
    //打开设备
    openAllCamera();




    // 单选按钮
    connect(ui->radioButton_0, &QPushButton::clicked, [=] { changNowCamera(0); });
    connect(ui->radioButton_1, &QPushButton::clicked, [=] { changNowCamera(1); });
    connect(ui->radioButton_2, &QPushButton::clicked, [=] { changNowCamera(2); });
    connect(ui->radioButton_3, &QPushButton::clicked, [=] { changNowCamera(3); });
    connect(ui->radioButton_4, &QPushButton::clicked, [=] { changNowCamera(4); });
    connect(ui->radioButton_5, &QPushButton::clicked, [=] { changNowCamera(5); });
    // 普通按钮
    connect(ui->pushButton_00, SIGNAL(clicked()), SLOT(restartDevice()));                       // 重启程序
    connect(ui->pushButton_01, SIGNAL(clicked()), SLOT(changNowImage()));                       // 原始图像/灰度图
    connect(ui->pushButton_02, SIGNAL(clicked()), SLOT(Window_CameraConfig()));                 // 启动/停止运行

    connect(ui->pushButton_05, SIGNAL(clicked()), SLOT(startOrStopRun()));                      // 启动/停止运行
    connect(ui->pushButton_10, SIGNAL(clicked()), SLOT(closeDevice()));                         // 退出:关闭程序
    connect(ui->pushButton_09, SIGNAL(clicked()), SLOT(test()));                                // 退出:关闭程序
}

//Todo 已完成功能 ######################################################################################################


//子窗口-->相机配置
void Main::Window_CameraConfig() {
    //拦截无相机
    if (cam_sum == 0) {
        QMessageBox::about(this, "无相机", "未检测到相机,该项不能设置!");
        return;
    }
    //权限验证
    if (!confirmPassword("user", "相机配置"))
        return;
    CameraConfig camera_config_window(this);
    camera_config_window.exec();
}

//重启程序
void Main::restartDevice() {

    __DirectlyRestart();

}


// 彩图/灰度图
void Main::changNowImage() {
    if (ui->pushButton_01->isChecked()) {
        label_main_control_array[now_show_num] = 2;
        ui->pushButton_01->setText("灰度图");
    } else {
        label_main_control_array[now_show_num] = 1;
        ui->pushButton_01->setText("原始图像");
    }
    // 从缓存图显示
    // displayFromCache()
}

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
    ///**/
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
void Main::closeDevice() {/**/
    /**/
    //权限验证

    //关闭所有相机
    closeAllCamera();
    //    exit(0);

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
void Main::openAllCamera() {

    printf("Open camera:\n");
    for (int cam_num = 0; cam_num < cam_sum; cam_num++) {
        camera_obj_vector.at(cam_num).Open();
    }
}

//全部开始抓取
void Main::startAllGrabbing() {

    printf("Start grabbing\n: ");
    for (int cam_num = 0; cam_num < cam_sum; cam_num++) {
        camera_obj_vector.at(cam_num).StartGrabbing();
    }
}

//停止所有抓取
void Main::stopAllGrabbing() {
    printf("Stop grabbing:\n");
    for (int cam_num = 0; cam_num < cam_sum; cam_num++) {
        camera_obj_vector.at(cam_num).StopGrabbing();
    }
}

//关闭所有相机
void Main::closeAllCamera() {
    printf("Close camera:\n");
    for (int cam_num = 0; cam_num < cam_sum; cam_num++) {
        camera_obj_vector.at(cam_num).Close();
    }
}

// Todo #############################################################################################################

// 读取系统参数
void Main::getSystemParameter() {

    system_config_path = config_path + "config.ini";

    char *lpPath = new char[128];
    strcpy(lpPath, system_config_path.data());

    run_mode = GetPrivateProfileInt("run", "mode", -66, lpPath);
    save_mode = GetPrivateProfileInt("save", "mode", -66, lpPath);
    save_type = GetPrivateProfileInt("save", "type", -66, lpPath);

    GetPrivateProfileString("save", "path", "", save_path, 128, lpPath);
    GetPrivateProfileString("read", "path", "", read_path, 128, lpPath);

    GetPrivateProfileString("password", "user", "", user_password, 16, lpPath);
    GetPrivateProfileString("password", "admin", "", admin_password, 16, lpPath);

    GetPrivateProfileString("product", "type", "", product_type, 128, lpPath);
    GetPrivateProfileString("product", "list", "", product_list, 256, lpPath);

    control_rejector = GetPrivateProfileInt("control", "rejector", -66, lpPath);
    GetPrivateProfileString("control", "port", "", control_port, 8, lpPath);

    delete[] lpPath;
}

//读取相机配置
void Main::getCameraConfigInfo() {
    camera_config_dir = config_path + product_type + "/";
    //获取相机配置对象Vector
    camera_config_vector = getConfigObjVector();
}

//获取相机对象Vector
vector<CONFIG_FILE_LIST> Main::getConfigObjVector() {

    vector<CONFIG_FILE_LIST> _camera_config_vector;
    printf("+++++++++++++++++++++++++++++++++%d+++++++++++++++++++++++++++\n", cam_sum);
    char *lpPath = new char[128];
    int exposure_time, image_gain, frame_rate, train[4];

    for (int i = 0; i < cam_sum; i++) {
        string camera_config_path = camera_config_dir + "camera" + to_string(i) + ".ini";
        strcpy(lpPath, camera_config_path.data());

        exposure_time = GetPrivateProfileInt("camera", "exposure_time", -66, lpPath);
        image_gain = GetPrivateProfileInt("camera", "image_gain", -66, lpPath);
        frame_rate = GetPrivateProfileInt("camera", "frame_rate", -66, lpPath);
        train[0] = GetPrivateProfileInt("train", "x1", -66, lpPath);
        train[1] = GetPrivateProfileInt("train", "x2", -66, lpPath);
        train[2] = GetPrivateProfileInt("train", "y1", -66, lpPath);
        train[3] = GetPrivateProfileInt("train", "y2", -66, lpPath);

        //todo 参数未添加

        CONFIG_FILE_LIST camera_config_list{exposure_time, image_gain, frame_rate, train};
        _camera_config_vector.push_back(camera_config_list);

    }
    return _camera_config_vector;
}

//获取相机路径Vector
int Main::getConfigPathVector() {

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

//直接重启: 内部调用
void Main::__DirectlyRestart() {
    __ReadyQuit();
    //返回重启编号
    QApplication::exit(6);
}

// 准备离开: 内部调用,结束任务
void Main::__ReadyQuit() {
    // 持久化参数到文件
    saveConfig();
    // 停止抓取
    stopRun();
    // 关闭所有相机
    closeAllCamera();
    // 保存csv
    updateCsv();

}

void Main::saveConfig() {
    // todo

}

void Main::updateCsv() {
    //todo
}

//检查密码的函数, 密码正确返回True
bool Main::confirmPassword(string utype, string info) {

    bool ok;
    string password, text;
    if (utype == "admin") {
        text = QInputDialog::getText(this, QString::fromStdString(info), "请输入管理员密码~", QLineEdit::Password, "",
                                     &ok).toStdString();
        password = admin_password;
        cout << text << endl;
    } else {
        text = QInputDialog::getText(this, QString::fromStdString(info), "请输入用户密码~", QLineEdit::Password, "",
                                     &ok).toStdString();
        password = user_password;
    }

    if (ok) {
        if (text == password) return true;
        else if (QMessageBox::warning(this, "密码错误", "密码应为1~6位字符!\\n是否重试?", QMessageBox::Yes | QMessageBox::No) ==
                 QMessageBox::Yes)
            return confirmPassword(utype, info);
        else return false;
    } else return false;
}

// 显示与隐藏相机单选按钮: 按相机个数
void Main::hideRadioButton() {
    if (cam_sum <= 0) { ui->radioButton_0->setVisible(false); } else { ui->radioButton_0->setVisible(true); }
    if (cam_sum <= 1) { ui->radioButton_1->setVisible(false); } else { ui->radioButton_1->setVisible(true); }
    if (cam_sum <= 2) { ui->radioButton_2->setVisible(false); } else { ui->radioButton_2->setVisible(true); }
    if (cam_sum <= 3) { ui->radioButton_3->setVisible(false); } else { ui->radioButton_3->setVisible(true); }
    if (cam_sum <= 4) { ui->radioButton_4->setVisible(false); } else { ui->radioButton_4->setVisible(true); }
    if (cam_sum <= 5) { ui->radioButton_5->setVisible(false); } else { ui->radioButton_5->setVisible(true); }


}

// 禁用单选按钮
void Main::disabledRadioButton() {
    ui->radioButton_0->setEnabled(false);
    ui->radioButton_1->setEnabled(false);
    ui->radioButton_2->setEnabled(false);
    ui->radioButton_3->setEnabled(false);
    ui->radioButton_4->setEnabled(false);
    ui->radioButton_5->setEnabled(false);

}

// 启用单选按钮
void Main::enabledRadioButton() {
    ui->radioButton_0->setEnabled(true);
    ui->radioButton_1->setEnabled(true);
    ui->radioButton_2->setEnabled(true);
    ui->radioButton_3->setEnabled(true);
    ui->radioButton_4->setEnabled(true);
    ui->radioButton_5->setEnabled(true);
}