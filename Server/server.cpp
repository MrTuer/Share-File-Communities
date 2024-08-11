#include "server.h"
#include "ui_server.h"
#include<QDebug>
#include<QDebug>
#include<QObject>
#include<QFile>
#include<QDebug>
#include<QHostAddress>
#include"mytcpserver.h"

Server::Server(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Server)
{
    ui->setupUi(this);

    loadConfig();
    //connect(&m_tcpSocket,&QTcpSocket::connected,this,&Server::connectHostSucessed);
    qDebug()<<"打开服务端配置文件 ip:"<< m_strIp <<"port:"<<m_usPort;
    MyTcpServer::getinstance().listen(QHostAddress(m_strIp),m_usPort);
    qDebug()<<"打开服务端配置文件 ip:"<< m_strIp <<"port:"<<m_usPort;
}

Server &Server::getinstance()
{
    static Server instance;
    return instance;
}

Server::~Server()
{
    delete ui;
}

void Server::loadConfig()
{
    QFile file(":/client.config");
    if(file.open(QIODevice::ReadOnly)){
        QByteArray baData = file.readAll();
        QString strData = QString(baData);
        QStringList strList = strData.split("\r\n");
        m_strIp = strList.at(0);
        m_usPort = strList.at(1).toUShort();
        m_strRootPath = strList.at(2);
        qDebug()<<"打开服务端配置文件 ip:"<< m_strIp <<"port:"<<m_usPort<<"Rootpath:"<<m_strRootPath;
        file.close();
    } else {
         qDebug()<<"打开配置失败";
    }
}
void Server::connectHostSucessed()
{
    qDebug()<<"连接服务器成功";
}

QString &Server::get_RootPath()
{
    return m_strRootPath;
}




