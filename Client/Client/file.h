#ifndef FILE_H
#define FILE_H
#include <QWidget>
#include <QListWidgetItem>
#include <QFile>

#include "protocol.h"
#include "sharedfile.h"

namespace Ui {
class File;
}

class File : public QWidget
{
    Q_OBJECT

public:
    explicit File(QWidget *parent = nullptr);
    ~File();
    QString m_strCurPath;
    QString m_strUserPath;
    void flushFile();//处理刷新问津
    QList<FileInfo * > m_fileList;
    void updateFileList(QList<FileInfo *>pFileInfo);//更新文件列表
    //移动文件
    QString m_strMvFuleName;
    QString m_strMvPath;
    //上传文件
    bool m_bUpload;
    QString m_strUploadPath;
    void uploadFile();
    //下载文件
    QFile m_fDownloadFile;
    QString m_strDownloadFile;
    qint64 m_fDownloadFileTotalSize;
    qint64 m_fDownloadRecveived;
    bool m_bDownload;
    QString m_fDownloadPath;

    void upDownloadFileDataAgree(PDU * pdu);
    void upDownloadFileData(PDU *pdu);

//    //分享文件
    QString m_strSharedFile;
    SharedFile * m_pShareFile;
    SharedFile  *get_SharedFile();
//    QString m_strSharedFile;

private slots:
    void on_mkDir_PB_clicked();

    void on_flushFlie_PB_clicked();

    void on_delDir_PB_clicked();

    void on_rename_PB_clicked();

    void on_file_LW_itemDoubleClicked(QListWidgetItem *item);

    void on_return_PB_clicked();

    void on_mvFile_PB_clicked();

    void on_upload_PB_clicked();

    void on_shareFile_PB_clicked();

    void on_download_PB_clicked();

    void on_delFile_PB_clicked();

private:
    Ui::File *ui;

};

#endif // FILE_H
