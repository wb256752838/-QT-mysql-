#include "sjk.h"
#include <QApplication>
#include <QFile>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sjk w;
    w.show();
    QFile qssFile(":/MacOS.qss");//1.改变主题
        if(qssFile.open(QFile::ReadOnly)){//2.
            a.setStyleSheet(qssFile.readAll());//3.
        }
        qssFile.close();//4.

    return a.exec();
}
