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

    DoozeMap map1;

    bool map1Chance;

    GameSocketManager* gsm2;

    DoozeMap map2;

    bool map2Chance;

    bool gameFinished;

private slots:
    void player1Win();

    void player2Win();

    void thereIsNoChancePlayer1();

    void thereIsNoChancePlayer2();

    void setMap1SituationAtPosition(int i , int j , Situation s);

    void setMap2SituationAtPosition(int i , int j , Situation s);

    void newConnectionHandler();
};

#endif // GAMESERVER_H
