#include "index.h"
#include "reshandler.h"
#include"client.h"
#include "client.h"
#include "index.h"
#include <QMessageBox>
#include<QFile>
#include<QDebug>
#include<QLineEdit>
#include<QWidget>
#include<QTcpSocket>
#include<QObject>
#include<QTcpServer>
#include<QString>
#include <QMessageBox>
#include"chat.h"
#include<QDebug>
#include <QMessageBox>
#include<QMessageBox>

MsgHandler::MsgHandler()
{

}

void MsgHandler::resign(PDU *pdu)
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
      QMessageBox::information(&Client::getinstance(),"注册","注册成功");
    }else{
      QMessageBox::information(&Client::getinstance(),"注册","用户名已存在");
    }
}

void MsgHandler::login(PDU *pdu)
{
    bool ret;
    char caName[32] = {'\0'};
    memcpy(&ret,pdu->caData,sizeof(bool));
    memcpy(&caName,pdu->caData+32,32);
    Client::getinstance().getm_username() = caName;
    if(ret){
      Index::getinstence().show();
      Index::getinstence().setWindowTitle(Client::getinstance().getm_username());
      Client::getinstance().hide();
    }else{
     QMessageBox::information(&Client::getinstance(),"登录","用户名或密码错误");
    }
}

void MsgHandler::user_select(PDU *pdu)
{
    int ret;
    char name[32];
    memcpy(&ret,pdu->caData,sizeof(int));
    //qDebug()<<"respend ret"<<ret;
    memcpy(&name,pdu->caData+sizeof(int),32);
    //qDebug()<<"respend name"<<name;
    if(ret == 1){
        int res = QMessageBox::information(&Index::getinstence(),"搜索",QString("%1 在线").arg(name),"添加好友","取消");
        if(res != 0 ){
            return;
        }
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REQUST;
        memcpy(pdu->caData,Client::getinstance().getm_username().toStdString().c_str(),32);
        memcpy(pdu->caData+32,name,32);
        Client::getinstance().sendPDU(pdu);

    }else if(ret == 0){
        QMessageBox::information(&Index::getinstence(),"搜索","用户不在线");
    }else{
        QMessageBox::information(&Index::getinstence(),"搜索","该用户不存在");
    }


}

void MsgHandler::online_users(PDU *pdu)
{
    uint n = pdu->uiMsgLen/32;
    QStringList userList;
    for(int i=0;i<n;i++){
        char name[32];
        memcpy(&name,pdu->caMsg+i*32,32);
        if(QString(name) == Client::getinstance().getm_username()){
            continue;
        }
        userList.append(name);
    }
    Index::getinstence().getFriend()->getOnlineUser()->updateOnlineUserWidgetList(userList);
    qDebug()<<n;
}

void MsgHandler::add_friend(PDU *pdu)
{
    int ret;
    memcpy(&ret,pdu->caData,sizeof(int));
    qDebug()<<"Serve ret :"<<ret;
    QString showMsg;
    if(ret == 1){
        QMessageBox::information(Index::getinstence().getFriend(),"添加好友","添加成功");
    }
    else if(ret == 0){
        QMessageBox::information(Index::getinstence().getFriend(),"添加好友","用户离线");
    }else if(ret == -1){
         QMessageBox::information(Index::getinstence().getFriend(),"添加好友","用户不存在");
    }else if(ret == -2){
         QMessageBox::information(Index::getinstence().getFriend(),"添加好友","已经是好友");
    }

}

void MsgHandler::accept_friend(PDU *pdu)
{
    char CaCurName[32]={'\0'};
    char CaTarName[32]={'\0'};
    memcpy(&CaCurName,pdu->caData,32);
    memcpy(&CaTarName,pdu->caData+32,32);
    qDebug()<<"caCurName:"<<CaCurName<<endl<<"caTarName :"<<CaTarName;
    int ret = QMessageBox::question(&Index::getinstence(),"好友请求",QString("是否同意 %1 的添加好友请求").arg(CaCurName));
    if(ret != QMessageBox::Yes){
        return;
    }
    PDU *repdu = mkPDU(0);
    memcpy(repdu->caData,&CaCurName,32);
    memcpy(repdu->caData+32,&CaTarName,32);
    repdu->uiMsgType = ENUM_MSG_TYPE_ACCEPT_FRIEND_REQUST;
    Client::getinstance().sendPDU(repdu);
}



