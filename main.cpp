#include "gamepanel.h"
#include "loading.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<Cards>("Cards&");//自定义类型注册为 Qt 元对象类型
    qRegisterMetaType<Cards>("Cards");
    Loading w;
    w.show();
    return a.exec();
}
