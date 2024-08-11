#include "mytcpserver.h"
#include "mytcpserver.h"
#include "mytcpserver.h"


void MyTcpServer::incomingConnection(qintptr handle)
{
    qDebug()<<"新客户端连接";
    //将连接的客户端存入socket列表.
    MyTcpSocket * pTcpSocket = new MyTcpSocket();
    pTcpSocket->setSocketDescriptor(handle);//socket描述符
    m_tcpSocketList.append(pTcpSocket);
    connect(pTcpSocket,&MyTcpSocket::offline,this,&MyTcpServer::deleteSocket);
}

void MyTcpServer::resent(char *TarName, PDU *pdu)
{
    if(TarName == NULL || pdu == NULL){
        return;
    }
    for(int i=0;i<m_tcpSocketList.size();i++){
        if(m_tcpSocketList.at(i)->m_StLoginrName == TarName){
            m_tcpSocketList.at(i)->write((char *)pdu,pdu->uiPDULen);
            break;
        }
    }

}



MyTcpServer::MyTcpServer()
{

}

void MyTcpServer::deleteSocket(MyTcpSocket *socket)
{
    m_tcpSocketList.removeOne(socket);
    socket->deleteLater();
    socket =NULL;
    qDebug()<<m_tcpSocketList.size();
    for(int i=0;i<m_tcpSocketList.size();i++){
        qDebug()<<m_tcpSocketList.at(i)->m_StLoginrName;
    }
}
MyTcpServer &MyTcpServer::getinstance()
{
    static MyTcpServer instance;
    return instance;
}
