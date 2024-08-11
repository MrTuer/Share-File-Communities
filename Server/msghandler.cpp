#include "msghandler.h"
#include "operatedb.h"
#include "mytcpsocket.h"
#include "protocol.h"
#include "mytcpsocket.h"
#include "operatedb.h"
#include <QDir>
#include <QString>
#include <QMessageBox>
#include "operatedb.h"
#include "mytcpserver.h"
#include "server.h"
MsgHandler::MsgHandler()
{
    m_bUpload = false;
    m_bDownload = false;
}

MsgHandler::~MsgHandler()
{
}
PDU * MsgHandler::resign(PDU *pdu)
{
    char caName[32] = {'\0'};
    char caPwd[32] = {'\0'};
    memcpy(caName,pdu->caData,32);
    memcpy(caPwd,pdu->caData+32,32);

    bool ret = OperateDB::getinstance().handleRegist(caName,caPwd);
    if(ret == true){
        QDir dir;
        dir.mkdir(QString("%1/%2").arg(Server::getinstance().get_RootPath()).arg(caName));
    }
    PDU  *respdu =mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_REGIST_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;
}

PDU *MsgHandler::login(PDU *pdu,QString &Loginer)
{
    char caName[32] = {'\0'};
    char caPwd[32] = {'\0'};
    memcpy(caName,pdu->caData,32);
    memcpy(caPwd,pdu->caData+32,32);
    qDebug()<<"Name :"<<caName<<"\n"<<"aPwd:"<<caPwd;
    qDebug()<<"ret :"<<pdu->uiMsgType;
    bool ret = OperateDB::getinstance().handleLogin(caName,caPwd);

    if(ret ){
        Loginer = caName;
    }
    PDU  *respdu =mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    memcpy(respdu->caData+32,&caName,32);
    return respdu;
}

PDU *MsgHandler::user_select(PDU *pdu)
{
    char caName[32] = {'\0'};
    memcpy(caName,pdu->caData,32);

    qDebug()<<"Name"<<caName;
    int ret = OperateDB::getinstance().hand_select_user(caName);
    qDebug()<<"ret"<<ret;
    PDU  *respdu =mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_USER_SELECT_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(int));
    memcpy(respdu->caData+sizeof(int),caName,32);
    return respdu;
}

PDU *MsgHandler::online_users()
{
    QStringList CaName;
    CaName = OperateDB::getinstance().hand_online_users();
    uint n = CaName.size()*32;
    PDU  *respdu =mkPDU(n);
    for(int i=0;i<CaName.size();i++){
        memcpy(respdu->caMsg+i*32,CaName.at(i).toStdString().c_str(),32);
    }
    respdu->uiMsgType = ENUM_MSG_TYPE_ONLINE_USER_RESPEND;
    return respdu;

}

PDU *MsgHandler::add_friend(PDU *pdu)
{
    char caCurName[32] = {'\0'};
    char caTarName[32] = {'\0'};
    memcpy(caCurName,pdu->caData,32);
    memcpy(caTarName,pdu->caData+32,32);
    int ret = OperateDB::getinstance().handleaddfriend(caCurName,caTarName);
    qDebug()<<"handleaddfriend ret : "<<ret;
    if(ret== 1){
        MyTcpServer::getinstance().resent(caTarName,pdu);
    }
    PDU  *respdu =mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(int));
    return respdu;
}

PDU *MsgHandler::accept_friend(PDU *pdu)
{
    char caCurName[32] = {'\0'};
    char caTarName[32] = {'\0'};
    memcpy(caCurName,pdu->caData,32);
    memcpy(caTarName,pdu->caData+32,32);
    qDebug()<<"caCurName:"<<caCurName
            <<endl
            <<"caTarName :"<<caTarName;
    int ret = OperateDB::getinstance().handacceptfriend(caCurName,caTarName);
    qDebug()<<"handacceptfriend ret : "<<ret;

    PDU  *respdu =mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_ACCEPT_FRIEND_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(int));
    MyTcpServer::getinstance().resent(caCurName,respdu);
    return respdu;
}

