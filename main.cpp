#include "game.h"
#include "tcpsocketmanager.h"
#include "homepage.h"
#include "lsmanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TCPSocketManager s;
    Game g;
    HomePage h;
    LSManager ls;

    return a.exec();
}
