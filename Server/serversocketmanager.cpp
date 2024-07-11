#include "serversocketmanager.h"
#include "usersHandler.h"
#include "usersHandler.h"

QByteArray make_json_byte_for_server(const QJsonObject& obj){
    QJsonDocument doc(obj);

    QByteArray byteArray = doc.toJson();

    return byteArray;
}

ServerSocketManager::ServerSocketManager(QTcpSocket* s) {
    socket = s;

    QObject::connect(socket,SIGNAL(readyRead()),this,SLOT(read_handler()));

    QObject::connect(socket,SIGNAL(disconnected()),this,SLOT(disconnect_handler()));
}

void ServerSocketManager::read_handler()
{
    QJsonDocument processDoc = QJsonDocument::fromJson(socket->readAll());

    QJsonObject processObj = processDoc.object();

    QString process = processObj["process"].toString();

    qDebug() << "new process : " << process << " form " << username << " starrted  <baseserver process -> bp>";

    //start game process
    if(process =="Start Game"){
        qDebug() << "   bp--get ip config of game server from base server";

        QJsonObject ipConfigOfGameServer = emit start_game_request(username);

        qDebug() << "   bp--ip config of game server getted";

        socket->write(make_json_byte_for_server(ipConfigOfGameServer));

        qDebug() << "   bp--ip config of game server sended";
    }

    //Signup process
    else if(process=="Signup"){
        qDebug() << "   bp--try to signup";

        bool check = try_to_signup(processObj);

        if(check){
            qDebug() << "   bp--signup was succussefuly";
        }
        else{
            qDebug() << "   bp--signup go wrong";
        }

        this->socket->write(QByteArray().setNum(check));
    }

    //Login process
    else if(process=="Login"){

        bool check = try_to_login(processObj);

        if(check){
            qDebug() << "   bp--login was succussefuly";

            username = processObj["username"].toString();

            qDebug() << "   bp--username of Server Socket Manager setted";

            changeUserSit(username , true);

            qDebug() << "user " << username << " is online now";
        }
        else{
            qDebug() << "   bp--login go wrong";
        }

        this->socket->write(QByteArray().setNum(check));
    }

    //Get Information process
    else if(process=="Get Information By Username"){
        QJsonObject userNameInformation;

        QString requestUserName = processObj["username"].toString();

        qDebug() << "   bp--check for username request acsses that " << username << " information";
        if(requestUserName != username){
            qDebug() << "   bp--request for information denide for " << requestUserName <<" and send null json";
            socket->write(make_json_byte_for_server(userNameInformation));
        }
        else{
            qDebug() << "   bp--request for information sucssefuly";
            QJsonObject userNameInformation = get_user_information(username);

            socket->write(make_json_byte_for_server(userNameInformation));
        }
    }


    else if(process=="Default Login"){
        bool check = try_to_default_login(processObj);

        if(check){
            qDebug() << "   bp--defult login was succussefuly";

            username = processObj["username"].toString();

            qDebug() << "   bp--username of Server Socket Manager setted";

            changeUserSit(username , true);

            qDebug() << "user " << username << " is online now";
        }
        else{
            qDebug() << "   bp--defult login go wrong";
        }

        this->socket->write(QByteArray().setNum(check));
    }

    //Get Statement process
    if(process=="Get Player Statement"){
        QString requestUserName = processObj["username"].toString();

        qDebug() << "   bp--check for username request acsses" << username << " information";
        if(requestUserName != username){
            qDebug() << "   bp--request for information denide for " << requestUserName <<" and send -1";
            socket->write(QByteArray().setNum(-1));
        }
        else{
            qDebug() << "   bp--request for player statment sucssefuly";

            socket->write(QByteArray().setNum(emit get_player_statement(requestUserName)));
        }
    }

    else if(process=="Logout"){

        changeUserSit(username , false);

        username = "";
    }

    qDebug() << "process : " << process << " form " << username << " finished </bp>";

}

void ServerSocketManager::disconnect_handler()
{
    qDebug() << "user " << username << " disconnected form base server";

    changeUserSit(username , false);

    qDebug() << "user " << username << " is ofline now";

    this->deleteLater();
}
