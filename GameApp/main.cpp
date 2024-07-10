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

    QObject::connect(&ls , SIGNAL(defult_login_signal(QJsonObject)) , &s , SLOT(try_to_default_login_handler(QJsonObject)));

    QObject::connect(&ls.l , SIGNAL(try_to_login(QJsonObject)) , &s , SLOT(try_to_login_handler(QJsonObject)));

    QObject::connect(&ls.s , SIGNAL(try_to_signup(QJsonObject)) , &s , SLOT(try_to_signup_handler(QJsonObject)));

    QObject::connect(&ls.l , SIGNAL(login_successfully()) , &h , SLOT(show()));

    QObject::connect(&ls.l , SIGNAL(get_user_information(QString)) , &s , SLOT(get_user_information(QString)));

    QObject::connect(&h , SIGNAL(logout()) , &ls.l , SLOT(show()));

    QObject::connect(&h , SIGNAL(logout()) , &s , SLOT(user_is_offline()));

    QObject::connect(&h , SIGNAL(get_statement(QString)) , &s , SLOT(get_player_statement(QString)));

    QObject::connect(&h , SIGNAL(try_to_start_new_game(QString)) , &s , SLOT(try_to_start_game(QString)));

    QObject::connect(&h , SIGNAL(show_game_page(QJsonObject)) , &g , SLOT(start(QJsonObject)));

    QObject::connect(&s , SIGNAL(startGame(QJsonObject)) , &h , SLOT(start_game(QJsonObject)));

    QObject::connect(&g , SIGNAL(answered_true(int,int)) , &s , SLOT(subserver_palayer_answered_true_process(int , int)));

    QObject::connect(&g , SIGNAL(answered_false(int,int)) , &s , SLOT(subserver_palayer_answered_false_process(int,int)));

    QObject::connect(&g , SIGNAL(get_question(QuestionType)) , &s , SLOT(get_question_by_type(QuestionType)));

    QObject::connect(&g , SIGNAL(is_answering(int,int)) , &s , SLOT(subserver_player_is_answering_process(int ,int)));

    QObject::connect(&g , SIGNAL(set_back_normal(int,int)) , &s , SLOT(subserver_player_set_back_to_normal(int ,int)));

    QObject::connect(&s,SIGNAL(set_button_situation(int,int,Situation)),&g,SLOT(set_button_situation(int,int,Situation)));

    QObject::connect(&s,SIGNAL(game_drawed()),&g,SLOT(game_drawed()));

    QObject::connect(&s,SIGNAL(player_won()),&g,SLOT(player_won()));

    QObject::connect(&s,SIGNAL(player_lose()),&g,SLOT(player_lose()));



    ls.manage();
    return a.exec();
}
