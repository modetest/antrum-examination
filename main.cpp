#include "loginwidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QIcon icon(":/antrum/resouce/favicon32.ico");
    a.setApplicationName(u8"Î¸ñ¼¼à²â");
    a.setWindowIcon(icon);

    loginWidget w;
    w.show();
    return a.exec();
}
