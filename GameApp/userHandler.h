#ifndef USERHANDLER_H
#define USERHANDLER_H
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

QString get_challange_situation(int challangeNumber);

QJsonObject get_file_jsonobj(QString fileName);

void delete_file_content();

QString get_user_name();

int get_user_score();

QString get_challanger_name(int challangerIndex);

QString get_challange_time(int challangeNumber);

#endif // USERHANDLER_H
