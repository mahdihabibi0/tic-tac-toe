#ifndef GETIPPAGE_H
#define GETIPPAGE_H

#include <QDialog>

namespace Ui {
class GetIpPage;
}

class GetIpPage : public QDialog
{
    Q_OBJECT

public:
    explicit GetIpPage(QWidget *parent = nullptr);

    ~GetIpPage();

    QString get_ip_address();

    int get_port();

private:
    QString ipAddress   ;

    int port;

    Ui::GetIpPage *ui;
private slots:
    void accept_button();
};

#endif // GETIPPAGE_H
