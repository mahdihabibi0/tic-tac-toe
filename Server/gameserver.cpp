#include <QSignalSpy>
#include "gameserver.h"

GameServer::GameServer(QHostAddress ip) :
    gsm1(new GameSocketManager())  ,
    gsm2(new GameSocketManager())  ,
    countOfPlayer(0)
{
    for (int i = 5000; i < 65535; ++i) {
        if(this->listen(ip,i)){
            break;
        }
    }

    qDebug() << "game server created :" <<this->serverAddress() << " : " << this->serverPort();

    QObject::connect(this , SIGNAL(newConnection()) , this , SLOT(newConnectionHandler()));

    QObject::connect(this,&GameServer::ready_for_start_the_game,[&](){
        gsm1->start_game(gsm2->get_username());
        gsm2->start_game(gsm1->get_username());
    });

    //gsm1 signals
    QObject::connect(gsm1,SIGNAL(username_setted(QString)),this,SLOT(check_user_name(QString)));

    QObject::connect(gsm1,SIGNAL(noChanceForWin()),this,SLOT(checkForGameEqualed()));

    QObject::connect(gsm1,SIGNAL(playerWin(QString)),this,SLOT(player1Win(QString)));

    QObject::connect(gsm1,SIGNAL(disconnect(QString)),this,SLOT(socket_disconnected_handler(QString)));

    QObject::connect(gsm1,SIGNAL(player_answered_true(QPair<int,int>)),gsm2,SLOT(challanger_answered_true(QPair<int,int>)));

    QObject::connect(gsm1,SIGNAL(player_set_button_normal(QPair<int,int>)),gsm2,SLOT(challanger_set_button_back_to_normal(QPair<int,int>)));

    QObject::connect(gsm1,SIGNAL(player_answering(QPair<int,int>)),gsm2,SLOT(challanger_answering(QPair<int,int>)));

    //gsm2 signals
    QObject::connect(gsm2,SIGNAL(username_setted(QString)),this,SLOT(check_user_name(QString)));

    QObject::connect(gsm2,SIGNAL(noChanceForWin()),this,SLOT(checkForGameEqualed()));

    QObject::connect(gsm2,SIGNAL(playerWin(QString)),this,SLOT(player2Win(QString)));

    QObject::connect(gsm2,SIGNAL(disconnect(QString)),this,SLOT(socket_disconnected_handler(QString)));

    QObject::connect(gsm2,SIGNAL(player_answered_true(QPair<int,int>)),gsm1,SLOT(challanger_answered_true(QPair<int,int>)));

    QObject::connect(gsm2,SIGNAL(player_set_button_normal(QPair<int,int>)),gsm1,SLOT(challanger_set_button_back_to_normal(QPair<int,int>)));

    QObject::connect(gsm2,SIGNAL(player_answering(QPair<int,int>)),gsm1,SLOT(challanger_answering(QPair<int,int>)));

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
    if(disconnectedPlayers.size())
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

int GameServer::request_for_rem_time_of_game(QString username)
{
    auto fined = disconnectedPlayers.find(username);

    if(fined == disconnectedPlayers.end())
        return 0;

    return fined.value()->get_rem_time();
}

void GameServer::addToWaiters(QString username)
{
    PlayersWaiting.push_back(username);
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
            PlayersWaiting.erase(i);
            break;
        }

    countOfPlayer++;

    if(countOfPlayer==2)
        emit ready_for_start_the_game();

    return true;
}

void GameServer::newConnectionHandler()
{
    QTcpSocket* newSocket = nextPendingConnection();

    qDebug() << "new game socket : " << newSocket->localAddress() << " : " << newSocket->localPort();

    if(!gsm1->setSocket(newSocket))
        if(!gsm2->setSocket(newSocket)){
            newSocket->disconnect();
            return;
        }


}

void GameServer::socket_disconnected_handler(QString username)
{
    Timer* t = new Timer(20);

    disconnectedPlayers.insert(username , t);

    QObject::connect(t , &Timer::time_finished , [&]() {
        for (auto i = disconnectedPlayers.begin(); i != disconnectedPlayers.end() ; ++i)
            if(i.key() == username)
            {
                disconnectedPlayers.erase(i);
                break;
            }

        t->deleteLater();

    });
}
