#include "Cpp_Main/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication _(argc, argv);
    MainWindow w;
    w.show();
    QApplication::exec();
}