void MsgHandler::accept_friend_res(PDU *pdu)
{

    QMessageBox::information(&Index::getinstence(),"添加好友","成功添加");
}

void MsgHandler::flush_friend(PDU *pdu)
{
    uint n = pdu->uiMsgLen/32;
    QStringList userList;
    char name[32];
    for(int i=0;i<n;i++){
        memcpy(&name,pdu->caMsg+i*32,32);
        userList.append(QString(name));
    }
    Index::getinstence().getFriend()->updateFriendLW(userList);
}

void MsgHandler::delete_friend_res(PDU *pdu)
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret == true){
        QMessageBox::information(&Client::getinstance(),"删除好友","删除成功");
    }else{
        QMessageBox::information(&Client::getinstance(),"删除好友","删除失败");
    }
    Index::getinstence().getFriend()->flushFriend();
}

void MsgHandler::chat(PDU *pdu)
{
    Chat * c = Index::getinstence().getFriend()->getChat();
    if(c->isHidden()){
        c->show();
    }
    char caChatName[32]={"\0"};
    memcpy(caChatName,pdu->caData+32,32);
    QString strMsg = QString("%1 : %2").arg(caChatName).arg(pdu->caMsg);
    c->strChatName = caChatName;
    c->updateShow_LE(strMsg);
}

void MsgHandler::mkdir(PDU *pdu)
{
    bool res;
    memcpy(&res,pdu->caData,sizeof(bool));
    qDebug()<<"ret:"<<res<<endl;
    if(res){
        QMessageBox::information(Index::getinstence().getFile(),"创建文件夹","创建成功");
        Index::getinstence().getFile()->flushFile();
    }else{
        QMessageBox::information(Index::getinstence().getFile(),"创建文件夹","创建失败");
    }
}

void MsgHandler::flush_file(PDU *pdu)
{
    int count = pdu->uiMsgLen/sizeof(FileInfo);
    QList<FileInfo * > pFileList;
    for(int i=0;i<count ;i++){
        FileInfo * pFileInfo = new FileInfo;
        memcpy(pFileInfo,pdu->caMsg+i*sizeof(FileInfo),sizeof(FileInfo));
        qDebug()<<"File Name = "<<pFileInfo->caName<<"File Type = "<<pFileInfo->iFileType;
        pFileList.append(pFileInfo);
    }
    Index::getinstence().getFile()->updateFileList(pFileList);
}

void MsgHandler::delDir(PDU *pdu)
{
    bool ret;

    memcpy(&ret,pdu->caData,sizeof(bool));
    qDebug()<<"\n ret:"<<ret<<endl;
    if(ret){
        QMessageBox::information(Index::getinstence().getFile(),"删除文件夹","删除成功");
        Index::getinstence().getFile()->flushFile();
    }else{
        QMessageBox::information(Index::getinstence().getFile(),"删除文件夹","删除失败");
    }
}

void MsgHandler::delFile(PDU *pdu)
{
    bool ret;

    memcpy(&ret,pdu->caData,sizeof(bool));
    qDebug()<<"\n ret:"<<ret<<endl;
    if(ret){
        QMessageBox::information(Index::getinstence().getFile(),"删除文件","删除成功");
        Index::getinstence().getFile()->flushFile();
    }else{
        QMessageBox::information(Index::getinstence().getFile(),"删除文件","删除失败");
    }
}

