#include "cards.h"
#include "login.h"
#include "aescrypto.h"
#include <QApplication>
#include <qdebug.h>
#include "rsacrypto.h"

void test(){
    AesCrypto aes(AesCrypto::AES_CBC_128,"1234567887654321");
    QString text = "这是AES的测试内容ddddd";
    QByteArray AesText = aes.encrypt(text.toUtf8());
    qDebug()<<"原文: "<<text<<"\n";
    qDebug()<<"加密后: "<<AesText<<"\n";
    AesText = aes.decrypt(AesText);
    qDebug()<<"解密后: "<<AesText.data()<<"\n";

    RsaCrypto rsa;
    rsa.generateRsaKey(RsaCrypto::BITS_2k);
    //可加密的最大数据长度bits/8-11;11是填充
    RsaCrypto rsa1("public.pem",RsaCrypto::PublicKey);
    QByteArray temp = "天长地久有时尽，此恨绵绵无绝期！";
    temp = rsa1.pubKeyEncrypt(temp);

    RsaCrypto rsa2("private.pem",RsaCrypto::PrivateKey);
    temp = rsa2.priKeyDecrypt(temp);
    qDebug()<< temp.data();
    temp =rsa2.sign(AesText);
    bool flag=rsa1.verify(temp,AesText);
    qDebug()<<"flag = "<<flag;
}

int main(int argc, char *argv[])
{
    test();
    QApplication a(argc, argv);
    qRegisterMetaType<Cards>("Cards&");//自定义类型注册为 Qt 元对象类型
    qRegisterMetaType<Cards>("Cards");
    Login w;
    w.show();
    return a.exec();
}
