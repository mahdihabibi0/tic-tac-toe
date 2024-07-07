#ifndef GAMESERVER_H
#define GAMESERVER_H
#include <QTcpServer>
#include "gamesocketmanager.h"
#include <QMap>
#include "timer.h"

class GameServer : public QTcpServer
{
    Q_OBJECT
public:
    GameServer(QHostAddress ip);

    bool requestForBackingToGame(QString username);

    bool requestForNewConection(QString username);

    int  intrequest_for_rem_time_of_game(QString username);
private:
    GameSocketManager* gsm1;

    GameSocketManager* gsm2;

    bool gameFinished;

    QVector<QString> PlayersWaiting;

    QMap<QString , Timer*> disconnectedPlayers;
private slots:
    void checkForGameEqualed();

    void player1Win(QString name);

    void player2Win(QString name);

    bool check_user_name(QString username);

    void newConnectionHandler();

    void socket_disconnected_handler(QString username);
};

#endif // GAMESERVER_H
