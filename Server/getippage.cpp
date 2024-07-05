#include "getippage.h"
#include "ui_getippage.h"
#include <QMessageBox>

GetIPpage::GetIPpage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GetIPpage)
{
    ui->setupUi(this);
    ui->ipInput->setInputMask("000.000.000.000.00");
}
void show_message_box(QWidget* parent ,QString title , QString Text , QString styleSheet){
    QMessageBox mes(parent);

    mes.setIcon(QMessageBox::Warning);

    mes.setWindowTitle(title);

    mes.setText(Text);

    mes.setStyleSheet(styleSheet);

    mes.setStandardButtons(QMessageBox::Ok);

    mes.show();

    mes.exec();
}

GetIPpage::~GetIPpage()
{
    delete ui;
}

void GetIPpage::on_ipInput_editingFinished()
{
    if (ui->ipInput->hasAcceptableInput()) {
        ui->submitBtn->setEnabled(true);
    } else {
        ui->submitBtn->setEnabled(false);
    }
}


void GetIPpage::on_submitBtn_clicked()
{
    if(emit ipAddress(ui->ipInput->text()))
        this->close();
    else{
        ui->ipInput->clear();
        show_message_box(this , "invalide input" , "the ip adress is not corruct" , "color : rgba(255 , 0 , 0)");
        ui->submitBtn->setEnabled(false);
    }
}

