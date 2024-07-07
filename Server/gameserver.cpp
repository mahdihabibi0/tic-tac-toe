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

    //gsm1 signals
    QObject::connect(gsm1,SIGNAL(username_setted(QString)),this,SLOT(check_user_name(QString)));

    QObject::connect(gsm1,SIGNAL(noChanceForWin()),this,SLOT(checkForGameEqualed()));

    QObject::connect(gsm1,SIGNAL(playerWin(QString)),this,SLOT(player1Win(QString)));

    QObject::connect(gsm1,SIGNAL(disconnect(QString)),this,SLOT(socket_disconnected_handler(QString)));

    QObject::connect(gsm1,SIGNAL(player_answered_true(std::pair)),gsm2,SLOT(challanger_answered_true(std::pair)));

    QObject::connect(gsm1,SIGNAL(player_set_button_normal(std::pair)),gsm2,SLOT(challanger_set_button_back_to_normal(std::pair)));

    QObject::connect(gsm1,SIGNAL(player_answering(std::pair)),gsm2,SLOT(challanger_answering(std::pair)));

    //gsm2 signals
    QObject::connect(gsm2,SIGNAL(username_setted(QString)),this,SLOT(check_user_name(QString)));

    QObject::connect(gsm2,SIGNAL(noChanceForWin()),this,SLOT(checkForGameEqualed()));

    QObject::connect(gsm2,SIGNAL(playerWin(QString)),this,SLOT(player2Win(QString)));

    QObject::connect(gsm2,SIGNAL(disconnect(QString)),this,SLOT(socket_disconnected_handler(QString)));

    QObject::connect(gsm2,SIGNAL(player_answered_true(std::pair)),gsm1,SLOT(challanger_answered_true(std::pair)));

    QObject::connect(gsm2,SIGNAL(player_set_button_normal(std::pair)),gsm1,SLOT(challanger_set_button_back_to_normal(std::pair)));

    QObject::connect(gsm2,SIGNAL(player_answering(std::pair)),gsm1,SLOT(challanger_answering(std::pair)));

}

bool GameServer::requestForBackingToGame(QString username)
{
    for (auto i = disconnectedPlayers.begin(); i != disconnectedPlayers.end(); ++i)
        if(username == i.key())
        {
            PlayersWaiting.push_back(i.key());

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

int GameServer::intrequest_for_rem_time_of_game(QString username)
{
    auto fined = disconnectedPlayers.find(username);

    if(fined == disconnectedPlayers.end())
        return 0;

    return fined.value()->get_rem_time();
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
    if(!PlayersWaiting.contains(username))
        return false;

    for (auto i = PlayersWaiting.begin(); i != PlayersWaiting.end(); ++i)
        if(*i == username)
        {
            disconnectedPlayers.erase(disconnectedPlayers.find(username));
            break;
        }

    return true;
}

void GameServer::newConnectionHandler()
{
    QTcpSocket* newSocket = nextPendingConnection();

    if(!gsm1->setSocket(newSocket))
        if(!gsm2->setSocket(newSocket))
            newSocket->disconnect();

}

void GameServer::socket_disconnected_handler(QString username)
{
    Timer* t = new Timer(20);

    disconnectedPlayers.insert(username , t);

    QObject::connect(t , &Timer::time_finished , [&]() {
        delete t;
        for (auto i = disconnectedPlayers.begin(); i != disconnectedPlayers.end() ; ++i)
            if(i.key() == username)
            {
                disconnectedPlayers.erase(i);
                break;
            }
    });
}
