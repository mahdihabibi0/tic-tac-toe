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

    bool requestForNewConection();
private:
    GameSocketManager* gsm1;

    DoozeMap map1;

    GameSocketManager* gsm2;

    DoozeMap map2;
private slots:
    void newConnectionHandler();
};

#endif // GAMESERVER_H
