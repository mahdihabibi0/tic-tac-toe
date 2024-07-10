#include "gamesocketmanager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QThread>
#include <QJsonObject>
#include <QEventLoop>
#include "usersHandler.h"

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
                         QPair<QString,QJsonValue>("j",QJsonValue(loc.second))
                     }));
    return jo;
}

QJsonObject make_command(QString command){
    QJsonObject jo;
    jo.insert("command",command);
    return jo;
}

QJsonObject make_byte_json(const QByteArray& data){
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    QJsonObject jsonObj = jsonDoc.object();

    return jsonObj;
}

GameSocketManager::GameSocketManager():
    socket(nullptr),
    chanceForWin(true),
    Active(false)
    {
    QObject::connect(&map , SIGNAL(win()) , this , SLOT(win_handler()));

    QObject::connect(&map , SIGNAL(thereIsNoChanceForWin()) , this , SLOT(thereIsNoChanceForWin_handler()));
}

bool GameSocketManager::getChanceForWin()
{
    return chanceForWin;
}

bool GameSocketManager::setSocket(QTcpSocket *socket)
{
    if(Active)
        return false;

    this->socket = socket;

    QObject::connect(socket,SIGNAL(readyRead()),this,SLOT(read_handler()));

    QObject::connect(socket , SIGNAL(disconnected()) , this , SLOT(disconnected_handler()));

    Active = true;

    return true;
}

void GameSocketManager::read_handler()
{
    QJsonObject processObj = make_byte_json(socket->readAll());

    QJsonObject locationObj = processObj["loc"].toObject();

    QPair<int,int> location;

    location.first = locationObj["i"].toInt();

    location.second = locationObj["j"].toInt();

    QString processStr = processObj["process"].toString();

    qDebug() << "new process : " << processStr << " form " << username << " starrted";

    //Answered True process
    if(processStr == "Answered True To Question"){
        //emit for secound player to set his own map
        emit player_answered_true(location);
        //set the situation in our map
        map.setItemAtPosition(location.first,location.second,Situation::AnsweredByYou);
    }

    //Answered false process
    else if(processStr == "Answered False To Question"){
        //emit for secound player to set his own map
        emit player_set_button_normal(location);
        //set the situation in our map
        map.setItemAtPosition(location.first,location.second,Situation::AnsweredFalseByYou);
    }

    //Answering to a question process
    else if(processStr == "Answering To Question"){
        //emit for secound player to set his own map
        emit player_answering(location);
        //set the situation in our map
        map.setItemAtPosition(location.first,location.second,Situation::AnsweringByYou);
    }

    //Button To Normal process , this process is for skip button clicked
    else if(processStr == "Set Button Back To Normal"){
        //emit for secound player to set his own map
        emit player_set_button_normal(location);
        //set the situation in our map
        map.setItemAtPosition(location.first,location.second,Situation::Normal);
    }

    //Get New Question process
    else if(processStr == "Get New Question By Type"){
        get_new_question_from_http(processObj["requestType"].toInt());
    }

    //Set Username and check it for connected to thrue subserver process
    else if(processStr == "Set Username"){
        username = processObj["username"].toString();
        if(!emit username_setted(processObj["username"].toString()))
        {
            socket->deleteLater();
        }
        else{
            changeUserSit(username , true);
            qDebug() << "user " << username << " is online now";
        }
    }

    qDebug() << "process : " << processStr << " form " << username << " finished";
}

void GameSocketManager::thereIsNoChanceForWin_handler()
{
    this->chanceForWin = false;

    emit noChanceForWin();
}

void GameSocketManager::win_handler()
{
    emit playerWin(username);
}

void GameSocketManager::disconnected_handler()
{
    qDebug()<<"client " << username <<" disconnected";

    changeUserSit(username , false);

    qDebug() << "user " << username << " is ofline now";

    Active = false;

    emit disconnected(username);

    qDebug()<<"signal disconnected is emited for " << username;

    socket->deleteLater();

    qDebug()<<"socket of " << username <<" deleted";
}

void GameSocketManager::start_game(int timer , QVector<QVector<MapItem>> MapStates , QString ChallengerName)
{
    QJsonObject startGame;

    startGame.insert("command","Start The Game");

    startGame.insert("ChallengerName",ChallengerName);

    startGame.insert("map" , get_map(MapStates));

    startGame.insert("timer" , timer);

    socket->write(make_json_byte_for_gamesocket(startGame));

    qDebug()<<"new command : start game :"<< username << " vs " << ChallengerName;
}

bool GameSocketManager::getActive()
{
    return Active;
}

QString GameSocketManager::get_username()
{
    return username;
}

