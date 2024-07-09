#include <QSignalSpy>
#include "gameserver.h"

int GameServer::idGen = 0;

GameServer::GameServer(QHostAddress ip) :
    id(idGen++)   ,
    countOfPlayer(0) ,
    gsm1(new GameSocketManager())  ,
    gsm2(new GameSocketManager())
{

    for (int i = 5000; i < 65535; ++i) {
        if(this->listen(ip,i)){
            break;
        }
    }

    qDebug() << "game server created :" <<this->serverAddress() << " : " << this->serverPort();

    QObject::connect(this , SIGNAL(newConnection()) , this , SLOT(newConnectionHandler()));

    QObject::connect(this,&GameServer::ready_for_start_the_game,[&](){
        gsm1->start_game(mapStates.getAllMap() , gsm2->get_username());

        gsm2->start_game(mapStates.getAllMap() , gsm1->get_username());
    });

    //game socket manager 1 signals
    QObject::connect(gsm1,SIGNAL(username_setted(QString)),this,SLOT(check_user_name(QString)));

    QObject::connect(gsm1,SIGNAL(noChanceForWin()),this,SLOT(checkForGameEqualed()));

    QObject::connect(gsm1,SIGNAL(playerWin(QString)),this,SLOT(player1Win(QString)));

    QObject::connect(gsm1,SIGNAL(disconnected(QString)),this,SLOT(socket_disconnected_handler(QString)));

    QObject::connect(gsm1,SIGNAL(player_answered_true(QPair<int,int>)),gsm2,SLOT(challanger_answered_true(QPair<int,int>)));

    QObject::connect(gsm1,SIGNAL(player_set_button_normal(QPair<int,int>)),gsm2,SLOT(challanger_set_button_back_to_normal(QPair<int,int>)));

    QObject::connect(gsm1,SIGNAL(player_answering(QPair<int,int>)),gsm2,SLOT(challanger_answering(QPair<int,int>)));

    QObject::connect(gsm1 , SIGNAL(get_map_states_from_game_server()) , this , SLOT(take_map_states_from_game_server()));

    //game socket manager 2 signals
    QObject::connect(gsm2,SIGNAL(username_setted(QString)),this,SLOT(check_user_name(QString)));

    QObject::connect(gsm2,SIGNAL(noChanceForWin()),this,SLOT(checkForGameEqualed()));

    QObject::connect(gsm2,SIGNAL(playerWin(QString)),this,SLOT(player2Win(QString)));

    QObject::connect(gsm2,SIGNAL(disconnected(QString)),this,SLOT(socket_disconnected_handler(QString)));

    QObject::connect(gsm2,SIGNAL(player_answered_true(QPair<int,int>)),gsm1,SLOT(challanger_answered_true(QPair<int,int>)));

    QObject::connect(gsm2,SIGNAL(player_set_button_normal(QPair<int,int>)),gsm1,SLOT(challanger_set_button_back_to_normal(QPair<int,int>)));

    QObject::connect(gsm2,SIGNAL(player_answering(QPair<int,int>)),gsm1,SLOT(challanger_answering(QPair<int,int>)));

    QObject::connect(gsm2 , SIGNAL(get_map_states_from_game_server()) , this , SLOT(take_map_states_from_game_server()));


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

//with out impeliment
void GameServer::checkForGameEqualed()
{
    if(!(gsm1->getChanceForWin() || gsm1->getChanceForWin()))
        qDebug() << "game is equal";
}
//with out impeliment
void GameServer::player1Win(QString name)
{
    qDebug() << "player " << name <<" win";
}
//with out impeliment
void GameServer::player2Win(QString name)
{
    qDebug() << "player " << name <<" win";
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
    qDebug()<<"--check for subserver to delete-- :" << id;
    if(!(gsm1->getActive() || gsm2->getActive()))
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

        t->deleteLater();

        if(gsm1->getActive())
            emit gsm1->playerWin(gsm1->get_username());

        if(gsm2->getActive())
            emit gsm2->playerWin(gsm2->get_username());

        qDebug()<<"--delete subserver-- :" << id;

        this->deleteLater();

        qDebug()<<"pair of " << username << " is deleted now";
    });
}

QVector<QVector<MapItem>> GameServer::take_map_states_from_game_server()
{
    return mapStates.getAllMap();
}
