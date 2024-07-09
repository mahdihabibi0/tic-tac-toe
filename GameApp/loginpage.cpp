#include "loginpage.h"
#include "QMessageBox"
#include <QJsonDocument>
#include <QFile>
#include <QThread>
#include "ui_loginpage.h"
#include "buttonSound.h"

LoginPage::LoginPage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginPage)
{
    ui->setupUi(this);
}

LoginPage::~LoginPage()
{
    delete ui;
}

void showMessageBoxForLogin(QString title , QString Text , QString styleSheet){
    QMessageBox mes;

    mes.setIcon(QMessageBox::Warning);

    mes.setWindowTitle(title);

    mes.setText(Text);

    mes.setStyleSheet(styleSheet);

    mes.setStandardButtons(QMessageBox::Ok);

    mes.exec();
}

bool LoginPage::check_inputs(){

    if(ui->usernameInput->text() == "" || ui->passwordInput->text() == ""){
        showMessageBoxForLogin("input error", "please enter all information", "color : rgb(255 , 0, 0);");

        return false;
    }
    return true;
}

void LoginPage::on_submitBtn_clicked()
{
    play_normal_button_sound();

    if(!check_inputs()){
        return;
    }
    QJsonObject userJson;

    userJson.insert("username" , QJsonValue(ui->usernameInput->text()));

    userJson.insert("password" , QJsonValue(ui->passwordInput->text()));

    if(!emit try_to_login(userJson))
    {
        showMessageBoxForLogin("invalide inputs", "username or password is wrong", "color : rgb(255 , 0, 0);");

        return;
    }

    userJson = emit get_user_information(userJson["usernaem"].toString());

    QFile jsonFile("user.json");

    QJsonDocument jsonDoc(userJson);

    if(jsonFile.open(QFile::WriteOnly))
    {
        jsonFile.write(QJsonDocument(jsonDoc).toJson(QJsonDocument::Indented));

        jsonFile.close();
    }

    ui->passwordInput->clear();

    ui->usernameInput->clear();

    emit login_successfully();

    this->close();
}


void LoginPage::on_signupBtn_clicked()
{
    play_normal_button_sound();

    emit signupButton_clicked();
}


void LoginPage::on_hidePassword_clicked()
{
    play_normal_button_sound();

    if(ui->passwordInput->echoMode() == QLineEdit::Password)
        ui->passwordInput->setEchoMode(QLineEdit::Normal);
    else
        ui->passwordInput->setEchoMode(QLineEdit::Password);
}

