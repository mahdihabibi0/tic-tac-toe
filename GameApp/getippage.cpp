#include "getippage.h"
#include "ui_getippage.h"
#include "buttonSound.h"

GetIpPage::GetIpPage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GetIpPage)
{
    ui->setupUi(this);

    QObject::connect(this,SIGNAL(accepted()),this,SLOT(accept_button()));

}

GetIpPage::~GetIpPage()
{
    delete ui;
}

QString GetIpPage::get_ip_address()
{
    return ipAddress;
}

int GetIpPage::get_port()
{
    return port;
}

void GetIpPage::accept_button()
{
    play_normal_button_sound();
    ipAddress = ui->ipAddress->text();
    port=ui->port->text().toInt();

}
