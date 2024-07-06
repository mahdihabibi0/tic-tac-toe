#include <QSignalSpy>
#include "gameserver.h"

GameServer::GameServer(QHostAddress ip) :
    gsm1(NULL)  ,
    gsm2(NULL) ,
    map1Chance(true) ,
    map2Chance(true)
{
    for (int i = 5000; i < 65535; ++i) {
        if(this->listen(ip,i)){
            qDebug() << "connected";
            break;
        }
    }

    qDebug() << "game server created :" <<this->serverAddress() << " : " << this->serverPort();

    QObject::connect(this , SIGNAL(newConnection()) , this , SLOT(newConnectionHandler()));

    QObject::connect(&map1 , SIGNAL(win()) , this , SLOT(player1Win()));

    QObject::connect(&map2 , SIGNAL(win()) , this , SLOT(player2Win()));

    QObject::connect(&map1 , SIGNAL(thereIsNoChanceForWin()) , this , SLOT(thereIsNoChancePlayer1()));

    QObject::connect(&map2 , SIGNAL(thereIsNoChanceForWin()) , this , SLOT(thereIsNoChancePlayer2()));

}

bool GameServer::requestForNewConection(QString username)
{
    QSignalSpy spy(this, SIGNAL(newConnection()));

    int count = spy.count();



    if(count >= 2)
    {
        return false;
    }
    else
    {

        return true;
    }

}

void GameServer::player1Win()
{

}

void GameServer::player2Win()
{

}

void GameServer::thereIsNoChancePlayer1()
{
    this->map1Chance = false;

    gameFinished = map1Chance || map2Chance;
}

void GameServer::thereIsNoChancePlayer2()
{
    this->map2Chance = false;

    gameFinished = map1Chance || map2Chance;
}

void GameServer::setMap1SituationAtPosition(int i, int j, Situation s)
{
    map1.setItemAtPosition(i , j , s);
}

void GameServer::setMap2SituationAtPosition(int i, int j, Situation s)
{
    map2.setItemAtPosition(i , j , s);
}

void GameServer::newConnectionHandler()
{
    if(!gsm1){
        gsm1 = new GameSocketManager(this->nextPendingConnection());
        // QObject::connect(gsm1 , SIGNAL() , this , )
        return;
    }

    if(!gsm2){
        gsm2 = new GameSocketManager(this->nextPendingConnection());

        return;
    }
}
