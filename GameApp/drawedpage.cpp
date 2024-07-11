#include "drawedpage.h"
#include "ui_drawedpage.h"

drawedPage::drawedPage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::drawedPage)
{
    ui->setupUi(this);
}

drawedPage::~drawedPage()
{
    delete ui;
}

void drawedPage::on_backToHome_clicked()
{
    this->close();
    emit connect_to_base_server_and_default_login();
}

