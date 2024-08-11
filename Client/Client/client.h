#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include<QFile>
#include<QDebug>
#include<QStringList>
#include<QHostAddress>
#include<QTcpSocket>
#include<QHostAddress>
#include"protocol.h"
#include"reshandler.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QWidget
{
    Q_OBJECT

public slots:
    void connectHostSucessed();
    QTcpSocket &getTcpSocket();

public:
    MsgHandler * mh;
    void   handlePDU(PDU * pdu);

    void recvMsg();
    void sendPDU(PDU * pdu);
    QString &getm_username();
    QString getm_strRootPath();

    QByteArray buffer;

    static Client &getinstance();
    ~Client();

    //加载配置文件
    void loadConfig();
    QTcpSocket & getSocket();
    QString m_username;

private slots:


    void on_login_PB_clicked();

    void on_regist_PB_clicked();

private:
    //实现单例模式
    //1.私有化构造函数，删除拷贝构造和拷贝赋值运算符。

    Client(QWidget *parent = nullptr);
    Client(const Client & other) = delete ;
    Client &operator=(const Client & other) = delete;
    Ui::Client *ui;
    QString m_strIp ;
    qint16 m_usPort ;
    QTcpSocket m_tcpSocket;
    QString m_strRootPath;
};
#endif // CLIENT_H
