#ifndef LOSERPAGE_H
#define LOSERPAGE_H

#include <QDialog>

namespace Ui {
class loserPage;
}

class loserPage : public QDialog
{
    Q_OBJECT

public:
    explicit loserPage(QWidget *parent = nullptr);
    ~loserPage();

private slots:
    void on_backToHome_clicked();

private:
    Ui::loserPage *ui;
signals:
    void connect_to_base_server_and_default_login();
};

#endif // LOSERPAGE_H
