#ifndef DRAWEDPAGE_H
#define DRAWEDPAGE_H

#include <QDialog>

namespace Ui {
class drawedPage;
}

class drawedPage : public QDialog
{
    Q_OBJECT

public:
    explicit drawedPage(QWidget *parent = nullptr);
    ~drawedPage();

private slots:
    void on_backToHome_clicked();

private:
    Ui::drawedPage *ui;
signals:
    void connect_to_base_server_and_default_login();
};

#endif // DRAWEDPAGE_H
