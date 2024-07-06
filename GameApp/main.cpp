#include "game.h"
#include "tcpsocketmanager.h"
#include "homepage.h"
#include "lsmanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TCPSocketManager s;
    // Game g;
    HomePage h;
    LSManager ls;

    QObject::connect(&ls , SIGNAL(defult_login_signal(QJsonObject)) , &s , SLOT(try_to_login_handler(QJsonObject)));
    QObject::connect(&ls.l , SIGNAL(try_to_login(QJsonObject)) , &s , SLOT(try_to_login_handler(QJsonObject)));
    QObject::connect(&ls.s , SIGNAL(try_to_signup(QJsonObject)) , &s , SLOT(try_to_signup_handler(QJsonObject)));
    QObject::connect(&ls.l , SIGNAL(login_successfully()) , &h , SLOT(show()));
    QObject::connect(&ls.l , SIGNAL(get_user_information(QString)) , &s , SLOT(get_user_information(QString)));
    QObject::connect(&h , SIGNAL(logout()) , &ls.l , SLOT(show()));
    ls.manage();
    return a.exec();
}
