#include <QSignalSpy>
#include "gameserver.h"
#include "usersHandler.h"

int GameServer::idGen = 0;

GameServer::GameServer(QHostAddress ip) :
    id(idGen++)   ,
    countOfPlayer(0) ,
    gsm1(new GameSocketManager())  ,
    gsm2(new GameSocketManager())   ,
    mapTime(0)
{

    for (int i = 5000; i < 65535; ++i) {
        if(this->listen(ip,i)){
            break;
        }
    }

    qDebug() << "game server created :" <<this->serverAddress() << " : " << this->serverPort();

    QTimer* timer =  new QTimer();

    QObject::connect(timer , &QTimer::timeout , [&](){
        mapTime++;
    });

    timer->start(1000);

    QObject::connect(this , SIGNAL(newConnection()) , this , SLOT(newConnectionHandler()));

    QObject::connect(this,&GameServer::ready_for_start_the_game,[&](){
        gsm1->start_game(mapTime , mapStates.getAllMap() , gsm2->get_username());

        gsm2->start_game(mapTime , mapStates.getAllMap() , gsm1->get_username());
    });

    //game socket manager 1 signals
    QObject::connect(gsm1,SIGNAL(username_setted(QString)),this,SLOT(check_user_name(QString)));

    QObject::connect(gsm2 , &GameSocketManager::checkForDrwed , [&]()->bool{
        if(!(gsm1->getChanceForWin() || gsm2->getChanceForWin()))
            return true;
        return false;
    });

    QObject::connect(gsm2 , &GameSocketManager::send_to_challenger_win , [&]()->void{
        gsm1->send_win();
    });

    QObject::connect(gsm1,SIGNAL(playerWin()),this,SLOT(player1Win()));

    QObject::connect(gsm1,SIGNAL(playerLose()),this,SLOT(player1Lose()));

    QObject::connect(gsm1,SIGNAL(disconnected(QString)),this,SLOT(socket_disconnected_handler(QString)));

    QObject::connect(gsm1,SIGNAL(player_answered_true(QPair<int,int>)),gsm2,SLOT(challanger_answered_true(QPair<int,int>)));

    QObject::connect(gsm1,SIGNAL(player_set_button_normal(QPair<int,int>)),gsm2,SLOT(challanger_set_button_back_to_normal(QPair<int,int>)));

    QObject::connect(gsm1,SIGNAL(player_answering(QPair<int,int>)),gsm2,SLOT(challanger_answering(QPair<int,int>)));

    //game socket manager 2 signals
    QObject::connect(gsm2,SIGNAL(username_setted(QString)),this,SLOT(check_user_name(QString)));

    QObject::connect(gsm1 , &GameSocketManager::checkForDrwed , [&]()->bool{
        if(!(gsm1->getChanceForWin() || gsm2->getChanceForWin()))
            return true;
        return false;
    });

    QObject::connect(gsm1 , &GameSocketManager::send_to_challenger_win , [&]()->void{
        gsm2->send_win();
    });

    QObject::connect(gsm2,SIGNAL(playerWin()),this,SLOT(player2Win()));

    QObject::connect(gsm2,SIGNAL(playerLose()),this,SLOT(player2Lose()));

    QObject::connect(gsm2,SIGNAL(disconnected(QString)),this,SLOT(socket_disconnected_handler(QString)));

    QObject::connect(gsm2,SIGNAL(player_answered_true(QPair<int,int>)),gsm1,SLOT(challanger_answered_true(QPair<int,int>)));

    QObject::connect(gsm2,SIGNAL(player_set_button_normal(QPair<int,int>)),gsm1,SLOT(challanger_set_button_back_to_normal(QPair<int,int>)));

    QObject::connect(gsm2,SIGNAL(player_answering(QPair<int,int>)),gsm1,SLOT(challanger_answering(QPair<int,int>)));


    QObject::connect(this , &GameServer::destroyed , [&](QObject *){
        emit GameDestroyed(id);
    });
}

