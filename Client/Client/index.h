#ifndef INDEX_H
#define INDEX_H

#include <QWidget>
#include"file.h"
#include"friend.h"
namespace Ui {
class Index;
}

class Index : public QWidget
{
    Q_OBJECT

public:
    static Index&getinstence();
    ~Index();
    Friend * getFriend();
    File * getFile();

private slots:
    void on_friend_PB_clicked();

    void on_file_PB_clicked();

private:
    Ui::Index *ui;
    explicit Index(QWidget *parent = nullptr);
    Index(const Index & other)=delete ;
    Index&operator=(const Index &)=delete;

};

#endif // INDEX_H
