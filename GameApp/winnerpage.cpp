#include "winnerpage.h"
#include "ui_winnerpage.h"

winnerPage::winnerPage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::winnerPage)
{
    ui->setupUi(this);
}

winnerPage::~winnerPage()
{
    delete ui;
}

void winnerPage::on_backToHome_clicked()
{
    this->close();

}

