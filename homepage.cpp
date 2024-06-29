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

void HomePage::on_startGame_clicked()
{
    if(emit try_to_start_new_game()){
        emit show_game_page();
    }
    else
        exit(0);
}

