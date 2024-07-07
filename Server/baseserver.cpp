#include "baseserver.h"
#include "socketmanager.h"
#include <QTcpSocket>
#include <QDialog>

BaseServer::BaseServer() {

    QObject::connect(&gip , SIGNAL(ipAddress(QString)) , this,SLOT(setIp(QString)));

    QObject::connect(&gip , SIGNAL(closeThePage()) , &gip,SLOT(close()));

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

QJsonObject BaseServer::start_game(QString username)
{
    QJsonObject configOBJ;

    for (GameServer* gameserver : gameservers) {
        if(gameserver->requestForBackingToGame(username)){
            configOBJ.insert("ipAddress" , QJsonValue(gameserver->serverAddress().toString()));

            configOBJ.insert("port" , QJsonValue((int)gameserver->serverPort()));

            return configOBJ;
        }
    }

    for (GameServer* gameserver : gameservers) {
        if(gameserver->requestForNewConection(username)){
            configOBJ.insert("ipAddress" , QJsonValue(gameserver->serverAddress().toString()));

            configOBJ.insert("port" , QJsonValue((int)gameserver->serverPort()));

            return configOBJ;
        }
    }

    GameServer* gs = new GameServer(IP);

    configOBJ.insert("ipAddress" , QJsonValue(gs->serverAddress().toString()));

    configOBJ.insert("port" , QJsonValue((int)gs->serverPort()));

    gameservers.push_back(gs);

    return configOBJ;
}

bool BaseServer::setIp(QString ip)
{
    if (IP.setAddress(ip))
        return true;
    else
        return false;
}

void BaseServer::new_connection(){
    QTcpSocket* socket = this->nextPendingConnection();

    qDebug() << "new server socket : " << socket->localAddress() << " : " << socket->localPort();

    SocketManager* sm = new SocketManager(socket);

    QObject::connect(sm , &SocketManager::get_player_statement , [&](QString username)->int{
        for (auto game : gameservers) {
            int res = game->request_for_rem_time_of_game(username);
            if(res)
                return res;
        }
        return 0;
    })

    QObject::connect(sm , SIGNAL(start_game_request()) , this , SLOT(start_game()));
}
