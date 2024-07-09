#include "baseserver.h"
#include "serversocketmanager.h"
#include <QTcpSocket>
#include <QDialog>

BaseServer::BaseServer() {

    QObject::connect(&gip , SIGNAL(ipAddressSetted(QString)) , this,SLOT(setIp(QString)));

    QObject::connect(&gip , &GetIPpage::finished , [&](){
        qDebug() << "peogram is exited";
        exit(0);
    });

    gip.show();

    gip.exec();
}

void BaseServer::start()
{
    for (int i = 5100; i < 5200; ++i) {
        if(this->listen(IP,i)){
            qDebug() << "base server created : " <<this->serverAddress() << " : " << this->serverPort();
            break;
        }
    }

    QObject::connect(this , SIGNAL(newConnection()) , this , SLOT(new_connection()));
}

QJsonObject BaseServer::start_game_requested(QString username)
{
    QJsonObject configOBJ;

    //chaeck for disconnected players
    for (auto gameserver : gameservers.values()) {
        if(gameserver->requestForBackingToGame(username)){
            qDebug() << "back " << username << " to a created subserver";

            configOBJ.insert("ipAddress" , QJsonValue(gameserver->serverAddress().toString()));

            configOBJ.insert("port" , QJsonValue((int)gameserver->serverPort()));

            return configOBJ;
        }
    }

    //chaeck for new connection in subserver
    for (auto gameserver : gameservers.values()) {
        if(gameserver->requestForNewConection(username)){
            qDebug() << "adding " << username << " to a created subserver";

            configOBJ.insert("ipAddress" , QJsonValue(gameserver->serverAddress().toString()));

            configOBJ.insert("port" , QJsonValue((int)gameserver->serverPort()));

            return configOBJ;
        }
    }

    //make new subserver
    qDebug() << "new subserver is creating for " << username;

    GameServer* gs = new GameServer(IP);

    qDebug() << "new subserver created for " << gs->getId();

    configOBJ.insert("ipAddress" , QJsonValue(gs->serverAddress().toString()));

    configOBJ.insert("port" , QJsonValue((int)gs->serverPort()));

    gameservers.insert(gs->getId() , gs);

    gs->waitFor(username);

    QObject::connect(gs , &GameServer::GameDestroyed , [&](int id){
        qDebug()<< "--fined subserver in baseserver vector-- : " << id;

        gameservers.erase(gameservers.find(id));

        qDebug()<<"--subserver : " << id << " in vector deleted--";
    });

    return configOBJ;
}

bool BaseServer::setIp(QString ip)
{
    if (IP.setAddress(ip))
    {
        qDebug() << "ip address set on " << IP << " succssesfuly";
        return true;
    }
    else{
        qDebug() << "ip address set on " << IP << " not succssesfuly";
        return false;
    }
}

void BaseServer::new_connection(){
    QTcpSocket* socket = this->nextPendingConnection();

    qDebug() << "new client connect to base server : " << socket->localAddress() << " : " << socket->localPort();

    ServerSocketManager* sm = new ServerSocketManager(socket);

    //connect teh new client signals from socket manager to BaseServer
    QObject::connect(sm , &ServerSocketManager::get_player_statement, [&](QString username)->int{
        qDebug() << "get rem time for bakc to game for " << username;

        for (auto game : gameservers.values()) {
            int res = game->request_for_rem_time_of_game(username);
            if(res){
                qDebug() << "   answer is " << res;
                return res;
            }
        }
        qDebug() << "   answer is " << 0;
        return 0;
    });

    QObject::connect(sm , SIGNAL(start_game_request(QString)) , this , SLOT(start_game_requested(QString)));
}
