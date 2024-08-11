#include "friend.h"
#include "ui_friend.h"
#include<QInputDialog>
#include<QDebug>
#include<QString>
#include<QMessageBox>
#include<QTcpServer>
#include<QString>
#include"protocol.h"
#include"client.h"
#include<QString>
#include<QTcpSocket>
#include "onlineuser.h"
#include <ui_onlineuser.h>
Friend::Friend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Friend)
{
    ui->setupUi(this);
    flushFriend();
    m_pOnlineUser = new OnlineUser;
    m_pChat = new Chat;
}

OnlineUser* Friend::getOnlineUser()
{
    return m_pOnlineUser;
}

Chat *Friend::getChat()
{
    return m_pChat;
}



Friend::~Friend()
{

    delete ui;
    delete m_pOnlineUser;
    delete m_pChat;
}

void Friend::flushFriend()
{
    PDU * pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FRIEND_REQUST;
    memcpy(pdu->caData,Client::getinstance().getm_username().toStdString().c_str(),32);
    Client::getinstance().sendPDU(pdu);
}

void Friend::updateFriendLW(QStringList friendList)
{
    ui->onlineFriend_LW->clear();
    ui->onlineFriend_LW->addItems(friendList);
}

QListWidget *Friend::getFriendListWidget()
{
    flushFriend();
    return ui->onlineFriend_LW;
}

void Friend::on_findUser_PB_clicked()
{
    QString name = QInputDialog::getText(this,"搜索","用户名");
    qDebug()<<"select name :"<<name;
    if(name.isEmpty()){
        return;
    }
    PDU * pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_USER_SELECT_REQUST;
    memcpy(pdu->caData,name.toStdString().c_str(),32);
    Client::getinstance().getSocket().write((char *)pdu,pdu->uiPDULen);

    free(pdu);
    pdu = NULL;
}

void Friend::on_onlineUser_PB_clicked()
{

    if(m_pOnlineUser->isHidden()){
        m_pOnlineUser->show();
    }
    PDU *pdu = mkPDU(0);
    pdu->uiMsgType =ENUM_MSG_TYPE_ONLINE_USER_REQUST;
    Client::getinstance().getSocket().write((char *)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

void Friend::on_flushFriend_PB_clicked()
{
    flushFriend();
}

void Friend::on_delFriend_PB_clicked()
{
    QListWidgetItem *pItem = ui->onlineFriend_LW->currentItem();
    if(!pItem){
        QMessageBox::information(this,"删除好友","请选择要删除的好友");
        return;
    }
    QString strTarName = pItem->text();
    int ret = QMessageBox::question(this,"删除好友",QString("是否要删除好友'%1'").arg(strTarName));
    qDebug()<<"strTarName:"<<strTarName
            <<endl
            <<"strCurName :"<<Client::getinstance().getm_username().toStdString().c_str()<<endl
           <<"ret :"<<ret;
    if(ret != QMessageBox::Yes){
        return;
    }
    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_REQUST;
    memcpy(pdu->caData,Client::getinstance().getm_username().toStdString().c_str(),32);
    memcpy(pdu->caData+32,strTarName.toStdString().c_str(),32);
    Client::getinstance().sendPDU(pdu);
}

void Friend::on_chat_PB_clicked()
{
    QListWidgetItem *pItem = ui->onlineFriend_LW->currentItem();
    if(!pItem){
        QMessageBox::information(this,"聊天","请选择要聊天的好友");
        return;
    }
    m_pChat-> strChatName = pItem->text();
    if(m_pChat->isHidden()){
        m_pChat->show();
    }
}
