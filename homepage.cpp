#include "homepage.h"
#include "ui_homepage.h"

QString get_user_name(){
    QFile user;
    user.setFileName("user.json");
    user.open(QIODevice::ReadOnly);
    QString userContacts = user.readAll();
    user.close();
    QJsonDocument userDoc = QJsonDocument::fromJson(userContacts.toUtf8());
    QJsonObject userObj = userDoc.object();
    return userObj["username"].toString();
}

QString get_user_score(){
    QFile user;
    user.setFileName("user.json");
    user.open(QIODevice::ReadOnly);
    QString userContacts = user.readAll();
    user.close();
    QJsonDocument userDoc = QJsonDocument::fromJson(userContacts.toUtf8());
    QJsonObject userObj = userDoc.object();
    return userObj["score"].toString();
}

HomePage::HomePage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HomePage)
{
    ui->setupUi(this);
    ui->userName->setText(get_user_name());
    ui->userScore->setText(get_user_score());
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::on_startGame_clicked()
{
    if(!emit try_to_start_new_game())
        return;

    QString playerStatus = emit my_status();
    if(playerStatus!="player1"){//this signal can specify that this player is player1 or player2

        this->close();

        emit show_game_page();
    }
    else{
        QMessageBox waitForPlayer2;
        waitForPlayer2.setIcon(QMessageBox::Information);
        waitForPlayer2.setWindowTitle("waiting");
        waitForPlayer2.setText("waiting for second player connection");
        waitForPlayer2.exec();

        if(emit waiting_for_player2_connection()){
            waitForPlayer2.close();
            this->close();
            emit show_game_page();
        }
    }
}

QString get_challanger_name(int challangerIndex){//we must send challanger index and get his/her name
    QFile user;
    user.setFileName("user.json");
    user.open(QIODevice::ReadOnly);
    QString userContacts = user.readAll();
    user.close();
    QJsonDocument userDoc = QJsonDocument::fromJson(userContacts.toUtf8());
    QJsonObject userObj = userDoc.object();
    QJsonArray userArr=userObj["last3games"].toArray();
    return userArr[challangerIndex].toObject()["challengerName"].toString();
}

QString get_challange_time(int challangeNumber){
    QFile user;
    user.setFileName("user.json");
    user.open(QIODevice::ReadOnly);
    QString userContacts = user.readAll();
    user.close();
    QJsonDocument userDoc = QJsonDocument::fromJson(userContacts.toUtf8());
    QJsonObject userObj = userDoc.object();
    QJsonArray userArr=userObj["last3games"].toArray();
    return userArr[challangeNumber].toObject()["challengeTime"].toString();
}

QString get_challange_situation(int challangeNumber){
    QFile user;
    user.setFileName("user.json");
    user.open(QIODevice::ReadOnly);
    QString userContacts = user.readAll();
    user.close();
    QJsonDocument userDoc = QJsonDocument::fromJson(userContacts.toUtf8());
    QJsonObject userObj = userDoc.object();
    QJsonArray userArr=userObj["last3games"].toArray();
    return userArr[challangeNumber].toObject()["challengeSituation"].toString();
}

void create_game_history_page_setting(QDialog &ghp){
    QWidget *centralwidget;
    QLabel *gameHistory;
    QTableWidget *gamesTable;
    ghp.resize(756, 649);
    centralwidget = new QWidget(&ghp);
    centralwidget->setObjectName("centralwidget");
    gameHistory = new QLabel(centralwidget);
    gameHistory->setObjectName("gameHistory");
    gameHistory->setGeometry(QRect(320, 80, 111, 81));
    gamesTable = new QTableWidget(centralwidget);
    gamesTable->setObjectName("gamesTable");
    gamesTable->setGeometry(QRect(180, 160, 401, 281));
    gamesTable->setColumnCount(3);
    gamesTable->setRowCount(1);
    gamesTable->item(0,0)->setText("Challenger Name");
    gamesTable->item(0,1)->setText("Situation");
    gamesTable->item(0,2)->setText("Time");
    int countOfGames = 0;
    for(int i=0;i<3;i++){
        QString challangerName = get_challanger_name(i);
        if(challangerName == "")
            continue;
        gamesTable->insertRow(countOfGames);
        gamesTable->item(countOfGames,0)->setText(challangerName);
        gamesTable->item(countOfGames,1)->setText(get_challange_situation(i));
        gamesTable->item(countOfGames,2)->setText(get_challange_time(i));
        countOfGames++;
    }

}

void HomePage::on_gameHistory_clicked()
{
    QDialog gameHistoryPage;
    create_game_history_page_setting(gameHistoryPage);
    gameHistoryPage.exec();
}

void delete_file_content(){

    QFile user("user.json");

    user.open(QFile::WriteOnly | QFile::Truncate);

    user.close();

}

void HomePage::on_logOut_clicked()
{
    this->close();
    emit logOut();
    delete_file_content();
}

