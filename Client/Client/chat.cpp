#include "chat.h"
#include "ui_chat.h"
#include "protocol.h"
#include "client.h"
#include "ui_chat.h"
Chat::Chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
}

Chat::~Chat()
{
    delete ui;
}

void Chat::updateShow_LE(QString strMsg)
{
    ui->show_TE->append(strMsg);
}

void Chat::on_send_PB_clicked()
{
    //获取输入框的内容；
    QString strMsg = ui->input_LE->text();
    if(strMsg.isEmpty()){
        return;
    }
    PDU *pdu = mkPDU(strMsg.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_CHAT_FRIEND_REQUEST;
    //通信双方名称
    memcpy(pdu->caData,strChatName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,Client::getinstance().getm_username().toStdString().c_str(),32);
    //通信信息
    memcpy(pdu->caMsg,strMsg.toStdString().c_str(),strMsg.toStdString().size());
    Client::getinstance().sendPDU(pdu);
    ui->input_LE->clear();
}
