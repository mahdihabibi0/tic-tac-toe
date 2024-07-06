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

    void start_game();
protected:
    void showEvent(QShowEvent* event) override;
private slots:

    void on_startGame_clicked();

    void on_gameHistory_clicked();

    void on_logOut_clicked();

private:
    Ui::HomePage *ui;

signals:
    void logout();

    bool try_to_start_new_game();

    void show_game_page();

};

#endif // HOMEPAGE_H
