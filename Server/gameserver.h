#ifndef GAMESERVER_H
#define GAMESERVER_H
#include <QTcpServer>
#include <QMap>
#include "gamesocketmanager.h"
#include "timer.h"
#include "mapstatements.h"

class GameServer : public QTcpServer
{
    Q_OBJECT
public:
    GameServer(QHostAddress ip);

    bool requestForBackingToGame(QString username);

    bool requestForNewConection(QString username);

    int  request_for_rem_time_of_game(QString username);

    void  waitFor(QString);

    int getId();
private:
    int id;

    static int idGen;

    int countOfPlayer;

    GameSocketManager* gsm1;

    GameSocketManager* gsm2;

    bool gameFinished;

    QVector<QString> PlayersWaiting;

    QMap<QString , Timer*> disconnectedPlayers;

    MapStatements mapStates;

    int mapTime;
private slots:
    void checkForGameEqualed();

    void player1Win(QString name);

    void player2Win(QString name);

    bool check_user_name(QString username);

    void newConnectionHandler();

    void socket_disconnected_handler(QString username);

    QVector<QVector<MapItem> > take_map_states_from_game_server();
signals:
    void GameDestroyed(int id);

    void ready_for_start_the_game();
};

#endif // GAMESERVER_H
