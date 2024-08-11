#include "client.h"
#include "ui_client.h"
#include<QFile>
#include<QDebug>
#include<QLineEdit>
#include<QWidget>
#include<QTcpSocket>
#include<QObject>
#include<QTcpServer>
#include<QString>
#include<QFile>
#include<QDebug>
#include<QMessageBox>
#include "protocol.h"
#include "index.h"


Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    mh = new MsgHandler();
    ui->setupUi(this);
    loadConfig();
    connect(&m_tcpSocket,&QTcpSocket::connected,this,&Client::connectHostSucessed);
    connect(&m_tcpSocket,&QTcpSocket::readyRead,this,&Client::recvMsg);
    m_tcpSocket.connectToHost(QHostAddress(m_strIp),m_usPort);


}

void Client::recvMsg()
{
    qDebug()<<"接收信息长度为："<<Client::getinstance().getSocket().bytesAvailable();
    QByteArray data = Client::getinstance().getSocket().readAll();
    buffer.append(data);
    while(buffer.size() >= int(sizeof(PDU))){//判断是否是一个完整的PDU
        PDU * pdu = (PDU *)buffer.data();
        if(buffer.size()<int(pdu->uiPDULen)){//判断是否够一个完整的协议请求
            break;
        }
        handlePDU(pdu);
        buffer.remove(0,pdu->uiPDULen);
    }
}

void Client::sendPDU(PDU *pdu)
{
    if(pdu==NULL){
        return;
    }
    m_tcpSocket.write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

QString &Client::getm_username()
{
    return m_username;
}

QString Client::getm_strRootPath()
{
    return m_strRootPath;
}



Client::~Client()
{
    delete mh;
    delete ui;
}

void Client::loadConfig()
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

QTcpSocket & Client::getSocket()
{
    return m_tcpSocket;
}



void Client::connectHostSucessed()
{
    QMessageBox::information(this,"连接服务器","服务器连接成功");
}

QTcpSocket &Client::getTcpSocket()
{
    return m_tcpSocket;
}



void Client::handlePDU(PDU *pdu)
{
    qDebug()<<"\n\n\nrecvMsg 信息类型:"<<pdu->uiMsgType
            <<"消息内容:"<<pdu->caMsg
            <<"参数1:"<<pdu->caData
            <<"参数2:"<<pdu->caData+32;
    switch(pdu->uiMsgType){
    case ENUM_MSG_TYPE_REGIST_RESPEND:
    {
        mh->resign(pdu);
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_RESPEND:
    {
        mh->login(pdu);
         break;
    }
    case ENUM_MSG_TYPE_USER_SELECT_RESPEND:
    {
        mh->user_select(pdu);
         break;
    }
    case ENUM_MSG_TYPE_ONLINE_USER_RESPEND:
    {
        mh->online_users(pdu);

        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_RESPEND:
    {
        mh->add_friend(pdu);

        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUST:
    {
        mh->accept_friend(pdu);

        break;
    }
    case ENUM_MSG_TYPE_ACCEPT_FRIEND_RESPEND:
    {
        mh->accept_friend_res(pdu);

        break;
    }
    case ENUM_MSG_TYPE_FLUSH_FRIEND_RESPEND:
    {
        mh->flush_friend(pdu);

        break;
    }
    case ENUM_MSG_TYPE_DELETE_FRIEND_RESPEND:
    {
        mh->delete_friend_res(pdu);

        break;
    }
    case ENUM_MSG_TYPE_CHAT_FRIEND_REQUEST:
    {
        mh->chat(pdu);
        break;
    }
    case ENUM_MSG_TYPE_CREATE_MKDIR_RESPEND:
    {
        mh->mkdir(pdu);
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_FILE_RESPEND:
    {
        mh->flush_file(pdu);
        break;
    }
    case ENUM_MSG_TYPE_DELETE_MKDIR_RESPEND:
    {
        mh->delDir(pdu);
        break;
    }
    case ENUM_MSG_TYPE_DELETE_FILE_RESPEND:
    {
        mh->delDir(pdu);
        break;
    }
    case ENUM_MSG_TYPE_RENAME_FILE_RESPEND:
    {
        mh->Rename_File(pdu);
        break;
    }
    case ENUM_MSG_TYPE_MOVE_FILE_RESPEND:
    {
        mh->Move_File(pdu);
        break;
    }
    case ENUM_MSG_TYPE_UPLOAD_FILE_RESPEND:
    {
        mh->Upload_File(pdu);
        break;
    }
    case ENUM_MSG_TYPE_UPLOAD_FILE_DATA_RESPEND:
    {
        mh->UploadData_File(pdu);
        break;
    }
    case ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST:
    {
        mh->download_File(pdu);
        break;
    }
    case ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPEND:
    {
        mh->download_File_Data_Agree(pdu);
        break;
    }
    case ENUM_MSG_TYPE_DOWNLOAD_FILE_DATA_RESPEND:
    {
        mh->download_File_Data(pdu);
        break;
    }

    case ENUM_MSG_TYPE_SHARE_FILE_RESPEND:
    {
        mh->share_File(pdu);
        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_REQUEST:
    {
        mh->share_File_Request(pdu);
        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_AGREE_RESPEND:
    {
        mh->share_File_AGREE(pdu);
        break;
    }

    default :
        break;
    }
}



Client &Client::getinstance()
{
    static Client instance;
    return instance;
}




void Client::on_login_PB_clicked()
{
    //获取用户名和密码
    QString strName = ui->user_ID->text();
    QString strPwd = ui->password->text();
    if(strName.isEmpty()||strPwd.isEmpty()){
        QMessageBox::critical(this,"登录","账号密码格式不合法");
    }
    //构建pdu
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_REQUEST;
    memcpy(pdu->caData,strName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,strPwd.toStdString().c_str(),32);


    m_tcpSocket.write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=NULL;
}

void Client::on_regist_PB_clicked()
{
        //获取用户名和密码
        QString strName = ui->user_ID->text();
        QString strPwd = ui->password->text();
        if(strName.isEmpty()||strPwd.isEmpty()){
            QMessageBox::critical(this,"注册","账号密码格式不合法");
        }
        //构建pdu
        PDU* pdu = mkPDU(0);

        pdu->uiMsgType = ENUM_MSG_TYPE_REGIST_REQUEST;
        memcpy(pdu->caData,strName.toStdString().c_str(),32);
        memcpy(pdu->caData+32,strPwd.toStdString().c_str(),32);

        m_tcpSocket.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
}

