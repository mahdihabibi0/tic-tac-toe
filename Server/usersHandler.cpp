#include "usersHandler.h"

bool try_to_default_login(QJsonObject requestForObj){
    QJsonObject allFileObj = getFileJson();

    auto it = allFileObj.find(requestForObj["username"].toString());

    qDebug() << "try to defult login started <tdl>";

    if(it == allFileObj.end()){
        qDebug() << "   tdl--the requested username is not exist";

        qDebug() << "try to defult login finished --> return false</tdl>";

        return false;
    }

    auto findObj = it.value().toObject();

    if(findObj["password"] != requestForObj["password"].toString()){

        qDebug() << "   tdl--password is wrong";

        qDebug() << "try to defult login finished --> return false</tdl>";

        return false;
    }

    if(findObj["situation"].toBool()){
        qDebug() << "   tdl--user is online";

        qDebug() << "try to defult login finished --> return false</tdl>";
        return false;
    }

    qDebug() << "try to defult login finished --> return true</tdl>";
    return true;
}

bool try_to_login(QJsonObject requestForObj){
    QJsonObject allFileObj = getFileJson();

    auto it = allFileObj.find(requestForObj["username"].toString());

    qDebug() << "try to login started <tl>";

    if(it == allFileObj.end()){
        qDebug() << "   tl--the requested username is not exist";

        qDebug() << "try to login finished --> return false</tl>";

        return false;
    }

    auto findObj = it.value().toObject();

    if(findObj["password"] != hash(requestForObj["password"].toString())){

        qDebug() << "   tl--password is wrong";

        qDebug() << "try to login finished --> return false</tl>";

        return false;
    }

    if(findObj["situation"].toBool()){
        qDebug() << "   tl--user is online";

        qDebug() << "try to login finished --> return false</tl>";
        return false;
    }

    qDebug() << "try to login finished --> return true</tl>";

    return true;
}

bool try_to_signup(QJsonObject requestForObj){
    QJsonObject allFileObj = getFileJson();

    qDebug() << "try to signup started <ts>";

    if(allFileObj.find(requestForObj["username"].toString())!= allFileObj.end()){
        qDebug() << "   ts--the requested username is already exist";

        qDebug() << "try to signup finished --> return false</ts>";

        return false;
    }

    qDebug() << "   ts--makeup the new user";

    QJsonObject newJsonObj;

    newJsonObj.insert("username",requestForObj["username"].toString());

    newJsonObj.insert("email",requestForObj["email"].toString());

    newJsonObj.insert("password",hash(requestForObj["password"].toString()));

    newJsonObj.insert("score", 0);

    auto tempGameHistorey = QJsonObject({
        qMakePair(QString("challangername"), QJsonValue(QString())),
        qMakePair(QString("situation"), QJsonValue(QString())),
        qMakePair(QString("time"), QJsonValue(QString()))
    });

    QJsonArray ja;

    ja.push_back(QJsonValue(tempGameHistorey));
    ja.push_back(QJsonValue(tempGameHistorey));
    ja.push_back(QJsonValue(tempGameHistorey));

    newJsonObj.insert("last3game",ja);

    newJsonObj.insert("situation" , false);

    allFileObj.insert(requestForObj["username"].toString()  , QJsonObject(newJsonObj));

    QJsonDocument newJsonDoc = QJsonDocument(allFileObj);

    qDebug() << "   ts--adding the new user to users.json";

    QFile file("users.json");

    file.open(QIODevice::WriteOnly);

    file.write(QJsonDocument(newJsonDoc).toJson(QJsonDocument::Indented));

    file.close();

    qDebug() << "try to signup finished --> return true</ts>";

    return true;
}

QJsonObject get_user_information(QString username){
    QJsonObject allFileObj = getFileJson();

    QJsonObject requestedObj = allFileObj[username].toObject();

    requestedObj.erase(requestedObj.find("situation"));

    return requestedObj;
}

