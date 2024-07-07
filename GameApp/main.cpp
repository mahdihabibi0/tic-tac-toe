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

    QObject::connect(&ls , SIGNAL(defult_login_signal(QJsonObject)) , &s , SLOT(try_to_defult_login_handler(QJsonObject)));

    QObject::connect(&ls.l , SIGNAL(try_to_login(QJsonObject)) , &s , SLOT(try_to_login_handler(QJsonObject)));

    QObject::connect(&ls.s , SIGNAL(try_to_signup(QJsonObject)) , &s , SLOT(try_to_signup_handler(QJsonObject)));

    QObject::connect(&ls.l , SIGNAL(login_successfully()) , &h , SLOT(show()));

    QObject::connect(&ls.l , SIGNAL(get_user_information(QString)) , &s , SLOT(get_user_information(QString)));

    QObject::connect(&h , SIGNAL(logout()) , &ls.l , SLOT(show()));

    QObject::connect(&g , SIGNAL(answered_true(int,int)) , &s , SLOT(subserver_palayer_answered_true_process(int , int)));

    QObject::connect(&g , SIGNAL(answered_false(int,int)) , &s , SLOT(subserver_palayer_answered_false_process(int,int)));

    QObject::connect(&g , SIGNAL(get_question(QuestionType)) , &s , SLOT(get_question_by_type(QuestionType)));

    QObject::connect(&g , SIGNAL(is_answering(int,int)) , &s , SLOT(subserver_player_is_answering_process(int ,int)));

    QObject::connect(&g , SIGNAL(set_back_normal(int,int)) , &s , SLOT(subserver_player_set_back_to_normal(int ,int)));

    ls.manage();
    return a.exec();
}
