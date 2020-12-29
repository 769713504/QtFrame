#include "Main/Main.h"

int showMainWindow(int argc, char *argv[]) {
    QApplication _(argc, argv);
    Main w;
    w.show();
    return QApplication::exec();
}


int main(int argc, char *argv[]) {
    int exit_code;
    while (true) {
        exit_code = showMainWindow(argc, argv);
        if (exit_code == 0) {
            break;
        } else if (exit_code == 6) {
            continue;
        } else {
            printf("\nExit code \"%d\" is not defined!\n", exit_code);
            break;
        }
    }
}

