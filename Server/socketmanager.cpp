#include "socketmanager.h"
#include "usersHandler.h"

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

    QString process = jsonobj["process"].toString();
    if(process =="Start Game"){
        socket->write(make_json_byte(emit start_game_request(username)));
    }

    else if(process=="Signup"){
        if(try_to_signup(jsonobj))
            this->socket->write("1");
        else
            this->socket->write("0");
    }

    else if(process=="Login"){

        username = jsonobj["username"].toString();

        if(try_to_login(jsonobj))
            this->socket->write("1");
        else
            this->socket->write("0");
    }

    else if(process=="Get Information By Username"){
        socket->write(make_json_byte(get_user_information(username)));
    }

    else if(process=="Default Login"){
        username = jsonobj["username"].toString();
        int res;
        if(try_to_default_login(jsonobj))
            res = 1;
        else
            res = 0;

        QByteArray intData(reinterpret_cast<const char*>(&res), sizeof(res));

        socket->write(intData);


    }
    if(process=="Get Player Statement"){
        int res = emit get_player_statement(jsonobj[username].toString());

        QByteArray intData(reinterpret_cast<const char*>(&res), sizeof(res));

        socket->write(intData);
    }
}

void SocketManager::disconnect_handler()
{

}
