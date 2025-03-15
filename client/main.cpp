#include "cards.h"
#include "login.h"
#include "aescrypto.h"
#include <QApplication>
#include <QFile>
#include <qdebug.h>
#include "rsacrypto.h"

void test(){
    // 创建 AES 加密对象，使用 CBC 模式，密钥长度为 128 位
    AesCrypto aes(AesCrypto::AES_CBC_128,"1234567887654321");

    // 原始明文
    QString text = "这是AES的测试内容ddddd";

    // 使用 AES 进行加密，需先将 QString 转换为 QByteArray（UTF-8 编码）
    QByteArray AesText = aes.encrypt(text.toUtf8());

    // 输出原始明文
    qDebug()<<"原文: "<<text<<"\n";

    // 输出 AES 加密后的数据
    qDebug()<<"加密后: "<<AesText<<"\n";

    // 使用 AES 进行解密
    AesText = aes.decrypt(AesText);

    // 输出解密后的数据，应该与原文相同
    qDebug()<<"解密后: "<<AesText.data()<<"\n";

    // ----------------------- RSA 加密解密 -----------------------

    // 创建 RSA 对象
    RsaCrypto rsa;

    // 生成 RSA 密钥对，密钥长度为 2048 位（2k bits）
    rsa.generateRsaKey(RsaCrypto::BITS_2k);

    // 使用 RSA 公钥加密，最大可加密数据长度为 bits/8 - 11（11 是填充所需空间）

    // 用公钥初始化一个新的 RSA 对象（从 public.pem 读取公钥）
    RsaCrypto rsa1("public.pem",RsaCrypto::PublicKey);

    // 待加密的字符串
    QByteArray temp = "天长地久有时尽，此恨绵绵无绝期！";

    // 使用 RSA 公钥加密
    temp = rsa1.pubKeyEncrypt(temp);

    // 用私钥初始化另一个 RSA 对象（从 private.pem 读取私钥）
    RsaCrypto rsa2("private.pem",RsaCrypto::PrivateKey);

    // 使用 RSA 私钥解密
    temp = rsa2.priKeyDecrypt(temp);

    // 输出解密后的数据，应该与原始文本一致
    qDebug()<< temp.data();

    // ----------------------- RSA 签名与验签 -----------------------

    // 使用 RSA 私钥对 AES 解密后的数据进行签名
    temp = rsa2.sign(AesText);

    // 使用 RSA 公钥验证签名
    bool flag = rsa1.verify(temp, AesText);

    // 输出验签结果，flag = true 表示签名验证成功（数据未被篡改）
    qDebug()<<"flag = "<<flag;
}


int main(int argc, char *argv[])
{
    // test();
    // 启用高 DPI 缩放
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    // 允许使用小数缩放因子，避免舍入误差
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication a(argc, argv);

    a.setFont(QFont("微软雅黑"));

    qRegisterMetaType<Cards>("Cards&");//自定义类型注册为 Qt 元对象类型
    qRegisterMetaType<Cards>("Cards");


    QFile file(":/conf/style.qss");
    file.open(QFile::ReadOnly);
    QByteArray all = file.readAll();
    a.setStyleSheet(all);
    file.close();

    Login w;
    int ret = w.exec();
    if(ret == QDialog::Accepted)
    {
        // test();
        return a.exec();
    }
}
