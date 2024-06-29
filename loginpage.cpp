#include "loginpage.h"
#include "QMessageBox"
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
bool LoginPage::check_inputs(){
    if(ui->usernameInput->text() == "" || ui->passwordInput->text() == ""){
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

void LoginPage::on_submitBtn_clicked()
{
    if(!check_inputs()){
        return;
    }
    QJsonObject userJson;

    userJson.insert("username" , QJsonValue(ui->usernameInput->text()));

    userJson.insert("password" , QJsonValue(ui->passwordInput->text()));

    if(emit try_to_login(userJson)){
        emit login_successfully();
    }
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

