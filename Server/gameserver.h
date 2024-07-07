#ifndef GAMESERVER_H
#define GAMESERVER_H
#include <QTcpServer>
#include "gamesocketmanager.h"

class GameServer : public QTcpServer
{
    Q_OBJECT
public:
    GameServer(QHostAddress ip);

    bool requestForBackingToGame(QString username);

    bool requestForNewConection(QString username);

private:
    GameSocketManager* gsm1;

    GameSocketManager* gsm2;

    bool gameFinished;

    QVector<QString> PlayersWaiting;

    QVector<QString> disconnectedPlayers;
private slots:
    void checkForGameEqualed();

    void player1Win(QString name);

    void player2Win(QString name);

    bool check_user_name(QString username);

    void newConnectionHandler();
};

#endif // GAMESERVER_H
