#include <QSignalSpy>
#include "gameserver.h"

GameServer::GameServer(QHostAddress ip)
{
    for (int i = 5000; i < 65535; ++i) {
        if(this->listen(ip,i)){
            qDebug() << "connected";
            break;
        }
    }

    qDebug() << "game server created :" <<this->serverAddress() << " : " << this->serverPort();

    QObject::connect(this , SIGNAL(newConnection()) , this , SLOT(newConnectionHandler()));

    gsm1 = NULL;

    gsm2 = NULL;
}

bool GameServer::requestForNewConection()
{
    QSignalSpy spy(this, SIGNAL(newConnection()));

    int count = spy.count();

    if(count >= 2)
        return false;
    else
        return true;

}

void GameServer::newConnectionHandler()
{
    if(!gsm1){
        gsm1 = new GameSocketManager(this->nextPendingConnection());

        return;
    }

    if(!gsm2){
        gsm2 = new GameSocketManager(this->nextPendingConnection());

        return;
    }
}
