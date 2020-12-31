#include <QMessageBox>
#include <QInputDialog>
#include <QSettings>
#include "Main.h"
#include "Child_CameraConfig/CameraConfig.h"
#include "Child_SystemConfig/SystemConfig.h"
#include "CV/SetCameraMap.h"
#include <QDebug>
#include <QtSerialPort/QSerialPort>        //提供访问串口的功能
#include <QtSerialPort/QSerialPortInfo>    //提供系统中存在的串口的信息

void Main::test() {
    // QStringList m_serialPortName;

            foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            qDebug() << "端口 : " << info.portName();
            qDebug() << "描述 : " << info.description();
            qDebug() << "制造商: " << info.manufacturer();
            qDebug() << "序列号: " << info.serialNumber();
            qDebug() << "系统位置: " << info.systemLocation();
        }

}

Main::~Main() {
    delete ui;
}

// 构造函数
Main::Main(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    // 引入GUI
    ui->setupUi(this);

    // 设置系统配置路径
    config_dir_path = "./File_Config/";
    // 枚举设备: 获取相机数量(从设备信息中)
    cam_sum = getCameraSum();
    // 读取系统参数
    readSystemParam();
    // 读取相机配置
    readCameraConfig_ToParamStructVector();

    // 定义变量
    defineVariable();
    // 从方法初始化变量
    initializeVariable();
    // 初始化设置
    initializeSetting();

    // 单选按钮
    connect(ui->radioButton_0, &QPushButton::clicked, [=] { changNowCamera(0); });
    connect(ui->radioButton_1, &QPushButton::clicked, [=] { changNowCamera(1); });
    connect(ui->radioButton_2, &QPushButton::clicked, [=] { changNowCamera(2); });
    connect(ui->radioButton_3, &QPushButton::clicked, [=] { changNowCamera(3); });
    connect(ui->radioButton_4, &QPushButton::clicked, [=] { changNowCamera(4); });
    connect(ui->radioButton_5, &QPushButton::clicked, [=] { changNowCamera(5); });
    // 普通按钮
    connect(ui->pushButton_00, SIGNAL(clicked()), SLOT(restartDevice()));                   // 重启程序
    connect(ui->pushButton_01, SIGNAL(clicked()), SLOT(changNowImage()));                   // 原始图像/灰度图
    connect(ui->pushButton_02, SIGNAL(clicked()), SLOT(Window_CameraConfig()));             // 相机配置
    connect(ui->pushButton_03, SIGNAL(clicked()), SLOT(Window_ExceptionImage()));           // 异常图像
    connect(ui->pushButton_04, SIGNAL(clicked()), SLOT(errorStop()));                       // 出错暂停
    connect(ui->pushButton_05, SIGNAL(clicked()), SLOT(startOrStopRun()));                  // 启动/停止运行
    connect(ui->pushButton_06, SIGNAL(clicked()), SLOT(trainingMode()));                    // 训练模式
    connect(ui->pushButton_07, SIGNAL(clicked()), SLOT(rejectorSwitch()));                  // 剔除开关
    connect(ui->pushButton_08, SIGNAL(clicked()), SLOT(monitorSwitch()));                   // 监视状态
    connect(ui->pushButton_09, SIGNAL(clicked()), SLOT(test()));                            // 更换产品
    connect(ui->pushButton_10, SIGNAL(clicked()), SLOT(closeDevice()));                     // 退出:关闭程序
    // 菜单按钮
    connect(ui->action_01, SIGNAL(triggered()), this, SLOT(Window_SystemConfig()));  // ---->系统配置
    connect(ui->action_02, SIGNAL(triggered()), this, SLOT(Window_SaveMode()));      // ---->保存模式
}

// Todo 已完成功能 ######################################################################################################

// 子窗口-->保存模式
void Main::Window_SaveMode() {
}

