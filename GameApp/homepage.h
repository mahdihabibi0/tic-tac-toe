#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QDialog>
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
protected:
    void showEvent(QShowEvent* event) override;
private slots:

    void on_startGame_clicked();

    void on_gameHistory_clicked();

    void on_logOut_clicked();
public slots:
    void start_game(QJsonObject jo);

private:
    Ui::HomePage *ui;

signals:
    int get_statement(QString username);

    void logout();

    bool try_to_start_new_game(QString);

    void show_game_page(QJsonObject);

};

#endif // HOMEPAGE_H
