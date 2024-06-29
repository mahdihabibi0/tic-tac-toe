#include "homepage.h"
#include "ui_homepage.h"

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
