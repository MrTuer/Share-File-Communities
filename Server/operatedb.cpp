#include "operatedb.h"

OperateDB &OperateDB::getinstance()
{
    static OperateDB instance;
    return instance;
}

void OperateDB::connect()
{
    m_db.setHostName("localhost");
    m_db.setPort(3306);
    m_db.setUserName("root");
    m_db.setPassword("123456");
    m_db.setDatabaseName("WP");
    if(m_db.open()){
        qDebug()<<"连接SQL成功";
    }else{
        qDebug()<<"连接失败 because:"<<m_db.lastError().text();
    }
}

bool OperateDB::handleRegist(char *Name, char *Pwd)
{
    if(Name==NULL||Pwd==NULL){
        return false;
    }
    QString sql = QString("select * from user_info where name='%1';").arg(Name);
    QSqlQuery q;
    if(!q.exec(sql)||q.next()){
        return false;
    }
    sql = QString("insert into user_info(name,pwd) values('%1','%2');").arg(Name).arg(Pwd);
    return q.exec(sql);
}

bool OperateDB::handleLogin(char *Name, char *Pwd)
{
    if(Name==NULL||Pwd==NULL){
        return false;
    }
    QString sql = QString("select * from user_info where name='%1' and pwd='%2' and online =0;").arg(Name).arg(Pwd);
    QSqlQuery q;
    if(!q.exec(sql)||!q.next()){
        return false;
    }
    sql = QString("update user_info set online =1 where name = '%1' and pwd='%2';").arg(Name).arg(Pwd);

    return q.exec(sql);
}

int OperateDB::handleaddfriend(char *CurName, char *TarName)
{
    if(CurName==NULL||TarName==NULL){
        return -1;
    }
    QString sql = QString(R"(
                          select * from friend
                          where
                            (user_id =(select id from user_info where name ='%1') and friend_id =(select id from user_info where name ='%2'))
                          or
                            (user_id =(select id from user_info where name ='%3') and friend_id =(select id from user_info where name ='%4'));
                          )"
                          ).arg(CurName).arg(TarName).arg(TarName).arg(CurName);
    QSqlQuery q;
    q.exec(sql);
    if(q.next()){
        return -2;
    }
    sql =QString("select online from user_info where name ='%1'").arg(TarName);
    q.exec(sql);
    if(q.next()){
        qDebug()<<"!q.next()";
        return q.value(0).toInt();

    }
    return -1;
}

int OperateDB::handacceptfriend(char *CurName, char *TarName)
{
    if(CurName==NULL||TarName==NULL){
        return -1;
    }
    QString sql = QString(R"(insert into friend(user_id,friend_id)
                          values((select id from user_info where name ='%1'),
                          (select id from user_info where name ='%2'));)").arg(CurName).arg(TarName);
    QSqlQuery q;
    return q.exec(sql);
}

void OperateDB::handleOffline(const char *name)
{
    if(name == NULL){
        qDebug()<<"handleOffline error";
        return;
    }
    QString sql = QString("update user_info set online =0 where name='%1';").arg(name);
    QSqlQuery q;
    q.exec(sql);
}

QStringList OperateDB::hand_online_users()
{
    QSqlQuery q;
    QStringList name;
    QString sql =QString("select name from user_info where online =1");
    q.exec(sql);
    while(q.next()){
        name.append(q.value(0).toString());
    }
    return name;
}

QStringList OperateDB::handleOnline1Friend(char *CurName)
{
    QStringList res;
    if(CurName == NULL){
        qDebug()<<"handleOffline error";
        return res;
    }
    QSqlQuery q;

    QString sql =QString(R"(
                         select name from user_info where id in (
                         select user_id from friend where friend_id = (select id from user_info where name ='%1')
                         union
                         select friend_id from friend where user_id = (select id from user_info where name ='%1')
                         ) and online =1;
                         ;)").arg(CurName);
    q.exec(sql);
    while(q.next()){
        res.append(q.value(0).toString());
    }
    return res;
}

OperateDB::~OperateDB()
{
    m_db.close();
}

int OperateDB::hand_select_user(QString name)
{
    if(name.isEmpty()){
        qDebug()<<"name.isEmpty()";
        return -1;
    }
    QString sql =QString("select online from user_info where name = '%1'").arg(name);
    QSqlQuery q;
    q.exec(sql);
    if(!q.next()){
        qDebug()<<"!q.next()";
        return -1;
    }
    return q.value(0).toInt();
}

bool OperateDB::handledelete_friend(char *CurName, char *TarName)
{
    if(CurName==NULL||TarName==NULL){
        return false;
    }
    QString sql = QString(R"(
                          select * from friend
                          where
                            (user_id =(select id from user_info where name ='%1') and friend_id =(select id from user_info where name ='%2'))
                          or
                            (user_id =(select id from user_info where name ='%3') and friend_id =(select id from user_info where name ='%4'));
                          )"
                          ).arg(CurName).arg(TarName).arg(TarName).arg(CurName);
    QSqlQuery q;
    q.exec(sql);
    if(!q.next()){
        return false;
    }
    sql = QString(R"(
                  DELETE FROM friend
                  WHERE
                      (user_id = (SELECT id FROM user_info WHERE name = '%1') AND friend_id = (SELECT id FROM user_info WHERE name = '%2'))
                      OR
                      (user_id = (SELECT id FROM user_info WHERE name = '%3') AND friend_id = (SELECT id FROM user_info WHERE name = '%4'));
                  )"
                  ).arg(CurName).arg(TarName).arg(TarName).arg(CurName);
    return q.exec(sql);
}

OperateDB::OperateDB(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
}
