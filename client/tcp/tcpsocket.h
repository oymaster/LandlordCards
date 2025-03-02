#ifndef TcpSocket_H
#define TcpSocket_H

#include <QObject>

#ifdef Q_OS_WIN
#include <winsock2.h>
#endif

class TcpSocket : public QObject
{
    Q_OBJECT
public:
    TcpSocket(QObject* parent=nullptr);
    //如果在创建对象的时候直接去连服务器 那么在对象被创建出来后就不需要再连接服务器了 所以添加一个带参数的构造函数
    TcpSocket(QByteArray ip,unsigned short port,QObject* parent=nullptr);
    ~TcpSocket();

    //连接服务器
    bool connectToHost(QByteArray ip,unsigned short port);
    //接收数据
    QByteArray recvMsg(int timeout=-1); //单位 s
    //发送数据
    bool sendMsg(QByteArray msg,int timeout=-1);
    //断开连接
    void disConnect();

signals:

private:
    bool readTimeout(int timeout);
    bool writeTimeout(int timeout);
    //在执行读操作时我们需要把内容写到buf指针指向的内存
    int readn(char* buf,int count);
    //在执行写操作时 我们不许要在buf指针指向的内存写数据只需要读就行
    int writen(const char* buf,int count);

#ifdef Q_OS_WIN
    SOCKET m_socket;
#endif

#ifdef Q_OS_LINUX
    int m_socket
#endif


};

#endif // TcpSocket_H
