#include "gamepanel.h"
#include "login.h"
#include "ui_login.h"

#include <QRegularExpressionValidator>
#include <QThreadPool>
#include <datamanager.h>

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->homeBtn,&QPushButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->netSetBtn,&QPushButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(2);
    });
    connect(ui->regUser,&QPushButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(1);
    });

    // 创建一个正则表达式，用于验证用户名
    QRegularExpression expreg("^[a-zA-Z0-9_]{3,16}$");

    // 需要将验证器保存在堆内存中，不能是局部变量，否则它在函数结束时会被销毁。
    // 为了确保验证器在整个窗口生命周期内有效，我们将它动态分配到堆内存。
    QRegularExpressionValidator *val1 = new QRegularExpressionValidator(expreg, this);  // 创建一个正则表达式验证器

    // 设置验证器给用户名输入框，使用父类指针指向子类对象，实现多态
    // 登陆时的用户名输入框
    ui->userName->setValidator(val1);
    // 注册时的用户名输入框
    ui->regUserName->setValidator(val1);

    // 现在修改正则表达式来验证密码的格式
    expreg.setPattern("^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?=.*[@$!%*?&.])[A-Za-z\\d@$!%*?&.]{4,20}$");
    // 密码的正则表达式：必须包含大写字母、小写字母、数字、特殊字符，长度4-20字符

    // 创建一个新的验证器，将正则表达式传递给它
    QRegularExpressionValidator *val2 = new QRegularExpressionValidator(expreg, this);
    // 设置验证器给密码输入框
    ui->passwd->setValidator(val2);
    // 设置验证器给注册时密码输入框
    ui->regPassword->setValidator(val2);

    // 现在设置手机号的正则表达式
    expreg.setPattern("^1[3456759]\\d{9}$");
    // 手机号的正则：必须是以1开头，第二位数字可以是3、4、5、6、7、8、9之一，总长度为11位

    // 创建一个新的验证器并设置给手机号输入框
    QRegularExpressionValidator *val3 = new QRegularExpressionValidator(expreg, this);
    ui->phone->setValidator(val3);

    // 设置IP地址的正则表达式
    expreg.setPattern("^((\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])\\.){3}(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])$");
    // IP地址的正则：四段数字，每段数字在0-255之间

    // 创建一个新的验证器并设置给IP地址输入框
    QRegularExpressionValidator *val4 = new QRegularExpressionValidator(expreg, this);
    ui->ipAddr->setValidator(val4);

    // 设置端口号的正则表达式
    expreg.setPattern("^(0|([1-9]\\d{0,3})|([1-5]\\d{4})|(6[0-4]\\d{3})|(65[0-4]\\d{2})|(655[0-2]\\d)|(6553[0-5]))$");
    // 端口号的正则：0到65535之间的数字

    // 创建一个新的验证器并设置给端口号输入框
    QRegularExpressionValidator *val5 = new QRegularExpressionValidator(expreg, this);
    ui->port->setValidator(val5);

    //处理按钮点击事件
    connect(ui->loginBtn,&QPushButton::clicked,this,&Login::onLogin);
    connect(ui->registerBtn,&QPushButton::clicked,this,&Login::onRegister);
    connect(ui->netOkBtn,&QPushButton::clicked,this,&Login::onNetOK);

    //设置线程池最大线程数量
    QThreadPool::globalInstance()->setMaxThreadCount(8);

}


bool Login::verifyData(QLineEdit *edit)
{
    // 如果输入框中的数据无效（不符合设定的输入规则）
    if(edit->hasAcceptableInput() == false)
    {
        // 设置输入框的边框为红色，表示输入无效
        edit->setStyleSheet("border: 2px solid red;");
        return false; // 返回 false，表示数据无效
    }
    else
    {
        // 如果输入有效，重置输入框样式
        edit->setStyleSheet("none");
    }
    return true; // 返回 true，表示数据有效
}


void Login::onLogin()
{
    bool flag1=verifyData(ui->userName);
    bool flag2=verifyData(ui->passwd);
    if(flag1&&flag2)
    {
        GamePanel *panel = new GamePanel;
        panel->show();
        // Message msg;
        // msg.userName=ui->userName->text().toUtf8();
        // msg.reqcode=RequestCode::UserLogin;
        // QByteArray passwd=ui->password->text().toUtf8();
        // //对密码进行加密
        // passwd=QCryptographicHash::hash(passwd,QCryptographicHash::Sha224).toHex();
        // msg.data1=passwd;
        // //连接服务器
        // startConnect(&msg);
    }
}

void Login::onRegister()
{
    bool flag1=verifyData(ui->regUserName);
    bool flag2=verifyData(ui->regPassword);
    bool flag3=verifyData(ui->phone);
    if(flag1 && flag2 && flag3)
    {
        // Message msg;
        // msg.userName=ui->regUserName->text().toUtf8();
        // msg.reqcode=RequestCode::UserLogin;
        // QByteArray passwd=ui->regPassword->text().toUtf8();
        // //对密码进行加密
        // passwd=QCryptographicHash::hash(passwd,QCryptographicHash::Sha224).toHex();
        // msg.data1=passwd;
        // msg.data2=ui->phone->text().toUtf8();
        // //连接服务器
        // startConnect(&msg);
    }
}

void Login::onNetOK()
{
    bool flag1=verifyData(ui->ipAddr);
    bool flag2=verifyData(ui->port);
    if(flag1&&flag2)
    {
        //得到一个实例对象
        DataManager* instance=DataManager::getInstance();
        //载入datamanager数据类中
        instance->setIP(ui->ipAddr->text().toUtf8());//从Qstring->QByteArray
        instance->setPort(ui->port->text().toUtf8());
    }
}


Login::~Login()
{
    delete ui;
}