// 子窗口-->系统配置
void Main::Window_SystemConfig() {
    // 权限验证
    if (!confirmPassword("admin", "系统配置")) return;
    //显示窗口
    SystemConfig system_config_window(this);
    if (!system_config_window.exec()) {
        if (is_change_param) {
            if (QMessageBox::warning(this, "重启", "系统参数已更改! 重启生效\n是否重启?", QMessageBox::Yes | QMessageBox::No) ==
                QMessageBox::Yes) {
                __DirectlyRestart();
            }
        }
    }
}

// 子窗口-->相机配置
void Main::Window_CameraConfig() {
    // 拦截无相机
    if (cam_sum == 0) {
        QMessageBox::about(this, "无相机", "未检测到相机,该项不能设置!");
        return;
    }
    // 权限验证
    if (!confirmPassword("user", "相机配置")) return;

    //显示窗口
    CameraConfig camera_config_window(this);
    if (!camera_config_window.exec()) {
        if (is_change_param) {
            if (QMessageBox::warning(this, "重启", "相机参数已更改! 重启生效\n是否重启?", QMessageBox::Yes | QMessageBox::No) ==
                QMessageBox::Yes) {
                __DirectlyRestart();
            }
        }
    }
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

// 变更当前相机:param cam_num:相机编号
void Main::changNowCamera(int cam_num) {
    if (!cam_sum) { return; }

    now_show_num = cam_num;
    // 显示当前编号的 参数列表
    showNowCameraArgslist();
    // 显示当前编号的图像
    showImageToMainLabelByRadioButtonControl();
    // 从缓存显示
    displayFromCache();
    // 显示训练模式下的矩形区域
    ui->label_main->updateRect(camera_config_struct_vector.at(now_show_num).topLeft.x(),
                               camera_config_struct_vector.at(now_show_num).topLeft.y(),
                               camera_config_struct_vector.at(now_show_num).bottomRight.x(),
                               camera_config_struct_vector.at(now_show_num).bottomRight.y());
}

// 显示当前相机的参数列表
void Main::showNowCameraArgslist() {
    for (int i = 0; i < cam_sum; i++) {
        if (i == now_show_num) {
            printf("Show param list %d \n", i);
            param_list_vector.at(i)->setVisible(true);
        } else {
            param_list_vector.at(i)->setVisible(false);
        }
    }
}

// 点击相机单选按钮,显示图片到主标签(通过开关控制)
void Main::showImageToMainLabelByRadioButtonControl() {
    // 置空
    for (int i = 0; i < cam_sum; i++)
        label_main_control_array[i] = 0;
    // 写入
    if (ui->pushButton_01->isChecked())
        label_main_control_array[now_show_num] = 2;
    else
        label_main_control_array[now_show_num] = 1;
}

// 从缓存显示 todo
void Main::displayFromCache() {


}

// 重启程序
void Main::restartDevice() {
    if (!confirmPassword("user", "重启程序")) { return; }

    __DirectlyRestart();
}

// 退出程序
void Main::closeDevice() {/**/
    if (!confirmPassword("user", "退出程序")) { return; }
    __ReadyQuit();
    // 删除缓存目录
    deleteExceptionDirectory();
    QApplication::exit(0);

}

void Main::closeEvent(QCloseEvent *event) {

    if (!confirmPassword("user", "关闭")) {
        event->ignore();
    } else {
        __ReadyQuit();
        // 删除缓存目录
        deleteExceptionDirectory();
        event->accept();
    }
}

// 重写按键事件
void Main::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        closeDevice();
    } else if (event->key() == Qt::Key_F12) {
        // 若"模拟运行"按钮未选中
        if (ui->checkBox->isChecked()) {
            // 触发一次
            allCameraTriggerOnce(-1);
        }
    }
}

// 运行或停止
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

// 运行
void Main::startRun() {
    if (!is_run) {
        startAllGrabbing();
    }
}

// 停止
void Main::stopRun() {
    if (is_run) {
        stopAllGrabbing();
    }
}

// 获取相机数量
int Main::getCameraSum() {
    if (0 == CMvCamera::EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &device_info_list)) {
        return device_info_list.nDeviceNum;
    }
    return 0;
}

