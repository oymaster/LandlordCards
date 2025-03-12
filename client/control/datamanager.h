#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QByteArray>
#include "cards.h"
#include "communication.h"

//用饿汉模式-
//懒汉模式线程安全问题
class DataManager
{
public:
    enum GameMode{Single, Network};
    enum RoomMode{Auto, Manual};
    //禁止使用拷贝构造函数
    DataManager(const DataManager&)=delete;
    //禁止赋值运算
    DataManager& operator=(const DataManager&)=delete;

    static DataManager* getInstance();

    //设置数据
    void setUserName(QByteArray name);
    void setIP(QByteArray ip);
    void setPort(QByteArray port);
    void setCommunication(Communication* comm);
    void setRoomName(QByteArray roomName);
    void setCards(Cards cs,Cards last);
    void setGameMode(GameMode mode);
    void setRoomMode(RoomMode mode);

    //获取数据
    QByteArray getUserName();
    QByteArray getRoomName();
    QByteArray getIP();
    QByteArray getPort();
    Communication* getCommunication();
    Cards getCards();
    Cards getLast3Cards();
    bool isNetworkMode();
    bool isManualMode();

private:
    DataManager()=default;
    static DataManager* m_data;
    QByteArray m_userName;
    QByteArray m_roomName;
    QByteArray m_ip="47.121.216.239";
    QByteArray m_port="8888";
    Communication* m_commun;//不能让他拷贝 所以必须用指针
    Cards m_cs;
    Cards m_last;
    GameMode m_mode;
    RoomMode m_roomMode;
};

#endif // DATAMANAGER_H
