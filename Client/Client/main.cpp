#include "client.h"
#include "index.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Client::getinstance().show();
    return app.exec();
}
