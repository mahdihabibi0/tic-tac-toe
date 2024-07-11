#ifndef GAMESOCKETMANAGER_H
#define GAMESOCKETMANAGER_H
#include <QTcpSocket>
#include <QObject>
#include <QJsonObject>
#include "mapsituations.h"

class GameSocketManager : public QObject
{
    Q_OBJECT
public:
    GameSocketManager();

    bool getChanceForWin();

    bool setSocket(QTcpSocket *socket);

    QString get_username();

    void start_game(int timer, QVector<QVector<MapItem>> MapStates , QString ChallengerName);

    bool getActive();

    void send_win();

    void send_lose();

    void send_game_equal();

    void finish_the_game();
private slots:
    void read_handler();

    void thereIsNoChanceForWin_handler();

    void disconnected_handler();

private:
    QString username;

    QTcpSocket* socket;

    MapSituations map;

    bool chanceForWin;

    void get_new_question_from_http(int);

    QJsonArray get_map(QVector<QVector<MapItem> > MapStates);
    //this value is false when socket is null or disconnected
    bool Active;

    QVector<QVector<MapItem>> mapstates;
signals:
    bool send_to_challenger_win();

    bool checkForDrwed();

    void player_answered_true(QPair<int,int>);

    void player_set_button_normal(QPair<int,int>);

    void player_answering(QPair<int,int>);

    bool username_setted(QString username);

    void disconnected(QString username);

    void send_username();

    void playerWin();

    void playerLose();

    void game_finished();
public slots:

    void challanger_answered_true(QPair<int,int>);

    void challanger_answering(QPair<int,int>);

    void challanger_set_button_back_to_normal(QPair<int,int>);


};

#endif // GAMESOCKETMANAGER_H
