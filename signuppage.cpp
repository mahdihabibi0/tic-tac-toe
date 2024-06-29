#include "signuppage.h"
#include "ui_signuppage.h"

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
