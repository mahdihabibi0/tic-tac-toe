#include "lsmanager.h"
#include <QFile>
#include <QJsonDocument>
LSManager::LSManager() {
    QObject::connect(&s , SIGNAL(loginButton_clicked()) , this , SLOT(open_login_page()));
    QObject::connect(&l , SIGNAL(signupButton_clicked()) , this , SLOT(open_signup_page()));
}

bool LSManager::try_to_login_by_default(){
    QFile defultUserJsonFile;

    defultUserJsonFile.setFileName("user.json");

    defultUserJsonFile.open(QIODevice::ReadOnly);

    if(!defultUserJsonFile.isOpen())
        return false;

    QJsonDocument userJsonDoc = QJsonDocument::fromJson(QString(defultUserJsonFile.readAll()).toUtf8());

    if(userJsonDoc.isNull())
        return false;

    QJsonObject userJsonObj = userJsonDoc.object();

    QString username = userJsonObj.value("username").toString();

    if(username.size() == 0)
        return false;

    if(emit defult_login_signal(userJsonObj))
        return true;

    return false;
}

void LSManager::manage(){
    if(!try_to_login_by_default())
        open_login_page();
    else
        emit l.login_successfully();
}

void LSManager::open_login_page(){
    l.show();
    s.hide();
}

void LSManager::open_signup_page(){
    s.show();
    l.hide();
}
