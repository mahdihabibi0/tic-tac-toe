#include "gamesocketmanager.h"
#include <QJsonObject>
#include <QJsonDocument>

QByteArray make_json_byte(const QJsonObject& obj){
    QJsonDocument doc(obj);

    QByteArray byteArray = doc.toJson();

    return byteArray;
}

QJsonObject make_command(QString command,QPair<int,int> loc){
    QJsonObject jo;
    jo.insert("command",command);
    jo.insert("loc",QJsonObject({
                         QPair<QString,QJsonValue>("i",QJsonValue(loc.first)),
                         QPair<QString,QJsonValue>("i",QJsonValue(loc.second))
                     }));
    return jo;
}

QJsonObject make_byte_json(const QByteArray& data){
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    QJsonObject jsonObj = jsonDoc.object();

    return jsonObj;
}

GameSocketManager::GameSocketManager(QTcpSocket *socket):socket(socket) , chanceForWin(true) {
    QObject::connect(socket,SIGNAL(readyRead()),this,SLOT(read_handler()));
    QObject::connect(&map , SIGNAL(thereIsNoChanceForWin()) , this , SLOT(thereIsNoChanceForWin_handler()));
}

bool GameSocketManager::getChanceForWin()
{
    return this->chanceForWin;
}

void GameSocketManager::read_handler()
{
    QJsonObject jsonobj=make_byte_json(socket->readAll());

    QJsonObject jl = jsonobj["loc"].toObject();
    QPair<int,int> location;
    location.first = jl["i"].toInt();
    location.second = jl["j"].toInt();



    if(jsonobj["process"].toString()=="Answered True To Question"){
        emit player_answered_true(location);
        map.setItemAtPosition(location.first,location.second,Situation::answerdTrue);
    }
    if(jsonobj["process"].toString()=="Answering To Question"){
        emit player_answering(location);

    }
    if(jsonobj["process"].toString()=="Answered False To Question"){

        map.setItemAtPosition(location.first,location.second,Situation::answerdFalse);

        emit player_set_button_normal(location);
    }
    if(jsonobj["process"].toString()=="Set Button Back To Normal"){
        QJsonObject oskipl;
        oskipl.insert("command","Skip Button Locked");
        socket->write(make_json_byte(oskipl));
        emit player_set_button_normal(location);
    }
    if(jsonobj["process"].toString()=="Get New Question By Type"){

    }
    if(jsonobj["process"].toString()=="Set Username"){
        username=jsonobj["username"].toString();
    }

}

void GameSocketManager::thereIsNoChanceForWin_handler()
{
    this->chanceForWin = false;

    emit noChanceForWin();
}

void GameSocketManager::win_handler(QString name)
{
    emit playerWin(name);
}

void GameSocketManager::challanger_answered_true(QPair<int,int> loc)
{
    map.setItemAtPosition(loc.first,loc.second,Situation::answeredByOppenent);
    socket->write(make_json_byte(make_command("Set Button To Answered By Opponent",loc)));

}

void GameSocketManager::challanger_answering(QPair<int,int> loc)
{
    socket->write(make_json_byte(make_command("Set Button To Answering By Opponent",loc)));
}

void GameSocketManager::challanger_set_button_back_to_normal(QPair<int,int> loc)
{
    socket->write(make_json_byte(make_command("Set Button To Normal By Opponent",loc)));
}
