#ifndef MSGHANDLER_H
#define MSGHANDLER_H
#include"protocol.h"
#include <QWidget>
#include <QWidget>
#include <QTcpSocket>
#include<QFile>
#include<QDebug>
#include<QStringList>
#include<QHostAddress>
#include<QTcpSocket>
#include<QHostAddress>
#include"protocol.h"
class MsgHandler
{
public:
    MsgHandler();
    void  resign(PDU *pdu);
    void  login(PDU *pdu);
    void  user_select(PDU *pdu);
    void  online_users(PDU *pdu);
    void  add_friend(PDU *pdu);
    void  accept_friend(PDU *pdu);
    void  addFriendRequset(PDU *pdu);
    void  accept_friend_res(PDU *pdu);
    void  flush_friend(PDU *pdu);
    void  delete_friend_res(PDU* pdu);
    void  chat(PDU *pdu);
    void  mkdir(PDU * pdu);
    void  flush_file(PDU * pdu);
    void  delDir(PDU *pdu);
    void  delFile(PDU *pdu);
    void  Rename_File(PDU *pdu);
    void  Move_File(PDU *pdu);
    void  Upload_File(PDU *pdu);
    void  UploadData_File(PDU *pdu);
    void  download_File(PDU *pdu);
    void  download_File_Data_Agree(PDU *pdu);
    void  download_File_Data(PDU *pdu);

    void  share_File(PDU *pdu);
    void  share_File_Request(PDU *pdu);
    void  share_File_AGREE(PDU *pdu);

};

#endif // MSGHANDLER_H
