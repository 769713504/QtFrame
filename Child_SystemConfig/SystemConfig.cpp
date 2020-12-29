#include <QMessageBox>
#include "SystemConfig.h"
#include "ui_SystemConfig.h"

SystemConfig::SystemConfig(Main *parent) : QDialog(parent), ui(new Ui::SystemConfig) {
    ui->setupUi(this);
    main_window = parent;
    main_window->is_change_param = false;

    // 获取系统配置参数
    getSysConfigArgs();
    setSpinBox_0();

    connect(ui->comboBox_0, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=] { setSpinBox_0(); });
    connect(ui->pushButton_1, SIGNAL(clicked()), SLOT(saveSet()));
    connect(ui->pushButton_2, SIGNAL(clicked()), SLOT(cancelSet()));
    connect(ui->pushButton_3, SIGNAL(clicked()), SLOT(modifyUserPassword()));
    connect(ui->pushButton_4, SIGNAL(clicked()), SLOT(modifyAdminPassword()));
}

SystemConfig::~SystemConfig() { delete ui; }

// 读取系统配置参数
void SystemConfig::getSysConfigArgs() {
    QString run_mode = main_window->run_mode;

    ui->comboBox_0->setCurrentIndex(main_window->run_mode.toInt());
    setPort();
    ui->spinBox_0->setValue(main_window->simulate_camnum.toInt());

    user_password = main_window->user_password;
    user_password = main_window->admin_password;
}

void SystemConfig::setSpinBox_0() {
    if (ui->comboBox_0->currentIndex() == 2) {
        ui->spinBox_0->setEnabled(true);
        ui->comboBox_1->setEnabled(false);
    } else {
        ui->spinBox_0->setEnabled(false);
        ui->comboBox_1->setEditable(true);
    }
}

void SystemConfig::setPort() {
    //todo
}

void SystemConfig::saveSet() {
    setSysConfigArgs();
    main_window->is_change_param = true;
    close();
}

#include "QInputDialog"

void SystemConfig::modifyAdminPassword() {
    bool ok;
    // # 第三个参数表示显示类型，可选，有正常（QLineEdit.Normal）、密碼（ QLineEdit. Password）、不显示（ QLineEdit. NoEcho）三种情况
    QString value = QInputDialog::getText(this, "管理员密码", "请输入1~6位字符", QLineEdit::Password, "6", &ok);
    if (ok) {
        if (value.length() > 6) {
            if (QMessageBox::warning(this, "无效密码!", "密码必须是1~6位字符!\n是否重新设置密码?")) {
                return modifyAdminPassword();
            }
        } else {
            admin_password = value;
        }
    }
}

void SystemConfig::cancelSet() {
    close();
}

void SystemConfig::modifyUserPassword() {
    bool ok;
    // # 第三个参数表示显示类型，可选，有正常（QLineEdit.Normal）、密碼（ QLineEdit. Password）、不显示（ QLineEdit. NoEcho）三种情况
    QString value = QInputDialog::getText(this, "用户密码", "请输入1~6位字符", QLineEdit::Password, "0", &ok);
    if (ok) {
        if (value.length() > 6) {
            if (QMessageBox::warning(this, "无效密码!", "密码必须是1~6位字符!\n是否重新设置密码?")) {
                return modifyAdminPassword();
            }
        } else {
            user_password = value;
        }
    }
}

void SystemConfig::setSysConfigArgs() {

    main_window->admin_password = admin_password;
    main_window->user_password = user_password;
    main_window->run_mode = QString::number(ui->comboBox_0->currentIndex());
    main_window->simulate_camnum = QString::number(ui->spinBox_0->value());
    main_window->control_port = ui->comboBox_1->currentText();
}

