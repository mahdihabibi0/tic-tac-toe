#include "server.h"
#include <QTcpSocket>
#include "socketmanager.h"
Server::Server() {
    for (int i = 5000; i < 65535; ++i) {
        if(this->listen(QHostAddress("192.168.134.24"),i)){
            qDebug() << "connected";
            break;
        }
    }

    qDebug() << this->serverAddress() << " : " << this->serverPort();

    QObject::connect(this , SIGNAL(newConnection()) , this , SLOT(new_connection()));

}
void Server::new_connection(){
    SocketManager* sm = new SocketManager;

    QTcpSocket* socket = this->nextPendingConnection();

    QObject::connect(socket , SIGNAL(readyRead()) , sm , SLOT(read_handler()));

    QObject::connect(socket , SIGNAL(disconnected()) , sm , SLOT(disconnect_handler()));
}
