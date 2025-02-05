#include "signuppage.h"
#include "ui_signuppage.h"
#include <QMessageBox>
#include "buttonSound.h"

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

void showMessageBoxForSignup(QString title , QString Text , QString styleSheet){
    QMessageBox mes;

    mes.setIcon(QMessageBox::Warning);

    mes.setWindowTitle(title);

    mes.setText(Text);

    mes.setStyleSheet(styleSheet);

    mes.setStandardButtons(QMessageBox::Ok);

    mes.exec();
}

bool SignupPage::check_inputs(){
    if(ui->usernameInput->text() == "" || ui->passwordInput->text() == "" || ui->emailInput->text() == ""){
        QMessageBox mes;

        showMessageBoxForSignup("input error", "please enter all information", "color : rgb(255 , 0, 0);");

        return false;
    }
    return true;
}

void SignupPage::on_submitBtn_clicked()
{
    playNormalSound();

    if(!check_inputs())

        return;

    QJsonObject userJson;

    userJson.insert("username" , QJsonValue(ui->usernameInput->text()));

    userJson.insert("password" , QJsonValue(ui->passwordInput->text()));

    userJson.insert("email" , QJsonValue(ui->emailInput->text()));

    if(!emit try_to_signup(userJson)){
        showMessageBoxForSignup("invalid inputs", "username is not valid", "color : rgb(255 , 0, 0);");

        return;
    }

    ui->passwordInput->clear();

    ui->usernameInput->clear();

    emit  loginButton_clicked();
}


void SignupPage::on_hidePassword_clicked()
{
    playNormalSound();

    if(ui->passwordInput->echoMode() == QLineEdit::Password){

        ui->passwordInput->setEchoMode(QLineEdit::Normal);
        ui->hidePassword->setStyleSheet("border-image: url(:/images/2.jpg);");
    }
    else{
        ui->passwordInput->setEchoMode(QLineEdit::Password);
        ui->hidePassword->setStyleSheet("border-image: url(:/images/3.jpg);");
    }

}


void SignupPage::on_loginBtn_clicked()
{
    playNormalSound();

    emit loginButton_clicked();
}

