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

int get_user_score(){
    return get_file_jsonobj("user.json")["score"].toInt();
}

HomePage::HomePage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HomePage)
{
    ui->setupUi(this);
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::start_game(){
    emit show_game_page();

    this->close();
}

void HomePage::showEvent(QShowEvent *event)
{
    ui->userName->setText(get_user_name());

    ui->userScore->setText(QString::number(get_user_score()));

    QDialog::showEvent(event);
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

QString get_challange_situation(int challangeNumber){
    QFile user;

    user.setFileName("user.json");

    user.open(QIODevice::ReadOnly);

    QString userContacts = user.readAll();

    user.close();

    QJsonDocument userDoc = QJsonDocument::fromJson(userContacts.toUtf8());

    QJsonObject userObj = userDoc.object();

    QJsonArray userArr=userObj["last3game"].toArray();

    return userArr[challangeNumber].toObject()["situation"].toString();
}

void create_game_history_page_setting(QDialog *Dialog){
    QTableWidget *gamesTable;

    if (Dialog->objectName().isEmpty())
        Dialog->setObjectName("Dialog");
    Dialog->resize(280, 337);
    gamesTable = new QTableWidget(Dialog);
    if (gamesTable->columnCount() < 3)
        gamesTable->setColumnCount(3);
    QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
    __qtablewidgetitem->setText("Challanger Name");
    gamesTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
    QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
    __qtablewidgetitem1->setText("Situation");
    gamesTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
    QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
    __qtablewidgetitem2->setText("Time");
    gamesTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
    gamesTable->setObjectName("tableWidget");
    gamesTable->setGeometry(QRect(0, 0, 401, 341));

    Dialog->setWindowTitle("History Table");

    int countOfGames = 0;

    for(int i=0;i<3;i++){

        QString challangerName = get_challanger_name(i);

        if(challangerName == "")
            continue;

        gamesTable->insertRow(countOfGames);

        gamesTable->setItem(countOfGames,0 , new QTableWidgetItem(challangerName));

        gamesTable->setItem(countOfGames,1 , new QTableWidgetItem(get_challange_situation(i)));

        gamesTable->setItem(countOfGames,2 , new QTableWidgetItem(get_challange_time(i)));

        countOfGames++;
    }

}

void HomePage::on_gameHistory_clicked()
{
    QDialog gameHistoryPage;

    create_game_history_page_setting(&gameHistoryPage);

    gameHistoryPage.setWindowModality(Qt::ApplicationModal);

    gameHistoryPage.exec();
}


void HomePage::on_logOut_clicked()
{
    emit logout();

    delete_file_content();

    this->close();
}

