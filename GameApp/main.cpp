#include "game.h"
#include "tcpsocketmanager.h"
#include "homepage.h"
#include "lsmanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Game c;
    c.show();
    //c.exec();
    //c.show();
    // TCPSocketManager s;
    // Game g;
    // HomePage h;
    // LSManager ls;

    // QObject::connect(&ls , SIGNAL(defult_login_signal(QJsonObject)) , &s , SLOT(try_to_login(QJsonObject)));
    // QObject::connect(&ls.l , SIGNAL(try_to_login(QJsonObject)) , &s , SLOT(try_to_login(QJsonObject)));
    // QObject::connect(&ls.s , SIGNAL(try_to_signup(QJsonObject)) , &s , SLOT(try_to_signup(QJsonObject)));
    return a.exec();
}
