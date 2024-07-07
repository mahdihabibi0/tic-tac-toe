#include <QSignalSpy>
#include "gameserver.h"

GameServer::GameServer(QHostAddress ip) :
    gsm1(new GameSocketManager())  ,
    gsm2(new GameSocketManager())
{
    for (int i = 5000; i < 65535; ++i) {
        if(this->listen(ip,i)){
            qDebug() << "connected";
            break;
        }
    }

    qDebug() << "game server created :" <<this->serverAddress() << " : " << this->serverPort();

    QObject::connect(this , SIGNAL(newConnection()) , this , SLOT(newConnectionHandler()));

    QObject::connect(gsm1,SIGNAL(player_answered_true(QJsonObject)),gsm2,SLOT(challanger_answered_true(QJsonObject)));

    QObject::connect(gsm1,SIGNAL(player_answering(QJsonObject)),gsm2,SLOT(challanger_answering(QJsonObject)));

    QObject::connect(gsm1,SIGNAL(player_answering_false(QJsonObject)),gsm2,SLOT(challanger_answered_false(QJsonObject)));




}

bool GameServer::requestForBackingToGame(QString username)
{
    for (auto i = disconnectedPlayers.begin(); i != disconnectedPlayers.end(); ++i)
        if(username == (*i))
        {
            PlayersWaiting.push_back(*i);

            disconnectedPlayers.erase(i);

            return true;
        }

    return false;
}

bool GameServer::requestForNewConection(QString username)
{
    if(!disconnectedPlayers.size())
        return false;

    QSignalSpy spy(this, SIGNAL(newConnection()));

    int count = spy.count();



    if(count >= 2)
    {
        return false;
    }
    else
    {
        PlayersWaiting.push_back(username);
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

bool GameServer::check_user_name(QString username)
{
    if(PlayersWaiting.contains(username))
        return true;
    else
        return false;
}

void GameServer::newConnectionHandler()
{
    QTcpSocket* newSocket = nextPendingConnection();

    if(!gsm1->setSocket(newSocket))
        if(!gsm2->setSocket(newSocket))
            newSocket->disconnect();
}
