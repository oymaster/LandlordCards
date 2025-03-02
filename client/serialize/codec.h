#ifndef CODEC_H
#define CODEC_H

#include <QByteArray>
#include "information.pb.h"
#include <QSharedPointer>

//数据和protobuf中的消息体是对应的
//序列化->基于这个结构体数据进行
//反序列化->将解析出来的数据存储到这个结构体中
struct Message{
    QByteArray userName;
    QByteArray roomName;
    QByteArray data1;
    QByteArray data2;
    QByteArray data3;

    RequestCode reqcode;
    RespondCode rescode;
};


class Codec
{
public:
    //序列化
    Codec(Message* msg);
    //反序列化
    Codec(QByteArray msg);
    //数据编码
    QByteArray encodeMsg();
    //数据解码
    QSharedPointer<Message> decodeMsg();
    // 重新加载数据
    void reLoad(QByteArray msg);
    void reLoad(Message* msg);
private:
    QByteArray m_msg;
    Information m_obj;//通过这个对象处理数据的序列化和反序列化
};

#endif // CODEC_H
