#include "client.h"
#include "file.h"
#include "ui_file.h"

#include <qinputdialog.h>
#include <QTimer>
#include <qmessagebox.h>
#include <qmessagebox.h>
#include <ui_file.h>
#include <QFileDialog>

File::File(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::File)
{
    ui->setupUi(this);
    m_strCurPath = QString("%1/%2")
            .arg(Client::getinstance().getm_strRootPath())
            .arg(Client::getinstance().m_username);
    m_strUserPath = m_strCurPath;
    m_bUpload =false;
    m_bDownload =false;
    flushFile();
    m_pShareFile = new SharedFile;
}

File::~File()
{
    delete ui;
    delete m_pShareFile;
}

void File::flushFile()
{
    qDebug()<<"FLUSH flie staart";
    PDU *pdu = mkPDU(m_strCurPath.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FILE_REQUEST;
    memcpy(pdu->caMsg,m_strCurPath.toStdString().c_str(),m_strCurPath.toStdString().size());
    Client::getinstance().sendPDU(pdu);

}

void File::updateFileList(QList<FileInfo *> pFileList)
{

    ui->file_LW->clear();
    foreach(FileInfo * pFileInfo,m_fileList){
        delete pFileInfo;
    }
    m_fileList.clear();
    m_fileList = pFileList;
    foreach(FileInfo * pFileInfo,pFileList){
        QListWidgetItem *pItem = new QListWidgetItem;
        if(pFileInfo->iFileType==0){
            pItem->setIcon(QIcon(QPixmap(":/Dir.png")));
        }else if(pFileInfo->iFileType == 1){
            pItem->setIcon(QIcon(QPixmap(":/file.png")));
        }
        pItem->setText(pFileInfo->caName);
        //qDebug()<<"File Name = "<<pFileInfo->caName<<"File Type = "<<pFileInfo->iFileType;
        ui->file_LW->addItem(pItem);
    }

}

void File::uploadFile()
{
    QFile file(m_strUploadPath);
    if(!file.open(QFile::ReadOnly)){
         QMessageBox::warning(this,"上传文件","打开文件失败");
         return ;
    }
    m_bUpload = true;
    PDU *datapdu = mkPDU(4096);
    datapdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_DATA_REQUEST;
    qint64 ret =0;
    while(1){
        ret = file.read(datapdu->caMsg,4096);
        if(ret==0){
            break;
        }
        if(ret<0){
            QMessageBox::warning(this,"上传文件","上传文件失败:读取失败");
            break;
        }
        datapdu->uiMsgLen = ret;
        datapdu->uiPDULen = ret + sizeof (PDU);
        Client::getinstance().getSocket().write((char*)datapdu,datapdu->uiPDULen);
    }
    m_bUpload = false;
    file.close();
    free(datapdu);
    datapdu = nullptr;
}

void File::upDownloadFileDataAgree(PDU * pdu)
{
    //确定接收文件的 大小
    m_fDownloadRecveived = 0;
    m_bDownload = true;
    memcpy(&m_fDownloadFileTotalSize,pdu->caData,sizeof(qint64));
    //返回确认传输信息的信号

    qDebug()<<"m_fDownloadFileTotalSize:"<<m_fDownloadFileTotalSize;
    qDebug()<<"Agree download data :pdu->caData:"<<pdu->caData;
    PDU *respdu = mkPDU(0);
    memcpy(respdu->caData,Client::getinstance().getm_username().toStdString().c_str(),32);
    qDebug()<<"Agree download data :getm_username:"<<respdu->caData;
    respdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_DATA_REQUEST;
    Client::getinstance().sendPDU(respdu);
}


void  File::upDownloadFileData(PDU *pdu)
{
    m_fDownloadFile.write(pdu->caMsg);
    m_fDownloadRecveived += pdu->uiMsgLen;
    qDebug()<<"m_fDownloadRecveived:"<<m_fDownloadRecveived;
    if(m_fDownloadRecveived < m_fDownloadFileTotalSize){
        qDebug()<<"m_fDownloadRecveived error:";
        return;
    }

    qDebug()<<"m_fDownloadRecveived:"<<m_fDownloadRecveived;
    m_fDownloadFile.close();
    m_bDownload =false;
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_DATA_RESPEND;
    bool ret = m_fDownloadRecveived ==m_fDownloadFileTotalSize;
    if(ret){
        QMessageBox::information(this,"下载文件","下载文件成功");;
    }
}


SharedFile *File::get_SharedFile()
{
    return m_pShareFile;;
}


void File::on_mkDir_PB_clicked()
{
    QString strNewDir = QInputDialog::getText(this,"新建文件夹","文件夹名");
    if(strNewDir.isEmpty()||strNewDir.toStdString().size()>32){
        QMessageBox::warning(this,"新建文件夹","文件夹名称长度非法");
        return;
    }
    //文件夹名放入caData 路径放入caMsg
    qDebug()<<"strNewDir"<<strNewDir;
    PDU *pdu = mkPDU(m_strCurPath.toStdString().size());
    pdu->uiMsgType = ENUM_MSG_TYPE_CREATE_MKDIR_REQUEST;
    memcpy(pdu->caData,strNewDir.toStdString().c_str(),32);
    memcpy(pdu->caMsg,m_strCurPath.toStdString().c_str(),m_strCurPath.toStdString().size());
    qDebug()<<"pdu->caMsg"<<m_strCurPath;
    Client::getinstance().sendPDU(pdu);

}

void File::on_flushFlie_PB_clicked()
{
    flushFile();
}

void File::on_rename_PB_clicked()
{


    QListWidgetItem *pItem = ui->file_LW->currentItem();
    if(pItem == NULL){
        QMessageBox::warning(this,"重命名文件","请选择要重命名的文件");
        return;
    }


    QString strOldFileName = pItem->text();
    QString strNewFileName = QInputDialog::getText(this,"重命名文件","新文件名");
    if(strNewFileName.isEmpty()||strNewFileName.toStdString().size()>32){
        QMessageBox::warning(this,"重命名文件","新文件名长度非法");
        return;
    }
        //确认重命名


    PDU *pdu = mkPDU(m_strCurPath.toStdString().size()+1);
    pdu->uiMsgType =ENUM_MSG_TYPE_RENAME_FILE_REQUEST;
    memcpy(pdu->caData,strNewFileName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,strOldFileName.toStdString().c_str(),32);
    memcpy(pdu->caMsg,m_strCurPath.toStdString().c_str(),m_strCurPath.toStdString().size());
    Client::getinstance().sendPDU(pdu);



}

void File::on_file_LW_itemDoubleClicked(QListWidgetItem *item)
{
    QString strDirName = item->text();
    foreach(FileInfo * pfileInfo,m_fileList){
        if(pfileInfo->caName == strDirName && pfileInfo->iFileType !=0){
            QMessageBox::warning(this,"进入文件夹","请选择文件夹");
            return;
        }
    }
    m_strCurPath = m_strCurPath+"/"+strDirName;
    flushFile();
}

void File::on_return_PB_clicked()
{
    if(m_strCurPath ==m_strUserPath){
        QMessageBox::warning(this,"返回","返回失败：已在顶层目录");
        return;
    }
    int index = m_strCurPath.lastIndexOf('/');
    m_strCurPath.remove(index,m_strCurPath.size()-(index));

    flushFile();
}

void File::on_mvFile_PB_clicked()
{
    if(ui->mvFile_PB->text() == "移动文件"){
        QListWidgetItem *pItem = ui->file_LW->currentItem();
        if(pItem == NULL){
            QMessageBox::warning(this,"移动文件","请选择要移动的文件");
            return;
        }
        QMessageBox::warning(this,"移动文件","请选择要移动到的文件夹");
        ui->mvFile_PB->setText("确认/取消");
        m_strMvFuleName =pItem->text();
        m_strMvPath = m_strCurPath + "/" + m_strMvFuleName;
        return ;
    }
    QListWidgetItem *pItem = ui->file_LW->currentItem();
    QString strTarPath;
    QString BoxMsg;
    if(pItem == NULL){
        strTarPath = m_strCurPath +"/"+m_strMvFuleName;
        BoxMsg = QString("是否移动到当前目录下");
    }else{
        foreach(FileInfo * pfileInfo,m_fileList){
            if(pfileInfo->caName == pItem->text() && pfileInfo->iFileType !=0){
                QMessageBox::warning(this,"进入文件夹","请选择文件夹");
                return;
            }
        }
        strTarPath = m_strCurPath + "/" +pItem->text() + "/" +m_strMvFuleName;
        BoxMsg = QString("是否移动到已选中目录下");
    }
    int ret = QMessageBox::information(this,"移动文件",BoxMsg,"确认","取消");
    ui->mvFile_PB->setText("移动文件");
    if(ret != 0){
        return;
    }
    int srcLen = m_strMvPath.toStdString().size();
    int tarLen = strTarPath.toStdString().size();
    PDU *pdu = mkPDU(srcLen+tarLen+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_MOVE_FILE_REQUEST;
    qDebug()<<"srcLen:"<<srcLen
            <<"\ntarLen:"<<tarLen
            <<"\nm_strMvPath:"<<m_strMvPath
            <<"\nstrTarPath:"<<strTarPath;

    memcpy(pdu->caData,&srcLen,sizeof(int));
    memcpy(pdu->caData+sizeof(int),&tarLen,sizeof(int));

    memcpy(pdu->caMsg,m_strMvPath.toStdString().c_str(),srcLen);
    memcpy(pdu->caMsg+srcLen,strTarPath.toStdString().c_str(),tarLen);
    qDebug()<<"srcLen tarLen:"<<pdu->caData
            <<"\nm_strMvPath strTarPath:"<<pdu->caMsg;
    Client::getinstance().sendPDU(pdu);
}

void File::on_upload_PB_clicked()
{
    if(m_bUpload){
        QMessageBox::warning(this,"上传文件","已经有文件在上传");
        return;
    }
    m_strUploadPath = QFileDialog::getOpenFileName();
    qDebug()<<"m_strUploadPath:"<<m_strUploadPath;
    int index = m_strUploadPath.lastIndexOf('/');
    QString curFileName = m_strUploadPath.right(m_strUploadPath.size()-index-1);
    qDebug()<<"curFileName:"<<curFileName;
    QFile file(m_strUploadPath);
    qint64 fileSize = file.size();
    PDU *pdu = mkPDU(m_strCurPath.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST;
    memcpy(pdu->caData,curFileName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,&fileSize,sizeof(qint64));
    memcpy(pdu->caMsg,m_strCurPath.toStdString().c_str(),m_strCurPath.toStdString().size());
    Client::getinstance().sendPDU(pdu);
}

void File::on_shareFile_PB_clicked()
{

    QListWidgetItem *pItem = ui->file_LW->currentItem();
    if(pItem == NULL){
        QMessageBox::warning(this,"分享文件","请选择要分享的文件");
        return;
    }
    m_strSharedFile =pItem->text();
    m_pShareFile->updateFriend_LW();
    if(m_pShareFile->isHidden()){
        m_pShareFile->show();
    }
}

void File::on_download_PB_clicked()
{

    //先确认是否有文件在下载
    if(m_bDownload){
        QMessageBox::warning(this,"下载文件","已经有文件在下载");
        return;
    }
    //获取所点击的文件的名字
    QListWidgetItem *pItem = ui->file_LW->currentItem();
    m_strDownloadFile = pItem->text() ;

    //设置下载路径
    m_fDownloadPath = QFileDialog::getExistingDirectory()+"/"+ m_strDownloadFile ;
    qDebug()<<"m_strDownloadFile:"<<m_strDownloadFile;
    qDebug()<<"m_fDownloadPath:"<<m_fDownloadPath;
    //先尝试在本地新建一个同名文件
    m_fDownloadFile.setFileName(m_fDownloadPath);
    QFile file(m_fDownloadPath);
    if(m_fDownloadFile.open(QIODevice::WriteOnly)){
        //能打开 发送下载请求给客户端
        //需要把目标文件在客户端的路径发送给 服务器 方便读取数据发送给客户端。

        qDebug()<<"m_strCurPath:"<<m_strCurPath;
        QString m_filePath = QString("%1/%2")
                .arg(m_strCurPath)
                .arg(m_strDownloadFile);

        qDebug()<<"m_filePath!"<<m_filePath;
        PDU *pdu =mkPDU(m_filePath.toStdString().size()+1);
        pdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST;
        memcpy(pdu->caData,Client::getinstance().getm_username().toStdString().c_str(),32);
        memcpy(pdu->caMsg,m_filePath.toStdString().c_str(),m_filePath.toStdString().size());
        Client::getinstance().sendPDU(pdu);
        qDebug()<<"open File successed!";
    }else{
        qDebug()<<"open File fauil!";
    }

}

void File::on_delDir_PB_clicked()
{
    //获取当前文件夹名
    QListWidgetItem *pItem = ui->file_LW->currentItem();
    if(pItem == NULL){
        QMessageBox::warning(this,"请选择要删除的文件夹","请选择要删除的文件夹");
        return;
    }
    QString strDelFileName = pItem->text();
    //判断选择的是否是文件夹
    foreach(FileInfo * pfileInfo,m_fileList){
        if(pfileInfo->caName == strDelFileName && pfileInfo->iFileType !=0){
            QMessageBox::warning(this,"删除文件夹","请选择文件夹");
            return;
        }
        //确认删除
        int ret = QMessageBox::question(this,"删除文件夹",QString("是否删除文件夹 %1").arg(strDelFileName));
        if(ret ==QMessageBox::Yes){
            QString strPath = m_strCurPath +"/"+strDelFileName;
            PDU *pdu = mkPDU(strPath.toStdString().size()+1);
            pdu->uiMsgType =ENUM_MSG_TYPE_DELETE_MKDIR_REQUEST;
            memcpy(pdu->caMsg,strPath.toStdString().c_str(),strPath.toStdString().size());
            Client::getinstance().sendPDU(pdu);
            return;
        }else{
            return;
        }

    }
}

void File::on_delFile_PB_clicked()
{
    //获取当前文件夹名
    QListWidgetItem *pItem = ui->file_LW->currentItem();
    if(pItem == NULL){
        QMessageBox::warning(this,"请选择要删除的文件","请选择要删除的文件");
        return;
    }
    QString strDelFileName = pItem->text();
    //判断选择的是否是文件
    foreach(FileInfo * pfileInfo,m_fileList){
        if(pfileInfo->caName == strDelFileName && pfileInfo->iFileType != 1){
            QMessageBox::warning(this,"删除文件","请选择文件");
            return;
        }
        //确认删除
        int ret = QMessageBox::question(this,"删除文件",QString("是否删除文件 %1").arg(strDelFileName));
        if(ret ==QMessageBox::Yes){
            QString strPath = m_strCurPath +"/"+strDelFileName;
            PDU *pdu = mkPDU(strPath.toStdString().size()+1);
            pdu->uiMsgType =ENUM_MSG_TYPE_DELETE_FILE_REQUEST;
            memcpy(pdu->caMsg,strPath.toStdString().c_str(),strPath.toStdString().size());
            Client::getinstance().sendPDU(pdu);
            return;
        }else{
            return;
        }

    }
}