void GameSocketManager::get_new_question_from_http(int type)
{
    QString path = "https://questionbank.liara.run/api/RWhzYW4gS2FyaW16YWRlaCxNb2hhbW1hZCBKYXZhZCBBYmJhc2ksOVZoRzBYMDUxWTN2/question?type=";
    switch (type) {
    case 0:
        path += "multiple";
        qDebug() << "   get qusetion of type : multiple";
        break;
    case 1:
        path += "short";
        qDebug() << "   get qusetion of type : short";
        break;
    case 2:
        path += "number";
        qDebug() << "   get qusetion of type : number";
        break;
    default:
        break;
    }
    QUrl url(path);
    QNetworkAccessManager manager;

    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    qDebug() << "   request is sended to get question";
    //wait for request finished
    QEventLoop loop;

    QObject::connect(reply, &QNetworkReply::finished,&loop,&QEventLoop::quit);

    loop.exec();

    qDebug() << "   reply of get question is finished";
    //the request is fineshed
    qint32 status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    QByteArray data = reply->readAll();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    QJsonObject jsonObj = jsonDoc.object();

    if(status != 200){
        qDebug() << "   error status is " << status;

        qDebug() << "   error is " << jsonObj["message"].toString();

        get_new_question_from_http(type);
    }
    else{


        qDebug()<<"new question getted from http";

        jsonObj.insert("command","New Question");

        socket->write(make_json_byte_for_gamesocket(jsonObj));

        qDebug()<<"new question sended to " << username;

    }

    reply->deleteLater();
}

//just for debuge
void show_sendedMap(QJsonArray& mapArr){
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            QJsonObject indexObj =  mapArr[i].toArray()[j].toObject();
            qDebug() << "(" << indexObj["type"].toInt() << "," << indexObj["mode"].toInt() << "," << indexObj["sit"].toInt() << ")";
        }
    }
}

QJsonArray GameSocketManager::get_map(QVector<QVector<MapItem>> MapStates)
{
    qDebug() << "creating the map to send";

    QJsonArray mapArr;

    for (int i = 0; i < 3; ++i){
        QJsonArray row;
        for (int j = 0; j < 3; ++j) {
            row.append(QJsonObject({
                QPair<QString , QJsonValue>("type" , MapStates[i][j].type),
                QPair<QString , QJsonValue>("mode" , MapStates[i][j].mode),
                QPair<QString , QJsonValue>("sit" , map.getSitOfItemAtPosition(i , j))
            }));
        }
        mapArr.append(QJsonValue(row));
    }

    show_sendedMap(mapArr);

    return mapArr;
}

void GameSocketManager::send_win()
{
    qDebug() << "new command : Player Won " << username;

    socket->write(make_json_byte_for_gamesocket(make_command("Player Won")));
}

void GameSocketManager::send_loose()
{
    qDebug() << "new command : Player Lose " << username;

    socket->write(make_json_byte_for_gamesocket(make_command("Player Lose")));
}

void GameSocketManager::send_game_equal()
{
    qDebug() << "new command : Game Drawed " << username;

    socket->write(make_json_byte_for_gamesocket(make_command("Game Drawed")));
}

void GameSocketManager::challanger_answered_true(QPair<int,int> loc)
{
    qDebug() << "new command : Answered By Opponen form" << username << " to " << loc.first << " , " << loc.second;

    socket->write(make_json_byte_for_gamesocket(make_command("Set Button To Answered By Opponent",loc)));

    map.setItemAtPosition(loc.first,loc.second,Situation::AnsweredByOpponent);
}

void GameSocketManager::challanger_answering(QPair<int,int> loc)
{
    if(map.getSitOfItemAtPosition(loc.first , loc.second) == Situation::AnsweredFalseByYou)
        return;

    qDebug() << "new command : Answering By Opponent form" << username << " to " << loc.first << " , " << loc.second;

    socket->write(make_json_byte_for_gamesocket(make_command("Set Button To Answering By Opponent",loc)));

    map.setItemAtPosition(loc.first,loc.second,Situation::AnsweringByOpponent);
}

void GameSocketManager::challanger_set_button_back_to_normal(QPair<int,int> loc)
{
    if(map.getSitOfItemAtPosition(loc.first , loc.second) == Situation::AnsweredFalseByYou)
        return;

    qDebug() << "new command : Normal By Opponent form" << username << " to " << loc.first << " , " << loc.second;

    socket->write(make_json_byte_for_gamesocket(make_command("Set Button To Normal By Opponent",loc)));

    map.setItemAtPosition(loc.first,loc.second,Situation::Normal);
}
