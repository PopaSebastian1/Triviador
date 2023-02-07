#include "Registration.h"
#include <QtWidgets/QApplication>
#include"QTTimer.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Registration w;
    w.show();
    return a.exec();
}
float timer(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    MyTimer timer;

    return a.exec();
}