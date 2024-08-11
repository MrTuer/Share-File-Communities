#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>
#include "onlineuser.h"
#include "chat.h"
namespace Ui {
class Friend;
}

class Friend : public QWidget
{
    Q_OBJECT

public:
    explicit Friend(QWidget *parent = nullptr);
    OnlineUser * getOnlineUser();
    Chat * getChat();
    ~Friend();
    void flushFriend();
    void updateFriendLW(QStringList friendList);

    QListWidget *getFriendListWidget();

public slots:

    void on_findUser_PB_clicked();

    void on_onlineUser_PB_clicked();

    void on_flushFriend_PB_clicked();

    void on_delFriend_PB_clicked();

    void on_chat_PB_clicked();

private:
    Ui::Friend *ui;
    OnlineUser * m_pOnlineUser;
    Chat * m_pChat;
};

#endif // FRIEND_H
