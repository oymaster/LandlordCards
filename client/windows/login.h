#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    //校验数据函数
    bool verifyData(QLineEdit* edit);
    //连接服务器
    // void startConnect(Message* msg);
    //槽函数
    void onLogin();
    void onRegister();
    void onNetOK();
    //保存数据
    void saveUserInfo();
    //加载数据
    void loadUserInfo();
    ~Login();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
