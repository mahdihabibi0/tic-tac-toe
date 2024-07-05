#include "homepage.h"
#include "ui_homepage.h"
#include <QMdiSubWindow>
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

QString get_user_score(){
    return get_file_jsonobj("user.json")["score"].toString();
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

void HomePage::start_game(){
    emit show_game_page();

    this->close();
}

void HomePage::on_startGame_clicked()
{
    if(!emit try_to_start_new_game())
        return;

    QMessageBox* waitForPlayer2 = new QMessageBox(this);
    waitForPlayer2->setIcon(QMessageBox::Information);
    waitForPlayer2->setStandardButtons(QMessageBox::NoButton);
    waitForPlayer2->setWindowTitle("waiting");
    waitForPlayer2->setText("waiting for second player connection");
    waitForPlayer2->setGeometry(QRect(100 , 100 , 100 , 31));
    waitForPlayer2->show();


    QObject::connect(this, SIGNAL(show_game_page()), waitForPlayer2, SLOT(deleteLater()));
}

//we must send challanger index and get his/her name
QString get_challanger_name(int challangerIndex){
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

    gameHistoryPage.setWindowModality(Qt::ApplicationModal);

    gameHistoryPage.show();
}


void HomePage::on_logOut_clicked()
{
    emit log_out();

    delete_file_content();

    this->close();
}

