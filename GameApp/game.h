#ifndef GAME_H
#define GAME_H

#include <QDialog>
#include "question.h"
#include "gamebutton.h"

namespace Ui {
class Game;
}

class Game : public QDialog
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);
    ~Game();

private:
    Ui::Game *ui;
    void checking_player_win(int i,int j);
    QDialog* winnerPage;
    void create_dialog(QString);

protected:
    void showEvent(QShowEvent *event) override;

signals:
    QJsonObject get_question(QuestionType);

    QJsonObject get_new_question_from_tcSocket(QuestionType);

    void answered_true(int,int);

    void show_home_page();

public slots:

    QJsonObject get_new_question(QuestionType type);

    void answered_true_to_question_handler(int i , int j);

    QJsonObject get_new_question_handler(QuestionType type);

    void set_button_situation(int,int,Situation);

    void player_won();

    void player_lose();

    void game_drawed();

    void play_again_handler();

    void back_to_home_handler();

};

#endif // GAME_H
