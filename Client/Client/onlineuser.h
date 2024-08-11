#ifndef ONLINEUSER_H
#define ONLINEUSER_H

#include <QWidget>
#include <QWidget>
#include <QStringList>
#include <QWindow>
#include <QDebug>
#include <QListWidget>

namespace Ui {
class OnlineUser;
}

class OnlineUser : public QWidget
{
    Q_OBJECT

public:
    explicit OnlineUser(QWidget *parent = nullptr);
    ~OnlineUser();
    void updateOnlineUserWidgetList(QStringList userList);
private slots:

    void on_onlineUser_LW_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::OnlineUser *ui;
};

#endif // ONLINEUSER_H
