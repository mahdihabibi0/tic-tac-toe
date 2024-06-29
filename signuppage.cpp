#include "signuppage.h"
#include "ui_signuppage.h"
#include <QMessageBox>

SignupPage::SignupPage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SignupPage)
{
    ui->setupUi(this);
}

SignupPage::~SignupPage()
{
    delete ui;
}

bool SignupPage::check_inputs(){
    if(ui->usernameInput->text() == "" || ui->passwordInput->text() == "" || ui->emailInput->text() == ""){
        QMessageBox mes;
        mes.setIcon(QMessageBox::Warning);
        mes.setWindowTitle("input error");
        mes.setText("please enter all information");
        mes.setStyleSheet("color : rgb(255 , 0, 0);");
        mes.setStandardButtons(QMessageBox::Ok);
        mes.exec();
        return false;
    }
    return true;
}


void SignupPage::on_submitBtn_clicked()
{
    if(!check_inputs()){
        return;
    }

    QJsonObject userJson;

    userJson.insert("username" , QJsonValue(ui->usernameInput->text()));

    userJson.insert("password" , QJsonValue(ui->passwordInput->text()));

    userJson.insert("email" , QJsonValue(ui->emailInput->text()));

    if(emit try_to_signup(userJson)){
        emit  loginButton_clicked();
    }
}


void SignupPage::on_hidePassword_clicked()
{
    if(ui->passwordInput->echoMode() == QLineEdit::Password)
        ui->passwordInput->setEchoMode(QLineEdit::Normal);
    else
        ui->passwordInput->setEchoMode(QLineEdit::Password);
}


void SignupPage::on_loginBtn_clicked()
{
    emit loginButton_clicked();
}

