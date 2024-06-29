#include "loginpage.h"
#include "QMessageBox"
#include <QJsonDocument>
#include <QFile>
#include "ui_loginpage.h"

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

void showMessageBox(QString title , QString Text , QString styleSheet){
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
        showMessageBox("input error", "please enter all information", "color : rgb(255 , 0, 0);");

        return false;
    }
    return true;
}

void LoginPage::on_submitBtn_clicked()
{
    if(!check_inputs()){
        return;
    }
    QJsonObject userJson;

    userJson.insert("username" , QJsonValue(ui->usernameInput->text()));

    userJson.insert("password" , QJsonValue(ui->passwordInput->text()));

    if(!emit try_to_login(userJson))
    {
        showMessageBox("invalide inputs", "username or password is wrong", "color : rgb(255 , 0, 0);");

        return;
    }

    userJson = emit get_user_information(userJson["usernaem"].toString());

    QFile jsonFile("user.json");

    QJsonDocument jsonDoc(userJson);

    if(jsonFile.open(QFile::WriteOnly | QFile::Text))
    {
        jsonFile.write(jsonDoc.toJson());

        jsonFile.close();
    }

    emit login_successfully();
}


void LoginPage::on_signupBtn_clicked()
{
    emit signupButton_clicked();
}


void LoginPage::on_hidePassword_clicked()
{
    if(ui->passwordInput->echoMode() == QLineEdit::Password)
        ui->passwordInput->setEchoMode(QLineEdit::Normal);
    else
        ui->passwordInput->setEchoMode(QLineEdit::Password);
}

