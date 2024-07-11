#include "servermanager.h"
#include "ui_servermanager.h"

ServerManager::ServerManager(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ServerManager)
{
    ui->setupUi(this);
}

ServerManager::~ServerManager()
{
    delete ui;
}
