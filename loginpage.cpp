#include "loginpage.h"
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
