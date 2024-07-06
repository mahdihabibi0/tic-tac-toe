#ifndef USERSHANDLER_H
#define USERSHANDLER_H
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

QJsonObject open_file(){
    QFile file("users.json");
    file.open(QIODevice::ReadOnly);
    QString content=file.readAll();
    file.close();
    QJsonDocument jd=QJsonDocument::fromJson(content.toUtf8());
    return jd.object();
}

bool try_to_signup(QJsonObject qjo){
    QJsonObject jo = open_file();

    if(jo.find(qjo["username"].toString())!=jo.end())
        return false;

    QJsonObject addToJsonFile;

    addToJsonFile.insert("username",qjo["username"].toString());

    addToJsonFile.insert("email",qjo["email"].toString());

    addToJsonFile.insert("password",qjo["password"].toString());

    jo.insert(qjo["username"].toString()  , QJsonObject(addToJsonFile));

    QJsonDocument jd = QJsonDocument(jo);

    QFile file("users.json");

    file.open(QIODevice::WriteOnly);

    file.write(QJsonDocument(jd).toJson(QJsonDocument::Indented));

    return true;
}

bool try_to_login(QJsonObject qjo){
    QJsonObject jo = open_file();

    if(jo.find(qjo["username"].toString())==jo.end() || jo[qjo["username"].toString()].toObject()["password"].toString()!=qjo["password"].toString())
        return false;

    return true;
}

void logout(QString obj){
    QJsonObject jo = open_file();
    jo[obj].toObject()["situation"] = "ofline";
}

#endif // USERSHANDLER_H
