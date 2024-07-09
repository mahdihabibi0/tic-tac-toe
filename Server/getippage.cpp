#include "getippage.h"
#include "ui_getippage.h"
#include <QMessageBox>

GetIPpage::GetIPpage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GetIPpage)
{
    ui->setupUi(this);

    ui->submitBtn->setEnabled(false);
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

void GetIPpage::on_submitBtn_clicked()
{
    if(emit ipAddressSetted(ui->ipInput->text()))
        this->hide();
    else{
        ui->ipInput->clear();

        show_message_box(this , "invalide input" , "the ip adress is not corruct" , "color : rgba(255 , 0 , 0)");

        ui->submitBtn->setEnabled(false);
    }
}

void GetIPpage::on_ipInput_textChanged(const QString &arg1)
{
    if (arg1.size() >= 10) {
        ui->submitBtn->setEnabled(true);
    } else {
        ui->submitBtn->setEnabled(false);
    }
}

