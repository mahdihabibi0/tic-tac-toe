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

enum CommandOfSubServer{
    setButtonToAnsweringByOpponent,
    setButtonToNormalByOpponent,
    setButtonToAnsweredByOpponent,
    startTheGame,
    newQuestion
};

class TCPSocketManager : public QTcpSocket
{
    Q_OBJECT

    QMessageBox waitingForServerConnection;

public:
    TCPSocketManager();

private slots:
    void subserver_read_handeler();

    void set_button_situation_handeler(QJsonObject obj , Situation s);

public slots:
    bool try_to_login(QJsonObject &user );

    bool try_to_signup(QJsonObject &user);

    bool try_to_start_game();

    void connected_to_server();

    void log_out();

    QJsonObject get_user_information(QString userName);

    void subserver_palayer_answerd_process(int i , int j);

    void subserver_player_is_answering_process(int i , int j);

    QJsonObject get_question_by_type(QuestionType type);
signals:
    void new_question_taken(QJsonObject Qobj);

    void server_is_online();

    void set_button_situation(int i,int j,Situation s);

    void startGame();

private:
    QMap<QString , CommandOfSubServer> commands;

};

#endif // TCPSOCKETMANAGER_H
