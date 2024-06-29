#include "tcpsocketmanager.h"
#include <QFile>
#include <QMessageBox>
#include <QDialog>


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
    user.insert("process" , QJsonValue("Login"));

    QJsonDocument doc(user);

    QByteArray byteArray = doc.toJson();

    this->write(byteArray);

    this->waitForReadyRead(-1);

    QByteArray answer = this->readAll();

    if(answer.toInt())
        return true;
    else
        return false;
}

bool TCPSocketManager::try_to_signup(QJsonObject &user)
{
    user.insert("process" , QJsonValue("Signup"));

    QJsonDocument doc(user);

    QByteArray byteArray = doc.toJson();

    this->write(byteArray);

    this->waitForReadyRead(-1);

    QByteArray answer = this->readAll();

    if(answer.toInt())
        return true;
    else
        return false;
}

void TCPSocketManager::connected_to_server()
{
    emit server_is_online();
    waitingForServerConnection.close();
}

bool TCPSocketManager::try_to_start_game(){
    QJsonObject process;

    process.insert("process" , QJsonValue("Start Game"));

    QJsonDocument doc(process);

    QByteArray byteArray = doc.toJson();

    this->write(byteArray);

    this->waitForReadyRead(-1);

    QJsonDocument document = QJsonDocument::fromJson(this->readAll());

    QJsonObject ipConfigObj = document.object();

    this->connectToHost(ipConfigObj["ipAddress"].toString() , ipConfigObj["port"].toInt());

    this->waitForReadyRead(-1);

    if(this->readAll().toInt())
    {
        return true;
    }

    return false;
}