// 获取相机对象Vector
void Main::getCameraObjectToVector() {
    for (int cam_num = 0; cam_num < cam_sum; cam_num++) {
        CAMERA_PARAMS_LIST camera_params_list{cam_num, &device_info_list, ui->label_main, label_main_control_array};
        // 创建相机对象
        CMvCamera camera_obj(&camera_params_list);
        camera_object_vector.push_back(camera_obj);
    }
}

// 打开所有相机
void Main::openAllCamera() {

    printf("Open camera:\n");
    for (int cam_num = 0; cam_num < cam_sum; cam_num++) {
        camera_object_vector.at(cam_num).Open();
    }
}

// 全部开始抓取
void Main::startAllGrabbing() {

    printf("Start grabbing\n: ");
    for (int cam_num = 0; cam_num < cam_sum; cam_num++) {
        camera_object_vector.at(cam_num).StartGrabbing();
    }
}

// 停止所有抓取
void Main::stopAllGrabbing() {
    printf("Stop grabbing:\n");
    for (int cam_num = 0; cam_num < cam_sum; cam_num++) {
        camera_object_vector.at(cam_num).StopGrabbing();
    }
}

// 关闭所有相机
void Main::closeAllCamera() {
    printf("Close camera:\n");
    for (int cam_num = 0; cam_num < cam_sum; cam_num++) {
        camera_object_vector.at(cam_num).Close();
    }
}

// Todo #############################################################################################################

// 读取系统参数
void Main::readSystemParam() {
    system_config_file_path = config_dir_path + "config.ini";

    QSettings configIniRead(system_config_file_path, QSettings::IniFormat);

    run_mode = configIniRead.value("run/mode").toString();
    save_mode = configIniRead.value("save/mode").toString();
    save_type = configIniRead.value("save/type").toString();
    save_path = configIniRead.value("save/path").toString();
    read_path = configIniRead.value("read/path").toString();
    control_rejector = configIniRead.value("control/rejector").toString();
    control_port = configIniRead.value("control/port").toString();
    simulate_camnum = configIniRead.value("simulate/camnum").toString();
    user_password = configIniRead.value("password/user").toString();
    admin_password = configIniRead.value("password/admin").toString();
    product_title = configIniRead.value("product/title").toString();
    product_list = configIniRead.value("product/list").toString();
    product_type = configIniRead.value("product/type").toString();

}

// 写入系统参数
void Main::writeSystemConfig() {

    QSettings configIniRead(system_config_file_path, QSettings::IniFormat);

    configIniRead.setValue("run/mode", run_mode);
    configIniRead.setValue("save/mode", save_mode);
    configIniRead.setValue("save/type", save_type);
    configIniRead.setValue("save/path", save_path);
    configIniRead.setValue("read/path", read_path);
    configIniRead.setValue("control/rejector", control_rejector);
    configIniRead.setValue("control/port", control_port);
    configIniRead.setValue("simulate/camnum", simulate_camnum);
    configIniRead.setValue("password/user", user_password);
    configIniRead.setValue("password/admin", admin_password);
    configIniRead.setValue("product/title", product_title);
    configIniRead.setValue("product/list", product_list);
    configIniRead.setValue("product/type", product_type);
}

// 读取相机配置
void Main::readCameraConfig_ToParamStructVector() {

    camera_config_dir_path = config_dir_path + product_type + "/";

    QString camera_config_path;
    int exposure_time, image_gain, frame_rate;
    int x1, y1, x2, y2;
    int width, height;

    for (int i = 0; i < cam_sum; i++) {
        camera_config_path = camera_config_dir_path + "camera" + QString::number(i) + ".ini";
        QSettings configIniRead(camera_config_path, QSettings::IniFormat);

        exposure_time = configIniRead.value("camera/exposure_time").toInt();
        image_gain = configIniRead.value("camera/image_gain").toInt();
        frame_rate = configIniRead.value("camera/frame_rate").toInt();
        int train[4];
        x1 = configIniRead.value("train/x1").toInt();
        y1 = configIniRead.value("train/y1").toInt();
        x2 = configIniRead.value("train/x2").toInt();
        y2 = configIniRead.value("train/y2").toInt();

        width = configIniRead.value("image/width").toInt();
        height = configIniRead.value("image/height").toInt();
        // todo 未添加de参数

        CAMERA_CONFIG_STRUCT camera_config_list{camera_config_path, exposure_time, image_gain, frame_rate,
                                                QPoint(x1, y1), QPoint(x2, y2), width, height};

        camera_config_struct_vector.push_back(camera_config_list);
    }
}