PDU *MsgHandler::flush_online_friend(PDU *pdu)
{
    char caCurName[32] = {'\0'};
    memcpy(caCurName,pdu->caData,32);
    QStringList CaName;
    CaName = OperateDB::getinstance().handleOnline1Friend(caCurName);
    uint n = CaName.size()*32;
    PDU  *respdu =mkPDU(n);
    for(int i=0;i<CaName.size();i++){
        memcpy(respdu->caMsg+i*32,CaName.at(i).toStdString().c_str(),32);
    }
    respdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FRIEND_RESPEND;
    return respdu;
}

PDU *MsgHandler::delete_friend(PDU *pdu)
{
    char caCurName[32] = {'\0'};
    char caTarName[32] = {'\0'};
    memcpy(caCurName,pdu->caData,32);
    memcpy(caTarName,pdu->caData+32,32);
    qDebug()<<"caCurName:"<<caCurName
            <<endl
            <<"caTarName :"<<caTarName;
    bool ret = OperateDB::getinstance().handledelete_friend(caCurName,caTarName);
    qDebug()<<"ret:"<<ret;
    PDU  *respdu =mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;
}

void MsgHandler::chat(PDU *pdu)
{
    char caTarName[32] = {'\0'};
    char caCurName[32] = {'\0'};
    memcpy(caTarName,pdu->caData,32);
    memcpy(caCurName,pdu->caData+32,32);
    qDebug()<<"caTarName :"<<caTarName;
    MyTcpServer::getinstance().resent(caTarName,pdu);
    MyTcpServer::getinstance().resent(caCurName,pdu);
}

PDU *MsgHandler::mkdir(PDU *pdu)
{
    QString strCurPath = pdu->caMsg;
    PDU * respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_CREATE_MKDIR_RESPEND;
    bool res = false;
    QDir dir;
    if(!dir.exists(strCurPath)){
        memcpy(respdu->caData,&res,sizeof(bool));
        qDebug()<<"strCurPath "<<strCurPath;
        return respdu;
    }
    QString strNewPath = QString("%1/%2").arg(strCurPath).arg(pdu->caData);
    qDebug()<<"mkdir:newPath: "<<strNewPath;
    if(dir.exists(strNewPath)||!dir.mkdir(strNewPath)){
        memcpy(respdu->caData,&res,sizeof(bool));
        return respdu;
    }
    res = true;
    memcpy(respdu->caData,&res,sizeof(bool));
    return respdu;
}

PDU *MsgHandler::flushFile(PDU *pdu)
{
    QString strPath = pdu->caMsg;
    QDir dir(strPath);
    QFileInfoList fileInfoList = dir.entryInfoList();
    int ifileCount = fileInfoList.size();
    PDU * respdu = mkPDU(sizeof(FileInfo)*(ifileCount-2));
    respdu->uiMsgType=ENUM_MSG_TYPE_FLUSH_FILE_RESPEND;
    FileInfo * pFileInfo = NULL;
    QString strFileName;
    for(int i=0,j=0;i<ifileCount;i++){
        strFileName = fileInfoList[i].fileName();
        if(strFileName == QString(".") || strFileName==QString("..")){
            continue;
        }
        pFileInfo = (FileInfo *)(respdu->caMsg)+j++;
        memcpy(pFileInfo->caName,strFileName.toStdString().c_str(),32);
        if(fileInfoList[i].isDir()){
            pFileInfo->iFileType = 0; //目录
        }else if(fileInfoList[i].isFile()){
            pFileInfo->iFileType = 1; //文件
        }
        qDebug()<<"FLush strFILE Name = "<<pFileInfo->caName
                <<"ifiletype = "<<pFileInfo->iFileType;


    }
    return respdu;

}

PDU *MsgHandler::delMkdir(PDU *pdu)
{
    QFileInfo fileInfo(pdu->caMsg);
    bool res = false;
    if(fileInfo.isDir()){
        QDir dir(pdu->caMsg);
        res = dir.removeRecursively();

        qDebug()<<"\nres:"<<res<<endl;
    }
    PDU *respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_DELETE_MKDIR_RESPEND;
    memcpy(respdu->caData,&res,sizeof(bool));
    return respdu;
}

