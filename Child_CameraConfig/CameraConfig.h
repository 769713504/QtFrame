//
// Created by xin on 2020/12/25.
//

#ifndef QTFRAME_CAMERACONFIG_H
#define QTFRAME_CAMERACONFIG_H

#include <QDialog>
#include <Main/Main.h>

QT_BEGIN_NAMESPACE
namespace Ui { class CameraConfig; }
QT_END_NAMESPACE





class CameraConfig : public QDialog {
Q_OBJECT

public:
    explicit CameraConfig(Main *parent = nullptr);

    ~CameraConfig() override;

public slots:

    // 复选
    void disableComboBox();

    // 保存参数
    void saveSet();

    // 取消操作
    void cancelSet();

private:
    Main *main_window;
    Ui::CameraConfig *ui;

    void setComboBox();

    void getNowCamArgs();
};

#endif //QTFRAME_CAMERACONFIG_H
