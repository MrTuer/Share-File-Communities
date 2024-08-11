#ifndef MSGHANDLER1_H
#define MSGHANDLER1_H
#include"protocol.h"
#include<QFile>
#include<QDebug>
#include<QLineEdit>
#include<QWidget>
#include<QTcpSocket>
#include<QObject>
#include<QTcpServer>
#include<QString>
#include<QMessageBox>
#include <QObject>
class MsgHandler
{
public:
    MsgHandler();
    ~MsgHandler();
    PDU * resign(PDU *pdu);
    PDU * login(PDU *pdu,QString &Loginer);
    PDU * user_select(PDU *pdu);
    PDU * online_users();
    PDU * add_friend(PDU *pdu);
    PDU * accept_friend(PDU *pdu);
    PDU * flush_online_friend(PDU *pdu);
    PDU * delete_friend(PDU * pdu);
    void chat(PDU * pdu);
    PDU * mkdir(PDU * pdu);
    PDU * flushFile(PDU * pdu);
    PDU * delMkdir(PDU * pdu);
    PDU * delFile(PDU * pdu);
    PDU * RenameFile(PDU * pdu);
    PDU * MoveFile(PDU * pdu);
    PDU * uploadFile(PDU * pdu);
    PDU * uploadFileData(PDU * pdu);
    PDU * downloadFile (PDU * pdu);
    PDU * downloadFileData (PDU * pdu);
    PDU * shareFile (PDU * pdu);
    PDU * shareFileAgree (PDU * pdu);

    //上传文件要用的
    QFile m_fUploadFile;
    qint64 m_fUploadFileTotalSize;
    qint64 m_fUploadRecveived;
    bool m_bUpload;
    //下载文件
    qint64 m_sDownloadTotal;//文件总大小
    QString m_sDownloadFilePath;//要下载的文件的路径
    QFile m_sDownloadFile;//打开文件
    bool m_bDownload;

    //分享文件

};

#endif // MSGHANDLER1_H