// 写入相机参数
void Main::writeCameraConfig_FromParamStructVector() {

    for (int i = 0; i < cam_sum; i++) {
        QSettings configIniRead(camera_config_struct_vector.at(i).config_path, QSettings::IniFormat);

        configIniRead.setValue("camera/exposure_time", camera_config_struct_vector.at(i).exposure_time);
        configIniRead.setValue("camera/image_gain", camera_config_struct_vector.at(i).image_gain);
        configIniRead.setValue("camera/frame_rate", camera_config_struct_vector.at(i).frame_rate);

        configIniRead.setValue("train/x1", camera_config_struct_vector.at(i).topLeft.x());
        configIniRead.setValue("train/y1", camera_config_struct_vector.at(i).topLeft.y());
        configIniRead.setValue("train/x2", camera_config_struct_vector.at(i).bottomRight.x());
        configIniRead.setValue("train/y2", camera_config_struct_vector.at(i).bottomRight.y());

        configIniRead.setValue("image/width", camera_config_struct_vector.at(i).width);
        configIniRead.setValue("image/height", camera_config_struct_vector.at(i).height);
        // todo 未添加de参数
    }
}


// 定义变量
void Main::defineVariable() {

}

// 初始化变量
void Main::initializeVariable() {

    // 异常图像存储路径
    createExceptionDirectory();
    // 传感器控制类对象
    // self.sensor_control_obj = SensorSerialPortControl(self.control_port) todo
    // 视觉检测对象向量
    getVisualDetectionObjectVector();
    // 获取 参数列表对象 列表
    getArgsListObjList();
    // 初始化参数列表: 向参数列表中添加参数
    createArgsListDict();

    // 所有相机配置文件中的坐标列表
    // self.train_coordinates_list = self.getTrainCoordinatesList()
    // 所有相机配置文件中 图像缩放比列表[[1.6,1.6],[x,y],...]
    getAllCameraZoomRatioVector();
    // 初始化日表对象
    // self.csv_path = self.getCsvPath()
    // "批"时间ID
    // self.batch_time_number = self.getTimeNumber()
    // 相机操作对象列表(含虚拟)
    // self.camera_operation_obj_list = self.getOperationObjList()
}

// 初始化设置
void Main::initializeSetting() {

    // 隐藏多余的单选按钮
    hideRadioButton();
    // 更改产品标签
    // self.label_product.setText(self.product_type)  todo
    // 监视状态
    monitorSwitch();
    // 初始化单选按钮
    changNowCamera();
    // 初始化所有相机坐标
    updateAllCoordinates();
    // 启动更新csv线程
    startUpdataCsv();
    // 设置剔除按钮
    setRejectorButton();




    // 获取相机对象-->camera_object_vector
    getCameraObjectToVector();
    // 打开相机
    openAllCamera();
    //设置相机参数
    setAllCameraParam();

}

