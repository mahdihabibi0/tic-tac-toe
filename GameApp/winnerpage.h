#ifndef WINNERPAGE_H
#define WINNERPAGE_H

#include <QDialog>

namespace Ui {
class winnerPage;
}

class winnerPage : public QDialog
{
    Q_OBJECT

public:
    explicit winnerPage(QWidget *parent = nullptr);
    ~winnerPage();

private slots:
    void on_backToHome_clicked();

private:
    Ui::winnerPage *ui;

signals:
    void connect_to_base_server_and_default_login();
};

#endif // WINNERPAGE_H
