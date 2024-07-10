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
