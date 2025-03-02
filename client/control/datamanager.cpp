#include "datamanager.h"

//单例懒汉模式，程序启动时就初始化一个该对象
DataManager* DataManager::m_data=new DataManager;

DataManager *DataManager::getInstance()
{
    return m_data;
}

void DataManager::setUserName(QByteArray name)
{
    m_userName=name;
}

void DataManager::setIP(QByteArray ip)
{
    m_ip=ip;
}

void DataManager::setPort(QByteArray port)
{
    m_port=port;
}



QByteArray DataManager::getUserName()
{
    return m_userName;
}


QByteArray DataManager::getIP()
{
    return m_ip;
}

QByteArray DataManager::getPort()
{
    return m_port;
}

