#ifndef USERSHANDLER_H
#define USERSHANDLER_H
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QTime>

QJsonObject open_file(){
    QFile file("users.json");
    if(!file.open(QIODevice::ReadOnly)){
        file.open(QIODevice::WriteOnly);
        file.close();
        file.open(QIODevice::ReadOnly);
    }
    QString content=file.readAll();
    file.close();
    QJsonDocument jd=QJsonDocument::fromJson(content.toUtf8());
    return jd.object();
}

QJsonObject get_user_information(QString username){
    QJsonObject jo = open_file();
    return jo[username].toObject();

}

bool try_to_signup(QJsonObject qjo){
    QJsonObject jo = open_file();

    if(jo.find(qjo["username"].toString())!=jo.end())
        return false;

    QJsonObject addToJsonFile;

    addToJsonFile.insert("username",qjo["username"].toString());

    addToJsonFile.insert("email",qjo["email"].toString());

    addToJsonFile.insert("password",qjo["password"].toString());

    addToJsonFile.insert("score", 0);

    addToJsonFile.insert("password",qjo["password"].toString());

    auto lg1 = QJsonObject({
        qMakePair(QString("challangername"), QJsonValue(QString())),
        qMakePair(QString("situation"), QJsonValue(QString())),
        qMakePair(QString("time"), QJsonValue(QString()))
    });

    QJsonArray ja;

    ja.push_back(QJsonValue(lg1));
    ja.push_back(QJsonValue(lg1));
    ja.push_back(QJsonValue(lg1));

    addToJsonFile.insert("last3game",ja);

    jo.insert(qjo["username"].toString()  , QJsonObject(addToJsonFile));

    QJsonDocument jd = QJsonDocument(jo);

    QFile file("users.json");

    file.open(QIODevice::WriteOnly);

    file.write(QJsonDocument(jd).toJson(QJsonDocument::Indented));

    file.close();

    return true;
}

bool try_to_login(QJsonObject qjo){
    QJsonObject jo = open_file();

    if(jo.find(qjo["username"].toString())==jo.end() || jo[qjo["username"].toString()].toObject()["password"].toString()!=qjo["password"].toString())
        return false;

    return true;
}

#endif // USERSHANDLER_H
