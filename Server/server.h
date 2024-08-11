#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QWidget>
#include <QTcpSocket>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Server; }
QT_END_NAMESPACE

class Server : public QWidget
{
    Q_OBJECT

public:
    static Server& getinstance();
    ~Server();
    Server(QWidget *parent = nullptr);
    void loadConfig();
    void connectHostSucessed();
    QString & get_RootPath();
private:
    Ui::Server *ui;
    QString m_strRootPath;
    QString m_strIp ;
    qint16 m_usPort ;
    QTcpSocket m_tcpSocket;

};
#endif // SERVER_H
