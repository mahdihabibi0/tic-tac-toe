#ifndef TCPSOCKETMANAGER_H
#define TCPSOCKETMANAGER_H
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QMessageBox>
#include <QMap>
#include "gamebutton.h"
#include "getippage.h"


enum CommandOfSubServer{
    setButtonToAnsweringByOpponent,
    setButtonToNormalByOpponent,
    setButtonToAnsweredByOpponent,
    startTheGame,
    newQuestion,
    playerWon,
    playerLose,
    gameِِDrawed
};

class TCPSocketManager : public QTcpSocket
{
    Q_OBJECT

    QMessageBox waitingForServerConnection;

public:
    TCPSocketManager();

    void connect_to_base_server();

private slots:
    void subserver_read_handeler();

    void set_button_situation_handeler(QJsonObject obj , Situation s);

public slots:
    bool try_to_login_handler(QJsonObject user );

    bool try_to_default_login_handler(QJsonObject user);

    bool try_to_signup_handler(QJsonObject user);

    bool try_to_start_game(QString username);

    void connected_to_base_server();

    QJsonObject get_user_information(QString username);

    void subserver_palayer_answered_true_process(int i , int j);

    void subserver_palayer_answered_false_process(int i , int j);

    void subserver_player_is_answering_process(int i , int j);

    void subserver_player_set_back_to_normal(int i,int j);

    QJsonObject get_question_by_type(QuestionType type);

    void close_the_program();

    int get_player_statement(QString username);

    void user_is_offline();

signals:
    void new_question_taken(QJsonObject Qobj);

    void base_server_is_online();

    void set_button_situation(int i,int j,Situation s);

    void startGame(QJsonObject);

    void player_won();

    void player_lose();

    void game_drawed();

private:
    GetIpPage gip;

    QMap<QString , CommandOfSubServer> commands;

};

#endif // TCPSOCKETMANAGER_H
