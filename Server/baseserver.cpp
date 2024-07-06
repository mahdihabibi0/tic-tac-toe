#include "baseserver.h"
#include "socketmanager.h"
#include <QTcpSocket>
#include <QDialog>

BaseServer::BaseServer() {

    QObject::connect(&gip , SIGNAL(ipAddress(QString)) , this,SLOT(setIp(QString)));

    QObject::connect(&gip , SIGNAL(closeThePage()) , &gip,SLOT(close()));

    gip.show();

    gip.exec();

    for (int i = 5100; i < 5200; ++i) {
        if(this->listen(IP,i)){
            qDebug() << "connected";

            break;
        }
    }

    qDebug() << "base server created : " <<this->serverAddress() << " : " << this->serverPort();

    QObject::connect(this , SIGNAL(newConnection()) , this , SLOT(new_connection()));
}

QJsonObject BaseServer::start_game(QString username)
{
    QJsonObject configOBJ;

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

    QObject::connect(sm , SIGNAL(start_game_request()) , this , SLOT(start_game()));
}
