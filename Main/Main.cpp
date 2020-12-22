#include   "Main.h"

// ���캯��
Main::Main(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    //����GUI
    ui->setupUi(this);
    //ö���豸: ��ȡ�������(���豸��Ϣ��)
    cam_sum = getCameraSum();
    //���豸
    res = openAllCamera();
}

// ����
Main::~Main() {
    delete ui;
}

// ��ȡ�������
int Main::getCameraSum() {
    if (0 == CMvCamera::EnumDevices(IN MV_GIGE_DEVICE | MV_USB_DEVICE, &device_info)) {
        return device_info.nDeviceNum;
    }
    return 0;
}

int Main::openAllCamera() {
    for (int cam_num = 0; cam_num < cam_sum; cam_num++) {
        //�����������
        CMvCamera camera_obj(&device_info, cam_num);
        camera_obj_vector.push_back(camera_obj);
    }


    for (int i = 0; i < cam_sum; i++) {
        cout << "�����" << i << endl;
        //        camera_obj_vector.at(i).Open();
    }


    return 0;
}

bool openAllCamera() {


}

// ��ȡϵͳ����
void Main::getSystemParameter() {

}

//��ȡ�������
void Main::getCameraConfigInfo() {

}

//�������
void Main::defineVariable() {

}

//��ʼ������
void Main::initializeVariable() {

}

//��ʼ������
void Main::initializeSetting() {


}







