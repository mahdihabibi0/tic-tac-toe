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

public slots:
    void set_button_situation(int,int,Situation);

    void player_won();

    void player_lose();

    void game_drawed();

    void start(QJsonObject);
private:
    Ui::Game *ui;

    QDialog* page;

    void create_dialog(QString);

    void play_again_handler();

    void back_to_home_handler();
protected:
    void showEvent(QShowEvent *event) override;

signals:
    QJsonObject get_question(QuestionType);

    void answered_true(int,int);

    void answered_false(int,int);

    void is_answering(int,int);

    void show_home_page();

    void set_back_normal(int,int);

    QJsonObject get_game_map();

private slots:

    QJsonObject get_new_question_handler(QuestionType type);

    void answered_true_to_question_handler(int i , int j);

    void answered_false_to_question_handler(int i,int j);

    void is_answering_to_question_handler(int i,int j);

    void set_back_button_to_normal_handler(int i , int j);

    void update_timer();

};

#endif // GAME_H
