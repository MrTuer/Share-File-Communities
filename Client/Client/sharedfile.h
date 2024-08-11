#ifndef SHAREDFILE_H
#define SHAREDFILE_H

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class SharedFile;
}

class SharedFile : public QWidget
{
    Q_OBJECT

public:
    explicit SharedFile(QWidget *parent = nullptr);
    ~SharedFile();
    void updateFriend_LW();

private slots:
    void on_selectall_PB_clicked();

    void on_concel_PB_clicked();

    void on_sure_PB_clicked();

private:
    Ui::SharedFile *ui;
};

#endif // SHAREDFILE_H
