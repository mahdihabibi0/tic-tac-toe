#ifndef USERSHANDLER_H
#define USERSHANDLER_H
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QTime>

QString hash(QString pass);

QJsonObject getFileJson();

QJsonObject get_user_information(QString username);

bool try_to_signup(QJsonObject requestForObj);

bool try_to_login(QJsonObject requestForObj);

bool try_to_default_login(QJsonObject requestForObj);

void changeUserSit(QString username , bool s);

void userWin(QString username , QString challengerNam);

void userLose(QString username , QString challengerNam);

void userDrawed(QString username , QString challengerName);

#endif // USERSHANDLER_H
