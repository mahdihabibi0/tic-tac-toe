#include "gamesocketmanager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonObject>
#include <QEventLoop>

QByteArray make_json_byte_for_gamesocket(const QJsonObject& obj){
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

GameSocketManager::GameSocketManager():
    chanceForWin(true) ,
    socket(nullptr) {
    QObject::connect(&map , SIGNAL(thereIsNoChanceForWin()) , this , SLOT(thereIsNoChanceForWin_handler()));
}

bool GameSocketManager::getChanceForWin()
{
    return this->chanceForWin;
}

bool GameSocketManager::setSocket(QTcpSocket *socket)
{
    if(this->socket)
        return false;

    this->socket = socket;

    QObject::connect(socket,SIGNAL(readyRead()),this,SLOT(read_handler()));

    QObject::connect(socket , SIGNAL(disconnected()) , this , SLOT(disconnected_handler()));

    return true;
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
        emit player_set_button_normal(location);
    }
    if(jsonobj["process"].toString()=="Get New Question By Type"){
        get_new_question_from_http(jsonobj["requestType"].toInt());
    }
    if(jsonobj["process"].toString()=="Set Username"){
        username = jsonobj["username"].toString();
        if(emit username_setted(jsonobj["username"].toString()))
        {
            delete socket;
            return;
        }
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

void GameSocketManager::disconnected_handler()
{
    emit disconnect(username);
}

void GameSocketManager::start_game(QString ChallengerName)
{
    QJsonObject startGame;
    startGame.insert("command","Start The Game");
    startGame.insert("ChallengerName",ChallengerName);

    socket->write(make_json_byte_for_gamesocket(startGame));
}

QString GameSocketManager::get_username()
{
    return username;
}

void GameSocketManager::get_new_question_from_http(int type)
{
    QUrl url("https://questionbank.liara.run/api/RWhzYW4gS2FyaW16YWRlaCxNb2hhbW1hZCBKYXZhZCBBYmJhc2ksOVZoRzBYMDUxWTN2/question?type=short");
    QNetworkAccessManager manager;

    QNetworkReply *reply = manager.get(QNetworkRequest(url));

    QEventLoop waitForQuestionRicive;

    QObject::connect(reply, &QNetworkReply::finished,&waitForQuestionRicive,&QEventLoop::quit);
    waitForQuestionRicive.exec();
    qint32 status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(status!=200)
        get_new_question_from_http(type);
    else{
        QByteArray data = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        QJsonObject jsonObj = jsonDoc.object();
        jsonObj.insert("command","New Question");
        socket->write(make_json_byte_for_gamesocket(jsonObj));
        qDebug()<<"new question";
    }
    reply->deleteLater();
}

void GameSocketManager::challanger_answered_true(QPair<int,int> loc)
{
    map.setItemAtPosition(loc.first,loc.second,Situation::answeredByOppenent);
    socket->write(make_json_byte_for_gamesocket(make_command("Set Button To Answered By Opponent",loc)));

}

void GameSocketManager::challanger_answering(QPair<int,int> loc)
{
    socket->write(make_json_byte_for_gamesocket(make_command("Set Button To Answering By Opponent",loc)));
}

void GameSocketManager::challanger_set_button_back_to_normal(QPair<int,int> loc)
{
    socket->write(make_json_byte_for_gamesocket(make_command("Set Button To Normal By Opponent",loc)));
}
