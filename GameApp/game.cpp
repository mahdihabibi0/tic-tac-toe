#include "game.h"
#include "ui_game.h"
#include "gamebutton.h"
#include <QRandomGenerator>
#include <QVector>
#include "userHandler.h"

QJsonObject get_http_request_question(QuestionType type,Game* g) {
    return emit g->get_question(type);
}

void Game::showEvent(QShowEvent *event)
{
    QObject::connect(&gameTimer,SIGNAL(timeout()),this,SLOT(update_timer()));
    gameTimer.start(1000);
    QDialog::showEvent(event);
}



Game::Game(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Game)
{
    ui->setupUi(this);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            GameButton* btn = qobject_cast<GameButton*>(ui->h->itemAtPosition(i,j)->widget());

            QObject::connect(btn , SIGNAL(answered_true_to_question(int,int)) , this , SLOT(answered_true_to_question_handler(int,int)));
            QObject::connect(btn , SIGNAL(answered_false_to_question(int,int)) , this , SLOT(answered_false_to_question_handler(int,int)));
            QObject::connect(btn , SIGNAL(get_new_question(QuestionType)) , this , SLOT(get_new_question_handler(QuestionType)));
            QObject::connect(btn , SIGNAL(is_answering_to_question(int,int)) , this , SLOT(is_answering_to_question_handler(int,int)));
            QObject::connect(btn , SIGNAL(set_back_button_to_normal(int,int)) , this , SLOT(set_back_button_to_normal_handler(int,int)));
        }
    }
}

Game::~Game()
{
    delete ui;
}

void Game::answered_true_to_question_handler(int i, int j)
{
    emit answered_true(i,j);
}

void Game::answered_false_to_question_handler(int i, int j)
{
    emit answered_false(i , j);
}

void Game::is_answering_to_question_handler(int i, int j)
{
    emit is_answering(i , j);
}

void Game::set_back_button_to_normal_handler(int i, int j)
{
    emit set_back_normal(i , j);
}

void Game::update_timer()
{
    timer++;
    ui->Timer->display(timer);
}

QJsonObject Game::get_new_question_handler(QuestionType type)
{
    return emit this->get_question(type);
}

void Game::set_button_situation(int i, int j, Situation sit)
{
    GameButton *btn = qobject_cast<GameButton*>(this->ui->h->itemAtPosition(i,j)->widget());

    btn->set_situation(sit);

}

void Game::player_won()
{
    this->close();
    emit win();
}

void Game::player_lose()
{
    this->close();
    emit lose();
}

void Game::game_drawed()
{
    this->close();
    emit drawed();
}

void Game::start(QJsonObject jo)
{
    ui->player1Username->setText(get_user_name());
    ui->player2Username->setText(jo["ChallengerName"].toString());
    timer = (jo["timer"].toInt());
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            GameButton *btn =qobject_cast<GameButton*>(this->ui->h->itemAtPosition(i,j)->widget());
            btn->setUpButtonQuestion(QuestionType(jo["map"].toArray()[i].toArray()[j].toObject()["type"].toInt()),QuestionMode(jo["map"].toArray()[i].toArray()[j].toObject()["mode"].toInt()));
            btn->setLoc(i,j);
            btn->set_situation(Situation(jo["map"].toArray()[i].toArray()[j].toObject()["sit"].toInt()));
        }
    }

    this->show();
}


