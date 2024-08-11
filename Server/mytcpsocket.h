#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H
#include<QFile>
#include<QDebug>
#include<QLineEdit>
#include<QWidget>
#include<QTcpSocket>
#include<QObject>
#include<QTcpServer>
#include<QString>
#include<QTcpServer>
#include<QTcpServer>
#include<QMessageBox>
#include <QObject>
#include<QTcpServer>
#include<QWidget>
#include<QTcpSocket>
#include"protocol.h"
#include "msghandler.h"

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MsgHandler *mh;
    MyTcpSocket();
    void recvMsg();
    void clientOffline();
    QString m_StLoginrName;
    QByteArray buffer;
    PDU * readPDU();
    PDU *  handlePDU(PDU * pdu);
    void sendPDU(PDU * pdu);
    ~MyTcpSocket();
signals:
    void offline(MyTcpSocket * socket);
};

#endif // MYTCPSOCKET_H
