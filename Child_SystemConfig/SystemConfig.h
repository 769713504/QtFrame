#ifndef QTFRAME_SYSTEMCONFIG_H
#define QTFRAME_SYSTEMCONFIG_H

#include <QDialog>
#include <Main/Main.h>

QT_BEGIN_NAMESPACE
namespace Ui { class SystemConfig; }
QT_END_NAMESPACE

class SystemConfig : public QDialog {
Q_OBJECT

public:
    explicit SystemConfig(Main *parent = nullptr);

    ~SystemConfig() override;

public slots:

    void setSpinBox_0();

    void cancelSet();

    void saveSet();

    void modifyUserPassword();

    void modifyAdminPassword();

private :
    Main *main_window;

    QString user_password;
    QString admin_password;


    Ui::SystemConfig *ui;

    void getSysConfigArgs();

    void setSysConfigArgs();

    void setPort();


};

#endif //QTFRAME_SYSTEMCONFIG_H