PDU *MsgHandler::delFile(PDU *pdu)
{
    QFileInfo fileInfo(pdu->caMsg);
    bool res = false;
    if(fileInfo.isFile()){

        qDebug()<<"pdu->caMsg:"<<pdu->caMsg;
        res = QFile::remove(pdu->caMsg);

        qDebug()<<"res:"<<res<<endl;
    }
    PDU *respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FILE_RESPEND;
    memcpy(respdu->caData,&res,sizeof(bool));
    return respdu;
}

PDU *MsgHandler::RenameFile(PDU *pdu)
{
    //读出文件名跟路径
    char strNewFileName[32]={'\0'};
    char strOldFileName[32]={'\0'};
    memcpy(strNewFileName,pdu->caData,32);
    memcpy(strOldFileName,pdu->caData+32,32);

    char *pPath = pdu->caMsg;
    //合并完整路径
    QString strOldPath = QString("%1/%2").arg(pPath).arg(strOldFileName);
    QString strNewPath = QString("%1/%2").arg(pPath).arg(strNewFileName);

    //使用dir的rename进行重命名。
    QDir dir;
    bool res = dir.rename(strOldPath,strNewPath);
    qDebug()<<"\nres:"<<res<<endl;
    PDU *respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_RENAME_FILE_RESPEND;
    memcpy(respdu->caData,&res,sizeof(bool));
    return respdu;
}

PDU *MsgHandler::MoveFile(PDU *pdu)
{
    //读出长度
    int srcLen =0;
    int tarLen =0;
    memcpy(&srcLen,pdu->caData,sizeof(int));
    memcpy(&tarLen,pdu->caData+sizeof(int),sizeof(int));
    //读出路径
    char *strOldPath = new char[srcLen+1];
    char *strNewPath = new char[tarLen+1];

    memset(strOldPath,'\0',srcLen+1);
    memset(strNewPath,'\0',tarLen+1);

    memcpy(strOldPath,pdu->caMsg,srcLen);
    memcpy(strNewPath,pdu->caMsg+srcLen ,tarLen);

    qDebug()<<"srcLen:"<<srcLen
            <<"\ntarLen:"<<tarLen
            <<"\nstrOldPath:"<<strOldPath
            <<"\nstrNewPath:"<<strNewPath;
    bool ret = QFile::rename(strOldPath,strNewPath);

    PDU *resPDU = mkPDU(0);
    resPDU->uiMsgType = ENUM_MSG_TYPE_MOVE_FILE_RESPEND;
    memcpy(resPDU->caData,&ret,sizeof (bool));
    delete[] strOldPath;
    delete[] strNewPath;
    strOldPath=nullptr;
    strNewPath=nullptr;
    return resPDU;
}

PDU *MsgHandler::uploadFile(PDU *pdu)
{
    PDU * respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_RESPEND;
    int ret = 0;
    memcpy(respdu->caData,&ret,sizeof(int));
    if(m_bUpload){
        qDebug()<<"已经有文件在上传";
        ret = 1;
        memcpy(respdu->caData,&ret,sizeof(int));
        return respdu;
    }
    char caFileName[32] ={'\0'};
    qint64 fileSize =0;

    memcpy(caFileName,pdu->caData,32);
    memcpy(&fileSize,pdu->caData+32,sizeof(qint64));
    qDebug()<<"caFileName!"<<caFileName;
    qDebug()<<"fileSize!"<<fileSize;
    QString strPath = QString("%1/%2").arg(pdu->caMsg).arg(caFileName);
    m_fUploadFile.setFileName(strPath);
    qDebug()<<"strPath!"<<strPath;

    if(m_fUploadFile.open(QIODevice::WriteOnly)){
        m_bUpload = true;
        m_fUploadFileTotalSize = fileSize;
        m_fUploadRecveived =0;
        qDebug()<<"open File successed!";
    }else{
        qDebug()<<"open File error !";
        ret = -1;

    }

    memcpy(respdu->caData,&ret,sizeof(int));
    return respdu;
}

PDU *MsgHandler::uploadFileData(PDU *pdu)
{
    m_fUploadFile.write(pdu->caMsg,pdu->uiMsgLen);
    m_fUploadRecveived+=pdu->uiMsgLen;
    if(m_fUploadRecveived < m_fUploadFileTotalSize){
        return NULL;
    }
    m_fUploadFile.close();
    m_bUpload =false;
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_DATA_RESPEND;
    bool ret = m_fUploadRecveived ==m_fUploadFileTotalSize;
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;
}

