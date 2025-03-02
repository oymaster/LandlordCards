#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QByteArray>


//用饿汉模式-
//懒汉模式线程安全问题
class DataManager
{
public:
    //禁止使用拷贝构造函数
    DataManager(const DataManager&)=delete;
    //禁止赋值运算
    DataManager& operator=(const DataManager&)=delete;

    static DataManager* getInstance();

    //设置数据
    void setUserName(QByteArray name);
    void setIP(QByteArray ip);
    void setPort(QByteArray port);

    //获取数据
    QByteArray getUserName();
    QByteArray getRoomName();
    QByteArray getIP();
    QByteArray getPort();

private:
    DataManager()=default;
    static DataManager* m_data;
    QByteArray m_userName;
    // QByteArray m_roomName;
    QByteArray m_ip="47.121.216.239";
    QByteArray m_port="9999";
};

#endif // DATAMANAGER_H
