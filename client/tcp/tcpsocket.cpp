#include "tcpsocket.h"
#include <QDebug>

TcpSocket::TcpSocket(QObject *parent)
{
#ifdef Q_OS_WIN
    //加载套接字库
    WSADATA data;
    WSAStartup(MAKEWORD(2,2),&data);
#endif
}

TcpSocket::TcpSocket(QByteArray ip, unsigned short port, QObject *parent) :TcpSocket(parent)
{
    connectToHost(ip,port);
}

TcpSocket::~TcpSocket()
{
#ifdef Q_OS_WIN
    //释放套接字库
    WSACleanup();
#endif
}

bool TcpSocket::connectToHost(QByteArray ip, unsigned short port)
{
    assert(port>0 && port<=65535);
    //初始化套接字
    m_socket=socket(AF_INET,SOCK_STREAM,0);
    assert(m_socket);

    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=inet_addr(ip.data());
    int ret=::connect(m_socket,reinterpret_cast<struct sockaddr*>(&addr),sizeof(addr));
    bool flag=ret==0?true:false;
    return flag;
}

QByteArray TcpSocket::recvMsg(int timeout)
{
    bool flag=readTimeout(timeout);
    QByteArray arry;

    if(flag)
    {
        //发送数据=数据头+数据块
        long headLen=0;
        int ret=readn((char*)&headLen,sizeof(int));
        if(ret==0)
        {
            return QByteArray();
        }

        qDebug()<<"大端接收数据块的长度"<<headLen;
        headLen=ntohl(headLen);//大端转小端
        qDebug()<<"小端接收数据块的长度"<<headLen;

        //申请一块新的内存
        char* data=new char[headLen];
        assert(data);
        ret=readn(data,headLen);
        if(ret==headLen)
        {
            //数据正常
            arry=QByteArray(data,headLen);
        }
        else
        {
            //数据异常
            arry = QByteArray();
        }
        delete[]data;
    }
    return arry;
}

bool TcpSocket::sendMsg(QByteArray msg, int timeout)
{
    bool flag=writeTimeout(timeout);
    if(flag)
    {
        //发送数据=数据头+数据块
        int headLen=htonl(msg.size());//主机字节序转成网络字节序
        //申请一块新的内存
        int length=sizeof(int)+msg.size();
        char* data=new char[length];
        assert(data);
        memcpy(data,&headLen,sizeof(int));
        memcpy(data+sizeof(int),msg.data(),msg.size());

        int ret=writen(data,length);
        flag=ret==length?true:false;
        delete[]data;
    }
}

void TcpSocket::disConnect()
{

    if(m_socket)
    {
#ifdef Q_OS_WIN
        closesocket(m_socket);
#endif
#ifdef Q_OS_LINUX
        close(m_socket);
#endif

    }
}

bool TcpSocket::readTimeout(int timeout)
{
    if(timeout==-1)
    {
        return true;//阻塞读数据（没数据时就阻塞）
    }
    //检测读缓冲区区
#ifdef Q_OS_WIN
    int nfds=0;
#endif
#ifdef Q_OS_LINUX
    int nfds=m_socket+1;
#endif
    fd_set rdset;
    //先清空
    FD_ZERO(&rdset);
    //再放入
    FD_SET(m_socket,&rdset);
    //不用检测写集合和异常集合
    struct timeval tmout;
    tmout.tv_sec = timeout;//秒
    tmout.tv_usec=0;
    int ret=select(nfds,&rdset,NULL,NULL,&tmout);
    bool flag=ret>0?true:false;
    return flag;

}

bool TcpSocket::writeTimeout(int timeout)
{
    if(timeout==-1)
    {
        return true;//阻塞写数据（可写数据时就写）
    }
    //检测写缓冲区区
#ifdef Q_OS_WIN
    int nfds=0;
#endif
#ifdef Q_OS_LINUX
    int nfds=m_socket+1;
#endif
    fd_set wset;
    //先清空
    FD_ZERO(&wset);
    //再放入
    FD_SET(m_socket,&wset);
    //不用检测写集合和异常集合
    struct timeval tmout;
    tmout.tv_sec = timeout;//秒
    tmout.tv_usec=0;
    int ret=select(nfds,NULL,&wset,NULL,&tmout);
    bool flag=ret>0?true:false;
    return flag;
}

int TcpSocket::readn(char *buf, int count)
{
    //通过一个临时变量，来记录下次要读的数量
    int last=count;//剩余的字节数
    int size=0;//每次读出的字节数
    char* pt=buf;
    //有可能一次读不够所以循环多次读
    while(last>0)
    {
        if((size=recv(m_socket,pt,last,0))<0)
        {
            perror("recv");
            return -1;
        }
        //对端关闭连接
        else if(size==0)
        {
            break;
        }
        //每次写入时位置是不同的
        pt+=size;
        last-=size;
    }
    return count-last;
}

int TcpSocket::writen(const char *buf, int count)
{
    //通过一个临时变量，来记录下次要读的数量
    int last=count;//剩余的要写入的字节数
    int size=0;//每次写入的字节数
    const char* pt=buf;
    //有可能一次读不够所以循环多次读
    while(last>0)
    {
        if((size=send(m_socket,pt,last,0))<0)
        {
            perror("send");
            return -1;
        }
        //对端关闭连接
        else if(size==0)
        {
            continue;
        }
        //每次写入时位置是不同的
        pt+=size;
        last-=size;
    }
    return count-last;
}
