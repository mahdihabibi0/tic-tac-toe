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
