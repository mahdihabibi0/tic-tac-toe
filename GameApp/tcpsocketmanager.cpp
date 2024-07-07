#include "tcpsocketmanager.h"
#include <QFile>
#include <QMessageBox>
#include <QDialog>
#include <QByteArray>
#include <QSignalSpy>
#include "getippage.h"
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

    // waitingForServerConnection.setWindowFlags(Qt::WindowTitleHint | Qt::FramelessWindowHint);

    waitingForServerConnection.setStandardButtons(QMessageBox::NoButton);

    waitingForServerConnection.exec();

}

void TCPSocketManager::connected_to_server()
{
    emit server_is_online();

    waitingForServerConnection.reject();
}

// the constructor
TCPSocketManager::TCPSocketManager() {

    QObject::connect(this,SIGNAL(connected()),this,SLOT(connected_to_server()));

    GetIpPage gip;

    QObject::connect(&gip , SIGNAL(rejected()),this,SLOT(close_the_program()));

    gip.exec();


    this->connectToHost(gip.get_ip_address(),gip.get_port());

    waiting_for_connection(waitingForServerConnection);

}


bool TCPSocketManager::try_to_login_handler(QJsonObject user)
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

bool TCPSocketManager::try_to_default_login_handler(QJsonObject user)
{
    QJsonObject process = make_process(user , "Default Login");

    this->write(make_json_byte(process));

    this->waitForReadyRead(-1);

    QByteArray answer = this->readAll();

    if(answer.toInt())
        return true;

    else
        return false;
}

bool TCPSocketManager::try_to_signup_handler(QJsonObject user)
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

QJsonObject TCPSocketManager::get_user_information(QString username)
{
    QJsonObject process = make_process("Get Information By Username");

    process.insert("username" , QJsonValue(username));

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

    c.insert("Player Won",CommandOfSubServer::playerWon);

    c.insert("Player Lose",CommandOfSubServer::playerLose);

    c.insert("Game Drawed",CommandOfSubServer::gameِِDrawed);

    c.insert("Skip Button Locked" , CommandOfSubServer::skipButtonLocked);
}

bool TCPSocketManager::try_to_start_game(){
    QJsonObject process = make_process("Start Game");

    this->write(make_json_byte(process));

    this->waitForReadyRead(-1);

    QJsonObject ipConfigObj = make_byte_json(this->readAll());

    this->disconnect();

    this->connectToHost(ipConfigObj["ipAddress"].toString() , ipConfigObj["port"].toInt());

    make_commands(commands);

    this->write(make_json_byte(process));

    QObject::connect(this , SIGNAL(readyRead()) , this , SLOT(subserver_read_handeler()));

    return true;
}

// sub server is connected
void TCPSocketManager::set_button_situation_handeler(QJsonObject obj , Situation s){
    int i = obj["loc"].toObject()["i"].toInt();

    int j = obj["loc"].toObject()["j"].toInt();

    emit set_button_situation(i , j , s);
}

void TCPSocketManager::subserver_palayer_answered_true_process(int i , int j){
    QJsonObject process = make_process("Answered True To Question");

    QJsonObject loc;

    loc.insert("i" , QJsonValue(i));

    loc.insert("j" , QJsonValue(j));

    process.insert("loc" , QJsonValue(loc));

    this->write(make_json_byte(process));
}

void TCPSocketManager::subserver_palayer_answered_false_process(int i, int j)
{
    QJsonObject process = make_process("Answered False To Question");

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

void TCPSocketManager::subserver_player_set_back_to_normal(int i, int j)
{
    QJsonObject process = make_process("Set Button Back To Normal");

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

void TCPSocketManager::close_the_program()
{
    exit(0);
}

int TCPSocketManager::get_player_statement(QString username)
{
    QJsonObject process = make_process("Get Player Statement");

    process.insert("username" , QJsonValue(username));

    this->write(make_json_byte(process));

    waitForReadyRead(-1);

    return this->readAll().toInt();
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
    case CommandOfSubServer::playerWon:
        emit player_won();
        break;
    case CommandOfSubServer::playerLose:
        emit player_lose();
        break;
    case CommandOfSubServer::gameِِDrawed:
        emit game_drawed();
        break;
    case CommandOfSubServer::skipButtonLocked:
        emit lock_skip_button();
        break;
    default:
        throw std::exception();
        break;
    }
}