PDU *MsgHandler::downloadFile(PDU *pdu)
{
    //记录客户名
    char strCurName[32] = {'\0'};
    memcpy(strCurName,pdu->caData,32);
    m_sDownloadFilePath = pdu->caMsg;
    QString strLoadPath = m_sDownloadFilePath;
    qDebug()<<"strLoadPath:"<<strLoadPath;
    m_sDownloadFile.setFileName(strLoadPath);
    //不能打开
    if(!m_sDownloadFile.open(QFile::ReadOnly)){
        PDU *resendpdu = mkPDU(0);
        resendpdu->uiMsgType =ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST;
        return resendpdu;
    }
    //能打开 发送传输数据请求
    PDU *respdu = mkPDU(0);
    qint64 fileSize = m_sDownloadFile.size();
    memcpy(respdu->caData,&fileSize,sizeof(qint64));
    respdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPEND;
    return respdu;
}

PDU *MsgHandler::downloadFileData(PDU *pdu)
{
    char strCurName[32] = {'\0'};
    memcpy(strCurName,pdu->caData,32);
    qDebug()<<"strCurName:"<<strCurName;

    m_bDownload = true;
    PDU *datapdu = mkPDU(4096);
    datapdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_DATA_RESPEND;
    qint64 ret =0;
    while(1){
        ret = m_sDownloadFile.read(datapdu->caMsg,4096);
        if(ret==0){
            break;
        }
        if(ret<0){

            break;
        }

        qDebug()<<"ret:"<<ret;

        datapdu->uiMsgLen = ret;
        datapdu->uiPDULen = ret + sizeof (PDU);
        MyTcpServer::getinstance().resent(strCurName,datapdu);
    }
//    sleep(1000);
//    m_bDownload = false;
//    m_sDownloadFile.close();
//    free(datapdu);
//    datapdu = nullptr;
    return datapdu;
}

PDU *MsgHandler::shareFile(PDU *pdu)
{
    char strCurName[32] = {'\0'};
    int friendNum = 0;
    memcpy(strCurName,pdu->caData,32);
    memcpy(&friendNum,pdu->caData+32,sizeof(int));
    int size = friendNum*32;

    qDebug()<<"strCurName:"<<strCurName;
    qDebug()<<"friendNum:"<<friendNum;

    PDU * resendpdu = mkPDU(pdu->uiMsgLen - size);

    resendpdu->uiMsgType=pdu->uiMsgType;

    memcpy(resendpdu->caData,strCurName,32);
    memcpy(resendpdu->caMsg,pdu->caMsg+size,pdu->uiMsgLen - size);

    qDebug()<<"strCurName:"<<strCurName;
    qDebug()<<"pdu->caMsg:"<<pdu->caMsg;
    char friendName[32]={'\0'};
    for(int i=0;i<friendNum;i++){

        memcpy(friendName,pdu->caMsg+i*32,32);
        qDebug()<<"friendName:"<<friendName;
        MyTcpServer::getinstance().resent(friendName,resendpdu);
    }
    free(resendpdu);
    resendpdu = nullptr;

    PDU * respdu = mkPDU(0);
    respdu->uiMsgType =ENUM_MSG_TYPE_SHARE_FILE_RESPEND;
    return respdu;

}

PDU *MsgHandler::shareFileAgree(PDU *pdu)
{
    QString strShareFilePath = pdu->caMsg;
    QString strRecvPath = QString("%1/%2").arg(Server::getinstance().get_RootPath()).arg(pdu->caData);

    qDebug()<<"strShareFilePath:"<<strShareFilePath;

    qDebug()<<"strRecvPath:"<<strRecvPath;

    int index = strShareFilePath.lastIndexOf('/');
    QString strFileName = strShareFilePath.right(strShareFilePath.size()-1-index);
    strRecvPath = strRecvPath +"/"+strFileName;
    qDebug()<<"strRecvPath:"<<strRecvPath;
    bool ret = QFile::copy(strShareFilePath,strRecvPath);
    PDU *respdu =mkPDU(0);
    memcpy(respdu->caData,&ret,sizeof(bool));
    respdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_AGREE_RESPEND;
    return  respdu;
}

