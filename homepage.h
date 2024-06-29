#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QDialog>

namespace Ui {
class HomePage;
}

class HomePage : public QDialog
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();

private slots:

    void on_startGame_clicked();

private:
    Ui::HomePage *ui;
signals:
    bool try_to_start_new_game();
    void show_game_page();
};

#endif // HOMEPAGE_H
