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

    QObject::connect(gsm1,SIGNAL(player_answered_true(QJsonObject)),gsm2,SLOT(challanger_answered_true(QJsonObject)));

    QObject::connect(gsm1,SIGNAL(player_answering(QJsonObject)),gsm2,SLOT(challanger_answering(QJsonObject)));

    QObject::connect(gsm1,SIGNAL(player_answering_false(QJsonObject)),gsm2,SLOT(challanger_answered_false(QJsonObject)));




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

void GameServer::checkForGameEqualed()
{
    if(!(gsm1->getChanceForWin() || gsm1->getChanceForWin()))
        gameFinished = true;
}

void GameServer::player1Win(QString name)
{
    //do some thing
}

void GameServer::player2Win(QString name)
{
    //do some thing
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
