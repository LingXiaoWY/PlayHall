#include "maindialog.h"
#include <QApplication>
#include "ckernel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainDialog w;
//    w.show();
    //创建核心类
    CKernel::GetInstance();
    return a.exec();
}
