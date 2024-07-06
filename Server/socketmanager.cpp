#include "socketmanager.h"
#include "usersHandler.h"

/*QJsonObject make_byte_json(const QByteArray& data){
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    QJsonObject jsonObj = jsonDoc.object();

    return jsonObj;
}*/

QByteArray make_json_byte(const QJsonObject& obj){
    QJsonDocument doc(obj);

    QByteArray byteArray = doc.toJson();

    return byteArray;
}

SocketManager::SocketManager(QTcpSocket* s) {
    socket=s;
    QObject::connect(socket,SIGNAL(readyRead()),this,SLOT(read_handler()));
    QObject::connect(socket,SIGNAL(disconnected()),this,SLOT(disconnect_handler()));
}

void SocketManager::read_handler()
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(socket->readAll());

    QJsonObject jsonobj = jsonDoc.object();

    if(jsonobj["process"].toString()=="Start Game"){
        socket->write(make_json_byte(emit start_game_request(username)));
    }
    if(jsonobj["process"].toString()=="logout"){
        logout(jsonobj["userName"].toString());
    }
    if(jsonobj["process"].toString()=="Signup"){
        if(try_to_signup(jsonobj))
            this->socket->write("1");
        else
            this->socket->write("0");
    }
    if(jsonobj["process"].toString()=="Login"){
        username = jsonobj["username"].toString();
        if(try_to_login(jsonobj))
            this->socket->write("1");
        else
            this->socket->write("0");
    }

}

void SocketManager::disconnect_handler()
{

}
