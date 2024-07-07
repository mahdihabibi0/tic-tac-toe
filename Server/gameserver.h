#ifndef GAMESERVER_H
#define GAMESERVER_H
#include <QTcpServer>
#include "doozemap.h"
#include "gamesocketmanager.h"

class GameServer : public QTcpServer
{
    Q_OBJECT
public:
    GameServer(QHostAddress ip);

    bool requestForNewConection(QString username);
private:
    GameSocketManager* gsm1;

    GameSocketManager* gsm2;

    bool gameFinished;

private slots:
    void checkForGameEqualed();

    void player1Win(QString name);

    void player2Win(QString name);

    void newConnectionHandler();
};

#endif // GAMESERVER_H
