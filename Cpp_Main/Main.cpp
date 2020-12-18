#include "Main.h"


Main::Main(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    getSystemParameter();
    getCameraConfigInfo();


}

Main::~Main() {
    delete ui;
}

// 读取系统参数
void Main::getSystemParameter() {

};

//读取相机配置
void Main::getCameraConfigInfo() {

}

//定义变量
void Main::defineVariable() {

};

//初始化变量
void Main::initializeVariable() {

};

//初始化设置
void Main::initializeSetting() {


};






