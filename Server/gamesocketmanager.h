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

    void send_loose();

    void send_game_equal();

private slots:
    void read_handler();

    void thereIsNoChanceForWin_handler();

    void win_handler();

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
signals:
    void player_answered_true(QPair<int,int>);

    void player_set_button_normal(QPair<int,int>);

    void player_answering(QPair<int,int>);

    void noChanceForWin();

    void playerWin(QString name);

    bool username_setted(QString username);

    void disconnected(QString username);

    void send_username();
public slots:

    void challanger_answered_true(QPair<int,int>);

    void challanger_answering(QPair<int,int>);

    void challanger_set_button_back_to_normal(QPair<int,int>);


};

#endif // GAMESOCKETMANAGER_H
