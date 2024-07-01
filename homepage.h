#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QDialog>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QWidget>
#include <QTableWidget>
#include <QMessageBox>

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

    void on_gameHistory_clicked();

    void on_logOut_clicked();

private:
    Ui::HomePage *ui;

signals:

    bool try_to_start_new_game();

    void show_game_page();

    void logOut();

    QString my_status();

    bool waiting_for_player2_connection();

};

#endif // HOMEPAGE_H
