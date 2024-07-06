#include "gamesocketmanager.h"
#include <QJsonObject>
#include <QJsonDocument>

QJsonObject make_byte_json(const QByteArray& data){
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    QJsonObject jsonObj = jsonDoc.object();

    return jsonObj;
}

GameSocketManager::GameSocketManager(QTcpSocket *socket):socket(socket) {
    QObject::connect(socket,SIGNAL(readyRead()),this,SLOT(read_handler()));
}

void GameSocketManager::read_handler()
{
    QJsonObject jsonobj=make_byte_json(socket->readAll());
    if(jsonobj["process"].toString()==""){}

}
