#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QList>
#include <QTcpServer>
#include <QTcpServer>
#include <QTcpServer>
#include <QTcpServer>
#include "mytcpsocket.h"

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    static MyTcpServer&getinstance();
    void incomingConnection(qintptr handle) override;//连接成功后自动调用
    void resent(char *TarName,PDU *pdu);

private:
    QList<MyTcpSocket*>m_tcpSocketList;

    MyTcpServer();
    MyTcpServer(const MyTcpServer& other) = delete ;
    MyTcpServer &operator=(const MyTcpServer & other) = delete;
public slots:
    void deleteSocket(MyTcpSocket * socket);
};

#endif // MYTCPSERVER_H
