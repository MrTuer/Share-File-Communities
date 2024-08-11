#include "sharedfile.h"
#include "ui_sharedfile.h"
#include "index.h"
#include "client.h"

SharedFile::SharedFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SharedFile)
{
    ui->setupUi(this);
}

SharedFile::~SharedFile()
{
    delete ui;
}

void SharedFile::updateFriend_LW()
{
    ui->listWidget->clear();
    QListWidget * friend_LW = Index::getinstence().getFriend()->getFriendListWidget();
    for(int i=0;i<friend_LW->count();i++){
        QListWidgetItem *friendItem = friend_LW->item(i);
        qDebug()<<"friendItem:"<<friendItem->text();
        QListWidgetItem *newItem = new QListWidgetItem(*friendItem);
        ui->listWidget->addItem(newItem);
    }
}



void SharedFile::on_selectall_PB_clicked()
{
    for(int i=0;i<ui->listWidget->count();i++){

        ui->listWidget->item(i)->setSelected(true);
    }
}

void SharedFile::on_concel_PB_clicked()
{
    for(int i=0;i<ui->listWidget->count();i++){
        ui->listWidget->item(i)->setSelected(false);
    }
}

void SharedFile::on_sure_PB_clicked()
{   //获取或当前用户名 路径
    QString strCurPath = Index::getinstence().getFile()->m_strCurPath;
    QString strCurUser = Client::getinstance().getm_username();
    QString strPath = strCurPath +"/"+Index::getinstence().getFile()->m_strSharedFile;
    //获取被选中的好友
    QList<QListWidgetItem * >pItem = ui->listWidget->selectedItems();
    int friendNum = pItem.size();
    qDebug()<<"strCurPath:"<<strCurPath;
    qDebug()<<"strCurUser:"<<strCurUser;
    qDebug()<<"strPath:"<<strPath;

    //封装pdu
    PDU *pdu = mkPDU(friendNum*32+strPath.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_REQUEST;
    memcpy(pdu->caData,strCurUser.toStdString().c_str(),32);
    memcpy(pdu->caData+32,&friendNum,sizeof (int));

    qDebug()<<"pdu->caData:"<<pdu->caData;
    qDebug()<<"pdu->caData+32:"<<pdu->caData+32;
    //复制caMsg
    for(int i=0;i<friendNum;i++){
        memcpy(pdu->caMsg+i*32,pItem.at(i)->text().toStdString().c_str(),32);
        qDebug()<<"pdu->caMsg+i*32:"<<pdu->caMsg+i*32;
    }
    memcpy(pdu->caMsg+friendNum*32,strPath.toStdString().c_str(),strPath.toStdString().size());
    Client::getinstance().sendPDU(pdu);

}
