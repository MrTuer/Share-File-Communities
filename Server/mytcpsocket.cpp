#include "mytcpsocket.h"
#include "protocol.h"
#include "mytcpsocket.h"
#include "operatedb.h"
#include <QMessageBox>
#include "operatedb.h"
MyTcpSocket::MyTcpSocket()
{
    mh = new MsgHandler();
    connect(this,&QTcpSocket::readyRead,this,&MyTcpSocket::recvMsg);
    connect(this,&QTcpSocket::disconnected,this,&MyTcpSocket::clientOffline);
}

void MyTcpSocket::recvMsg()
{
    qDebug()<<"接收信息长度为："<<bytesAvailable();
    QByteArray data = readAll();
    buffer.append(data);
    while(buffer.size() >= int(sizeof(PDU))){//判断是否是一个完整的PDU
        PDU * pdu = (PDU *)buffer.data();
        if(buffer.size()<int(pdu->uiPDULen)){//判断是否够一个完整的协议请求
            break;
        }
        PDU *respdu = handlePDU(pdu);
        sendPDU(respdu);
        buffer.remove(0,pdu->uiPDULen);
    }
}

void MyTcpSocket::clientOffline()
{
    qDebug()<<m_StLoginrName.toStdString().c_str();
    OperateDB::getinstance().handleOffline(m_StLoginrName.toStdString().c_str());
    emit offline(this);
}


void MyTcpSocket::sendPDU(PDU *pdu)
{
    if(pdu==NULL){
        return;
    }
    write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

PDU * MyTcpSocket::handlePDU(PDU* pdu)
{
    qDebug()<<"\n\n\nrecvMsg 信息类型:"<<pdu->uiMsgType
            <<"消息内容:"<<pdu->caMsg
            <<"参数1:"<<pdu->caData
            <<"参数2:"<<pdu->caData+32;
    switch(pdu->uiMsgType){
    case ENUM_MSG_TYPE_REGIST_REQUEST:
            return mh->resign(pdu);
            break;
    case ENUM_MSG_TYPE_LOGIN_REQUEST:
            return mh->login(pdu,m_StLoginrName);
            break;
    case ENUM_MSG_TYPE_USER_SELECT_REQUST:
            return mh->user_select(pdu);
            break;
    case ENUM_MSG_TYPE_ONLINE_USER_REQUST:
            return mh->online_users();
            break;
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUST:
            return mh->add_friend(pdu);
            break;
    case ENUM_MSG_TYPE_ACCEPT_FRIEND_REQUST:
            return mh->accept_friend(pdu);
            break;
    case ENUM_MSG_TYPE_FLUSH_FRIEND_REQUST:
        return mh->flush_online_friend(pdu);
        break;
    case ENUM_MSG_TYPE_DELETE_FRIEND_REQUST:
        return mh->delete_friend(pdu);
        break;
    case ENUM_MSG_TYPE_CHAT_FRIEND_REQUEST:
        mh->chat(pdu);
        break;
    case ENUM_MSG_TYPE_CREATE_MKDIR_REQUEST:
        return mh->mkdir(pdu);
        break;
    case ENUM_MSG_TYPE_FLUSH_FILE_REQUEST:
        return mh->flushFile(pdu);
        break;
    case ENUM_MSG_TYPE_DELETE_MKDIR_REQUEST:
        return mh->delMkdir(pdu);
        break;
    case ENUM_MSG_TYPE_DELETE_FILE_REQUEST:
        return mh->delFile(pdu);
        break;
    case ENUM_MSG_TYPE_RENAME_FILE_REQUEST:
        return mh->RenameFile(pdu);
        break;
    case ENUM_MSG_TYPE_MOVE_FILE_REQUEST:
        return mh->MoveFile(pdu);
        break;
    case ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST:
        return mh->uploadFile(pdu);
        break;
    case ENUM_MSG_TYPE_UPLOAD_FILE_DATA_REQUEST:
        return mh->uploadFileData(pdu);
        break;
    case ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST:
        return mh->downloadFile(pdu);
        break;
    case ENUM_MSG_TYPE_DOWNLOAD_FILE_DATA_REQUEST:
        return mh->downloadFileData(pdu);
        break;
    case ENUM_MSG_TYPE_SHARE_FILE_REQUEST:
        return mh->shareFile(pdu);
        break;
    case ENUM_MSG_TYPE_SHARE_FILE_AGREE_REQUEST:
        return mh->shareFileAgree(pdu);
        break;
    default :
        break;
    }
    return NULL;
}

MyTcpSocket::~MyTcpSocket()
{
    delete mh;
}