QJsonObject getFileJson(){
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

QString hash(QString pass){
    QString hashedPass = pass;

    int pbeg = 0; //this attribute go to forward
    int pend = pass.length() - 1; //this attribute back

    for (int var = 0; var < pass.length() - 1; ++var) {
        if(var%2 == 0){
            QChar res((int)pass[pbeg].toLatin1() + (int)pass[pend].toLatin1());
            hashedPass[pend] = res;
            pbeg++;
        }
        else{
            QChar res((int)pass[pbeg].toLatin1() + (int)pass[pend].toLatin1());
            hashedPass[pbeg] = res;
            pend--;
        }
    }

    return hashedPass;
}

void changeUserSit(QString username, bool s)
{
    QJsonObject allFileObj = getFileJson();

    QJsonObject requestedObj = allFileObj[username].toObject();

    qDebug() << "change user situation start <cs>";

    if(requestedObj["situation"].toBool() == s){

        qDebug() << " cs--user alredy has situation " << s;

        qDebug() << "change user situation finished </cs>";

        return ;
    }

    requestedObj["situation"] = s;

    allFileObj[username] = requestedObj;

    QFile file("users.json");

    file.open(QIODevice::WriteOnly);

    QJsonDocument newJsonDoc = QJsonDocument(allFileObj);

    file.write(QJsonDocument(newJsonDoc).toJson(QJsonDocument::Indented));

    file.close();

    qDebug() << " cs--user situation changed to " << s;

    qDebug() << "change user situation finished </cs>";
}

void userWin(QString username , QString challengerName)
{
    QJsonObject allFileObj = getFileJson();

    QJsonObject requestedObj = allFileObj[username].toObject();

    requestedObj["score"] = requestedObj["score"].toInt() + 1;

    QJsonArray last3game = requestedObj["last3game"].toArray();

    last3game.push_front(QJsonObject({
            QPair<QString , QString>("challangername" , challengerName),
            QPair<QString , QString>("situation" , "win"),
            QPair<QString , QString>("time" ,QTime::currentTime().toString("hh:mm:ss"))
        }));

    allFileObj[username] = requestedObj;

    QFile file("users.json");

    file.open(QIODevice::WriteOnly);

    QJsonDocument newJsonDoc = QJsonDocument(allFileObj);

    file.write(QJsonDocument(newJsonDoc).toJson(QJsonDocument::Indented));

    file.close();

}

void userLose(QString username , QString challengerName)
{
    QJsonObject allFileObj = getFileJson();

    QJsonObject requestedObj = allFileObj[username].toObject();

    requestedObj["score"] = requestedObj["score"].toInt() - 1;

    QJsonArray last3game = requestedObj["last3game"].toArray();

    last3game.push_front(QJsonObject({
        QPair<QString , QString>("challangername" , challengerName),
        QPair<QString , QString>("situation" , "lose"),
        QPair<QString , QString>("time" ,QTime::currentTime().toString("hh:mm:ss"))
    }));

    allFileObj[username] = requestedObj;

    QFile file("users.json");

    file.open(QIODevice::WriteOnly);

    QJsonDocument newJsonDoc = QJsonDocument(allFileObj);

    file.write(QJsonDocument(newJsonDoc).toJson(QJsonDocument::Indented));

    file.close();
}

void userDrawed(QString username, QString challengerName)
{
    QJsonObject allFileObj = getFileJson();

    QJsonObject requestedObj = allFileObj[username].toObject();

    QJsonArray last3game = requestedObj["last3game"].toArray();

    last3game.push_front(QJsonObject({
        QPair<QString , QString>("challangername" , challengerName),
        QPair<QString , QString>("situation" , "equal"),
        QPair<QString , QString>("time" ,QTime::currentTime().toString("hh:mm:ss"))
    }));

    allFileObj[username] = requestedObj;

    QFile file("users.json");

    file.open(QIODevice::WriteOnly);

    QJsonDocument newJsonDoc = QJsonDocument(allFileObj);

    file.write(QJsonDocument(newJsonDoc).toJson(QJsonDocument::Indented));

    file.close();
}