// 直接重启: 内部调用
void Main::__DirectlyRestart() {
    __ReadyQuit();
    // 返回重启编号
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

// 持久化参数到配置文件
void Main::saveConfig() {
    writeCameraConfig_FromParamStructVector();
    writeSystemConfig();

}

void Main::updateCsv() {
    // todo
}

// 检查密码的函数, 密码正确返回True
bool Main::confirmPassword(string utype, string info) {

    bool ok;
    QString password, text;
    if (utype == "admin") {
        text = QInputDialog::getText(this, QString::fromStdString(info), "请输入管理员密码~", QLineEdit::Password, "", &ok);
        password = admin_password;
    } else {
        text = QInputDialog::getText(this, QString::fromStdString(info), "请输入用户密码~", QLineEdit::Password, "", &ok);
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

//删除缓存目录
void Main::deleteExceptionDirectory() {
    //todo
}

void Main::allCameraTriggerOnce(int num) {
    //todo
}

// 设置所有相机参数
void Main::setAllCameraParam() {
    for (int i = 0; i < cam_sum; i++) {
        camera_object_vector.at(i).setParameter(camera_config_struct_vector.at(i).frame_rate,
                                                camera_config_struct_vector.at(i).exposure_time,
                                                camera_config_struct_vector.at(i).image_gain);
    }
}

//获取参数列表的对象向量
void Main::getArgsListObjList() {

    QGroupBox *groupBox;
    string name_s;
    char name_c[64];

    for (int i = 0; i < cam_sum; i++) {
        groupBox = new QGroupBox(ui->centralwidget);

        groupBox->setGeometry(QRect(840, 90, 231, 631));
        groupBox->setStyleSheet("background-color: rgb(255, 255, 255);\n");
        groupBox->setAlignment(Qt::AlignCenter);
        groupBox->setObjectName(QStringLiteral("groupBox"));

        name_s = "参数列表" + to_string(1);
        strcpy(name_c, name_s.c_str());

        groupBox->setTitle(QApplication::translate("MainWindow", name_c, Q_NULLPTR));

        // groupBox->raise();

        param_list_vector.push_back(groupBox);
    }
}

// 创建all_spinBox_list_dic参数列表  在参数列表中创建变量
void Main::createArgsListDict() {
    QLabel *label;
    QSpinBox *spinBox;
    vector<ParamsStruct> params_struct_vector;
    // 便利相机
    for (int i = 0; i < cam_sum; i++) {
        //遍历参数
        QGroupBox *groupBox = param_list_vector.at(i);
        params_struct_vector = visual_detection_object_vector[i]->params_vector;

        vector<QSpinBox *> spin_box_vector;

        for (int j = 0; j < params_struct_vector.size(); j++) {
            char name_c[64];
            strcpy(name_c, params_struct_vector[j].name.c_str());

            label = new QLabel(groupBox);
            label->setObjectName(QStringLiteral("label"));
            label->setGeometry(QRect(10, 40 * (j + 1), 120, 30));
            label->setText(QApplication::translate("MainWindow", name_c, Q_NULLPTR));

            spinBox = new QSpinBox(groupBox);
            spinBox->setObjectName(QStringLiteral("spinBox"));
            spinBox->setGeometry(QRect(130, 40 * (j + 1), 90, 30));
            spinBox->setValue(params_struct_vector[j].value);

            spin_box_vector.push_back(spinBox);

            connect(spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=] { changeArgsDict(i); });
        }
        allCamera_spinBoxVector_Vector.push_back(spin_box_vector);
    }

}

// 异常图像存储路径
void Main::createExceptionDirectory() {
    //todo
}

// 监视状态
void Main::monitorSwitch() {

    if (ui->pushButton_08->isChecked()) {
        if (!confirmPassword("user", "修改参数")) {
            ui->pushButton_08->setChecked(false);
            return;
        }
        ui->pushButton_08->setText("调整参数");
        for (int i = 0; i < cam_sum; i++) {
            param_list_vector.at(i)->setEnabled(true);
        }
    } else {
        ui->pushButton_08->setText("监视状态");
        for (int i = 0; i < cam_sum; i++) {
            param_list_vector.at(i)->setEnabled(false);
        }
    }
}

// 初始化所有相机坐标
void Main::updateAllCoordinates() {
    // todo
}

// 启动更新csv线程
void Main::startUpdataCsv() {
    // todo
}

// 设置剔除按钮
void Main::setRejectorButton() {
    if (control_rejector.toInt() == 1) {
        ui->pushButton_07->setText("剔除开");
        ui->pushButton_07->setChecked(true);
        is_Rejector = true;
    } else {
        ui->pushButton_07->setText("剔除关");
        ui->pushButton_07->setChecked(false);
        is_Rejector = false;
    }
}

//获取视觉检测对象向量
void Main::getVisualDetectionObjectVector() {
    SetCameraMap cameramap;
    for (int i = 0; i < cam_sum; i++) {
        visual_detection_object_vector.push_back(cameramap.camera_dict[i]);
    }
}

//改变参数列表
void Main::changeArgsDict(int cam_num) {

    vector<QSpinBox *> spinBoxVector = allCamera_spinBoxVector_Vector[cam_num];

    int *value_p = new int[spinBoxVector.size()];
    for (int i = 0; i < spinBoxVector.size(); i++)
        value_p[i] = spinBoxVector[i]->value();

    visual_detection_object_vector[cam_num]->updateArgs(value_p);
    delete[] value_p;
}


void Main::Window_ExceptionImage() {

}

void Main::errorStop() {

}

void Main::trainingMode() {

    if (ui->pushButton_06->isChecked()) {
        // 权限验证
        if (!confirmPassword("user", "训练模式")) {
            ui->pushButton_06->setChecked(false);
            return;
        }
        // 禁用单选按钮
        disabledRadioButton();
        ui->pushButton_06->setText("保存坐标");
        // 开启捕捉
        ui->label_main->setDrawRect(true);
    } else {
        ui->pushButton_06->setText("训练模式");
        // 关闭捕捉
        ui->label_main->setDrawRect(false);
        // 获取坐标
        ui->label_main->getCoord(coord_array);
        // 调整坐标
        trimCoordinates(coord_array);
        // 存入配置文件
        updateCameraConfig(coord_array);
        // 更新相机的坐标参数
        updateNowCoord(coord_array);
        // 启用单选按钮
        enabledRadioButton();
    }
}

// 剔除开关
void Main::rejectorSwitch() {
    if (ui->pushButton_07->isChecked()) {
        // 权限验证
        if (!confirmPassword("user", "开启剔除")) {
            ui->pushButton_07->setChecked(false);
            return;
        }
        ui->pushButton_07->setText("剔除开");
        is_Rejector = true;
        control_rejector = "1";
    } else {
        if (!confirmPassword("user", "关闭剔除")) {
            ui->pushButton_07->setChecked(false);
            return;
        }
        ui->pushButton_07->setText("剔除关");
        is_Rejector = false;
        control_rejector = "0";
    }
}

// 调整坐标
void Main::trimCoordinates(int *array) {
    if (array[0] < 0) array[0] = 0;
    if (array[1] < 0) array[1] = 0;
    if (array[2] > ui->label_main->width()) array[2] = ui->label_main->width();
    if (array[3] > ui->label_main->height()) array[3] = ui->label_main->height();

}

// 更新相机配置文件
void Main::updateCameraConfig(int *array) {
    camera_config_struct_vector.at(now_show_num).topLeft.setX(array[0]);
    camera_config_struct_vector.at(now_show_num).topLeft.setY(array[1]);
    camera_config_struct_vector.at(now_show_num).bottomRight.setX(array[2]);
    camera_config_struct_vector.at(now_show_num).bottomRight.setY(array[3]);
}

// 更新相机的坐标参数
void Main::updateNowCoord(int *array) {
    float zoom_x = allCamera_zoomRatio_mapVector[now_show_num].zoom_width;
    float zoom_y = allCamera_zoomRatio_mapVector[now_show_num].zoom_height;

    visual_detection_object_vector[now_show_num]->x1 = array[0] * zoom_x;
    visual_detection_object_vector[now_show_num]->y1 = array[1] * zoom_y;
    visual_detection_object_vector[now_show_num]->x2 = array[2] * zoom_x;
    visual_detection_object_vector[now_show_num]->y2 = array[3] * zoom_y;
}

// 获取所有相机的 缩放比结构体 向量
void Main::getAllCameraZoomRatioVector() {
    float m_w = ui->label_main->width(), m_h = ui->label_main->height();

    for (int i = 0; i < cam_sum; i++) {
        float width = camera_config_struct_vector.at(i).width;
        float height = camera_config_struct_vector.at(i).height;
        allCamera_zoomRatio_mapVector.push_back({width / m_w, height / m_h});
    }
}