bool GameServer::requestForBackingToGame(QString username)
{
    for (auto i = disconnectedPlayers.begin(); i != disconnectedPlayers.end(); ++i)
        if(username == i.key())
        {
            PlayersWaiting.push_back(i.key());

            i.value()->deleteLater();

            disconnectedPlayers.erase(i);

            return true;
        }

    return false;
}

bool GameServer::requestForNewConection(QString username)
{
    if(disconnectedPlayers.size() != 0)
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
    {
        return 0;
    }
    return (fined.value()->get_rem_time() > 2) ? (fined.value()->get_rem_time() - 2) : 0;
}

void GameServer::waitFor(QString username)
{
    PlayersWaiting.push_back(username);
}

int GameServer::getId()
{
    return id;
}

void GameServer::checkForGameEqualed()
{
    if(!(gsm1->getChanceForWin() || gsm1->getChanceForWin()))
    {
        gsm1->send_game_equal();

        gsm2->send_game_equal();

        userDrawed(gsm2->get_username(), gsm1->get_username());

        userDrawed(gsm1->get_username(), gsm2->get_username());
    }

}


void GameServer::player1Win()
{
    userWin(gsm1->get_username(), gsm2->get_username());
}

//with out impeliment
void GameServer::player2Win()
{

    userWin(gsm2->get_username(), gsm1->get_username());

}

void GameServer::player1Lose()
{
    userLose(gsm1->get_username(), gsm2->get_username());
}

void GameServer::player2Lose()
{

    userLose(gsm2->get_username(), gsm1->get_username());
}

bool GameServer::check_user_name(QString username)
{
    qDebug() << "check for username " << username <<"that is connected to gameserver";

    if(PlayersWaiting.contains(username) == false)
    {
        return false;
        qDebug() << "   answer is false becuse username not found in PlayersWating";
    }

    for (auto i = PlayersWaiting.begin(); i != PlayersWaiting.end(); ++i)
        if(*i == username)
        {
            PlayersWaiting.erase(i);
            break;
        }

    countOfPlayer++;

    qDebug() << "   now countOfPlayer is " << countOfPlayer;

    if(countOfPlayer == 2)
        emit ready_for_start_the_game();

    qDebug() << "   answer is true";
    return true;
}

//handlering the new connection to game server
void GameServer::newConnectionHandler()
{
    QTcpSocket* newSocket = nextPendingConnection();

    //fiil the game socket manager with new client
    if(!gsm1->setSocket(newSocket))
        if(!gsm2->setSocket(newSocket)){
            newSocket->disconnect();
            return;
        }

    qDebug() << "new client connect to game server : " << newSocket->localAddress() << " : " << newSocket->localPort();
}

void GameServer::socket_disconnected_handler(QString username)
{
    countOfPlayer--;

    changeUserSit(username , false);

    qDebug()<<"--check for subserver to delete-- :" << id;
    if(countOfPlayer == 0)
    {
        qDebug()<<"--delete subserver-- :" << id;

        for (auto pair = disconnectedPlayers.begin(); pair != disconnectedPlayers.end(); ++pair) {
            qDebug()<<"--delete " << pair.key() << " in disconnected players";
            pair.value()->deleteLater();
        }

        this->deleteLater();

        return;
    }

    qDebug()<<"make the pair of disconnected player : " << username;
    Timer* t = new Timer(20);

    disconnectedPlayers.insert(username , t);

    qDebug()<<"pair added to disconnected players : " << username;

    QObject::connect(t , &Timer::time_finished , [&]() {
        qDebug()<<"time for backing " << username << " is finished";

        for (auto i = disconnectedPlayers.begin(); i != disconnectedPlayers.end() ; ++i)
            if(i.key() == username)
            {
                disconnectedPlayers.erase(i);

                break;
            }

        if(gsm1->getActive())
            emit gsm1->send_win();

        if(gsm2->getActive())
            emit gsm2->send_win();

        qDebug()<<"--delete subserver-- :" << id;

        this->deleteLater();

        qDebug()<<"pair of " << username << " is deleted now";
    });
}

QVector<QVector<MapItem>> GameServer::take_map_states_from_game_server()
{
    return mapStates.getAllMap();
}
