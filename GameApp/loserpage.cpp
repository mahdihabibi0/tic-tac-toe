#include "loserpage.h"
#include "ui_loserpage.h"

loserPage::loserPage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::loserPage)
{
    ui->setupUi(this);
}

loserPage::~loserPage()
{
    delete ui;
}

void loserPage::on_backToHome_clicked()
{
    this->close();
    emit connect_to_base_server_and_default_login();
}

