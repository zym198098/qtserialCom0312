#include "qtserialCom.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qtserialCom w;
    w.show();
    return a.exec();
}
