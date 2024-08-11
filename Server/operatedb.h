#ifndef OPERATEDB_H
#define OPERATEDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QObject>
#include <QObject>
#include <QObject>
#include <QObject>
#include <QObject>
class OperateDB : public QObject
{
    Q_OBJECT
public:
    static OperateDB&getinstance();
    void connect();
    void getconnected();
    bool handleRegist(char *Name,char *Pwd);
    bool handleLogin(char *Name,char *Pwd);
    int handleaddfriend(char *CurName,char *TarName);
    int handacceptfriend(char *CurName,char *TarName);
    void handleOffline(const char *name);
    QStringList hand_online_users();
    QStringList handleOnline1Friend(char *CurName);
    QSqlDatabase m_db;
    ~OperateDB();
    int hand_select_user(QString name);
    bool handledelete_friend(char *CurName,char *TarName);

signals:

private:
    explicit OperateDB(QObject *parent = nullptr);
    OperateDB (const  OperateDB & istence) = delete ;
    OperateDB &operator = (const  OperateDB&)= delete ;
};

#endif // OPERATEDB_H