void MsgHandler::Rename_File(PDU *pdu)
{
    bool ret;

    memcpy(&ret,pdu->caData,sizeof(bool));
    qDebug()<<"\n ret:"<<ret<<endl;
    if(ret){
        QMessageBox::information(Index::getinstence().getFile(),"重命名文件","重命名成功");
        Index::getinstence().getFile()->flushFile();
    }else{
        QMessageBox::information(Index::getinstence().getFile(),"重命名文件","重命名失败");
    }
}

void MsgHandler::Move_File(PDU *pdu)
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    qDebug()<<"\n ret:"<<ret<<endl;
    if(ret){
        QMessageBox::information(Index::getinstence().getFile(),"移动文件","移动成功");
        Index::getinstence().getFile()->flushFile();
    }else{
        QMessageBox::information(Index::getinstence().getFile(),"移动文件","移动失败");
    }
}

void MsgHandler::Upload_File(PDU *pdu)
{
    int ret;
    memcpy(&ret,pdu->caData,sizeof(int));
    qDebug()<<"Serve ret :"<<ret;
    QString showMsg;
    if(ret == 1){
        QMessageBox::information(Index::getinstence().getFile(),"上传文件","已经有文件在上传");
    }
    else if(ret == 0){
        Index::getinstence().getFile()->uploadFile();
    }else if(ret == -1){
         QMessageBox::information(Index::getinstence().getFile(),"上传文件","打开文件失败");
    }
}

void MsgHandler::UploadData_File(PDU *pdu)
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    qDebug()<<"\n ret:"<<ret<<endl;
    if(ret){
        QMessageBox::information(Index::getinstence().getFile(),"上传文件","上传文件成功");
        Index::getinstence().getFile()->flushFile();
    }else{
        QMessageBox::information(Index::getinstence().getFile(),"上传文件","上传文件失败");
    }
}

void MsgHandler::download_File(PDU *pdu)
{
    QMessageBox::information(Index::getinstence().getFile(),"下载文件","下载文件失败：服务器无法打开");
}

void MsgHandler::download_File_Data_Agree(PDU *pdu)
{
    int ret = QMessageBox::question(Index::getinstence().getFile(),"下载文件","是否要下载选中文件");
    if(ret == QMessageBox::No){
        return;
    }
    Index::getinstence().getFile()->upDownloadFileDataAgree(pdu);
}

void MsgHandler::download_File_Data(PDU *pdu)
{
    Index::getinstence().getFile()->upDownloadFileData(pdu);
}

void MsgHandler::share_File(PDU *pdu)
{
    QMessageBox::information(Index::getinstence().getFile(),"分享文件","分享文件成功");

}

void MsgHandler::share_File_Request(PDU *pdu)
{
    QString strSharePath = pdu->caMsg;
    int index = strSharePath.lastIndexOf('/');
    QString strFileName = strSharePath.right(strSharePath.size() -index -1);
    int ret = QMessageBox::question(Index::getinstence().getFile()->m_pShareFile,
                                    "分享文件",
                                    QString("%1分享文件 %2 \n是否接受？")
                                    .arg(pdu->caData).arg(strFileName));
    if(ret !=QMessageBox::Yes){
        return;
    }

    qDebug()<<"strFileName:"<<strFileName;
    qDebug()<<"strSharePath:"<<strSharePath;
    PDU *respdu = mkPDU(pdu->uiMsgLen);
    respdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_AGREE_REQUEST;
    memcpy(respdu->caData,Client::getinstance().getm_username().toStdString().c_str(),32);
    memcpy(respdu->caMsg,pdu->caMsg,pdu->uiMsgLen);
    Client::getinstance().sendPDU(respdu);

}

void MsgHandler::share_File_AGREE(PDU *pdu)
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    qDebug()<<"\n ret:"<<ret<<endl;
    if(ret){
        QMessageBox::information(Index::getinstence().getFile(),"分享文件","分享文件成功");
        Index::getinstence().getFile()->flushFile();
    }else{
        QMessageBox::information(Index::getinstence().getFile(),"分享文件","分享文件失败");
    }
}
