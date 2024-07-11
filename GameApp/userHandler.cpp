#include"userHandler.h"
QString get_challange_situation(int challangeNumber) {
    QFile user;

    user.setFileName("user.json");

    user.open(QIODevice::ReadOnly);

    QString userContacts = user.readAll();

    user.close();

    QJsonDocument userDoc = QJsonDocument::fromJson(userContacts.toUtf8());

    QJsonObject userObj = userDoc.object();

    QJsonArray userArr = userObj["last3game"].toArray();

    return userArr[challangeNumber].toObject()["situation"].toString();
}

QJsonObject get_file_jsonobj(QString fileName){
    QFile user;

    user.setFileName(fileName);

    user.open(QIODevice::ReadOnly);

    QString userContacts = user.readAll();

    user.close();

    QJsonDocument userDoc = QJsonDocument::fromJson(userContacts.toUtf8());

    return userDoc.object();
}

void delete_file_content(){

    QFile user("user.json");

    user.open(QFile::WriteOnly | QFile::Truncate);

    user.close();
}

QString get_user_name(){

    return get_file_jsonobj("user.json")["username"].toString();
}

int get_user_score(){
    return get_file_jsonobj("user.json")["score"].toInt();
}

QString get_challanger_name(int challangerIndex){
    QFile user;

    user.setFileName("user.json");

    user.open(QIODevice::ReadOnly);

    QString userContacts = user.readAll();

    user.close();

    QJsonDocument userDoc = QJsonDocument::fromJson(userContacts.toUtf8());

    QJsonObject userObj = userDoc.object();

    QJsonArray userArr=userObj["last3game"].toArray();

    return userArr[challangerIndex].toObject()["challangername"].toString();
}

QString get_challange_time(int challangeNumber){
    QFile user;

    user.setFileName("user.json");

    user.open(QIODevice::ReadOnly);

    QString userContacts = user.readAll();

    user.close();

    QJsonDocument userDoc = QJsonDocument::fromJson(userContacts.toUtf8());

    QJsonObject userObj = userDoc.object();

    QJsonArray userArr=userObj["last3game"].toArray();

    return userArr[challangeNumber].toObject()["time"].toString();
}
