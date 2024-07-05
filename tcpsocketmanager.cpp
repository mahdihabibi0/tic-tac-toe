#include "tcpsocketmanager.h"
#include <QFile>
#include <QMessageBox>
#include <QDialog>
#include <QByteArray>
#include <QSignalSpy>
// usefull funcks

QJsonObject make_process(QString p){
    QJsonObject process;
    process.insert("process" , QJsonValue(p));
    return process;
}

QJsonObject make_process(const QJsonObject& obj , QString p){
    QJsonObject process(obj);
    process.insert("process" , QJsonValue(p));
    return process;
}

QByteArray make_json_byte(const QJsonObject& obj){
    QJsonDocument doc(obj);

    QByteArray byteArray = doc.toJson();

    return byteArray;
}

QJsonObject make_byte_json(const QByteArray& data){
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    QJsonObject jsonObj = jsonDoc.object();

    return jsonObj;
}
// before connection
void failed_connection(){

    QMessageBox msgBox;

    msgBox.setIcon(QMessageBox::Warning);

    msgBox.setWindowTitle("Warning");

    msgBox.setText("server is ofline");

    msgBox.setStandardButtons(QMessageBox::Ok);

    msgBox.exec();

}

void waiting_for_connection(QMessageBox& waitingForServerConnection){

    waitingForServerConnection.setIcon(QMessageBox::Warning);

    waitingForServerConnection.setWindowTitle("Connection Failur");

    waitingForServerConnection.setText("wating for server connection");

    waitingForServerConnection.setStandardButtons(QMessageBox::Ok);

    waitingForServerConnection.exec();

}

void TCPSocketManager::connected_to_server()
{
    emit server_is_online();

    waitingForServerConnection.close();
}

// the constructor
TCPSocketManager::TCPSocketManager() {

    QObject::connect(this,SIGNAL(connected()),this,SLOT(connected_to_server()));

    QFile serverIpPort;

    serverIpPort.setFileName("serveripportaddress.json");

    if(!serverIpPort.open(QIODevice::ReadOnly)){

        failed_connection();

        exit(0);

    }

    QString jsonContents = serverIpPort.readAll();

    serverIpPort.close();

    QJsonDocument jsonDoc =QJsonDocument::fromJson(jsonContents.toUtf8());

    if(jsonDoc.isNull()){

        failed_connection();

        exit(0);

    }

    QJsonObject jsonObj = jsonDoc.object();

    QJsonValue IPaddress = jsonObj.value("ipAddress");

    QJsonValue Port=jsonObj.value("port");

    this->connectToHost(IPaddress.toString(),Port.toInt());

    waiting_for_connection(waitingForServerConnection);

}


bool TCPSocketManager::try_to_login(QJsonObject &user)
{
    QJsonObject process = make_process(user , "Login");

    this->write(make_json_byte(process));

    this->waitForReadyRead(-1);

    QByteArray answer = this->readAll();

    if(answer.toInt())
        return true;

    else
        return false;

}

bool TCPSocketManager::try_to_signup(QJsonObject &user)
{

    QJsonObject process = make_process(user , "Signup");

    this->write(make_json_byte(process));

    this->waitForReadyRead(-1);

    QByteArray answer = this->readAll();

    if(answer.toInt())
        return true;

    else
        return false;

}

void TCPSocketManager::log_out()
{
    QJsonObject process = make_process("logout");

    this->write(make_json_byte(process));
}

QJsonObject TCPSocketManager::get_user_information(QString userName)
{

    QJsonObject process = make_process("Get Information By Username");

    process.insert("username" , QJsonValue(userName));

    this->write(make_json_byte(process));

    waitForReadyRead(-1);

    return make_byte_json(this->readAll());

}

void make_commands(QMap<QString , CommandOfSubServer>& c){
    c.insert("Set Button To Answering By Opponent" ,CommandOfSubServer::setButtonToAnsweringByOpponent);

    c.insert("Set Button To Normal By Opponent" ,CommandOfSubServer::setButtonToNormalByOpponent);

    c.insert("Set Button To Answered By Opponent" ,CommandOfSubServer::setButtonToAnsweredByOpponent);

    c.insert("Start The Game" ,CommandOfSubServer::startTheGame);

    c.insert("New Question" ,CommandOfSubServer::newQuestion);
}

bool TCPSocketManager::try_to_start_game(){
    QJsonObject process = make_process("Start Game");

    this->write(make_json_byte(process));

    this->waitForReadyRead(-1);

    QJsonObject ipConfigObj = make_byte_json(this->readAll());

    this->connectToHost(ipConfigObj["ipAddress"].toString() , ipConfigObj["port"].toInt());

    this->waitForReadyRead(-1);

    if(this->readAll().toInt())
    {
        make_commands(commands);

        QObject::connect(this , SIGNAL(readyRead()) , this , SLOT(subserver_read_handeler()));

        return true;
    }

    return false;
}

// sub server is connected
void TCPSocketManager::set_button_situation_handeler(QJsonObject obj , Situation s){
    int i = obj["loc"].toObject()["i"].toInt();

    int j = obj["loc"].toObject()["j"].toInt();

    emit set_button_situation(i , j , s);
}

void TCPSocketManager::subserver_palayer_answerd_process(int i , int j){
    QJsonObject process = make_process("Answerd To Question");

    QJsonObject loc;

    loc.insert("i" , QJsonValue(i));

    loc.insert("j" , QJsonValue(j));

    process.insert("loc" , QJsonValue(loc));

    this->write(make_json_byte(process));
}

void TCPSocketManager::subserver_player_is_answering_process(int i , int j){
    QJsonObject process = make_process("Answering To Question");

    QJsonObject loc;

    loc.insert("i" , QJsonValue(i));

    loc.insert("j" , QJsonValue(j));

    process.insert("loc" , QJsonValue(loc));

    this->write(make_json_byte(process));
}

QJsonObject TCPSocketManager::get_question_by_type(QuestionType type){
    QJsonObject process = make_process("Get New Question By Type");

    process.insert("requestType" , QJsonValue(type));

    this->write(make_json_byte(process));

    QSignalSpy spy(this, SIGNAL(new_question_taken(QJsonObject)));

    spy.wait(5000); // Wait until the signal fires from subserver_read_handeler

    if (spy.count() > 0) {
        // Signal was emitted, handle it
        QList<QVariant> arguments = spy.takeFirst();
        return arguments.at(0).toJsonObject();
    } else {
        throw QException();
    }
}

void TCPSocketManager::subserver_read_handeler(){
    QJsonObject commandObj = make_byte_json(this->readAll());

    CommandOfSubServer command = commands[commandObj["command"].toString()];

    switch (command) {
    case CommandOfSubServer::setButtonToAnsweredByOpponent:
        set_button_situation_handeler(commandObj , Situation::AnsweredByOpponent);
        break;
    case CommandOfSubServer::setButtonToNormalByOpponent:
        set_button_situation_handeler(commandObj , Situation::Normal);
        break;
    case CommandOfSubServer::setButtonToAnsweringByOpponent:
        set_button_situation_handeler(commandObj , Situation::AnsweringByOpponent);
        break;
    case CommandOfSubServer::startTheGame:
        emit startGame();
        break;
    case CommandOfSubServer::newQuestion:
        emit new_question_taken(commandObj);
        break;
    default:
        throw std::exception();
        break;
    }
}
