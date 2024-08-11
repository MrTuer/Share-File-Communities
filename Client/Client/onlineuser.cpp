#include "onlineuser.h"
#include <ui_onlineuser.h>
#include "client.h"
OnlineUser::OnlineUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OnlineUser)
{
    ui->setupUi(this);
}

OnlineUser::~OnlineUser()
{
    delete ui;
}

void OnlineUser::updateOnlineUserWidgetList(QStringList userList)
{
    ui->onlineUser_LW->clear();
    ui->onlineUser_LW->addItems(userList);

}

void OnlineUser::on_onlineUser_LW_itemDoubleClicked(QListWidgetItem *item)
{
    QString CurName = Client::getinstance().getm_username();
    QString TarName = item->text();

    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REQUST;
    memcpy(pdu->caData,CurName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,TarName.toStdString().c_str(),32);

    qDebug()<<"caCurName:"<<CurName<<endl<<"caTarName :"<<TarName;
    Client::getinstance().sendPDU(pdu);
}




