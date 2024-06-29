#include "tcpsocketmanager.h"
#include <QFile>
#include <QMessageBox>
#include <QDialog>
#include "Errors.h"


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

bool TCPSocketManager::try_to_login(const QJsonObject &user)
{
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

bool TCPSocketManager::try_to_signup(const QJsonObject &user)
{
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
