#ifndef IMDEX_H
#define IMDEX_H

#include <QWidget>

namespace Ui {
class Imdex;
}

class Imdex : public QWidget
{
    Q_OBJECT

public:
    explicit Imdex(QWidget *parent = nullptr);
    ~Imdex();

private slots:
    void on_friend_PB_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Imdex *ui;
};

#endif // IMDEX_H
