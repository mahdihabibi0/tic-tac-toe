#include "game.h"
#include "ui_game.h"
#include "gamebutton.h"
#include <QRandomGenerator>
#include <QVector>
#include "userHandler.h"

void Game::create_dialog(QString situation){
    QLabel *label;
    QPushButton *playAgain;
    QPushButton *backHome;
    winnerPage=new QDialog();
    winnerPage->resize(800,600);
    label = new QLabel(winnerPage);
    label->setObjectName("label");
    label->setGeometry(QRect(210, 20, 371, 121));
    label->setText(situation);
    playAgain = new QPushButton(winnerPage);
    playAgain->setObjectName("playAgain");
    playAgain->setGeometry(QRect(320, 330, 151, 71));
    backHome = new QPushButton(winnerPage);
    backHome->setObjectName("backHome");
    backHome->setGeometry(QRect(320, 330, 151, 71));
    QObject::connect(playAgain,SIGNAL(clicked(bool)),this,SLOT(play_again_handler()));
    QObject::connect(backHome,SIGNAL(clicked(bool)),this,SLOT(back_to_home_handler()));
    winnerPage->show();

}

QJsonObject get_http_request_question(QuestionType type,Game* g) {
    return emit g->get_question(type);
}

bool get_multiple_question(GameButton *btn,Game* g){
    static QVector<QuestionMode> vector={QuestionMode::bomb,QuestionMode::bomb,QuestionMode::normal,QuestionMode::normal,QuestionMode::normal,QuestionMode::king};
    if(vector.size()==0)
        return false;
    int modeIndex = QRandomGenerator::global()->bounded(vector.size());
    btn->setUpButtonQuestion(QuestionType::Multiple,vector[modeIndex]);
    vector.erase(vector.begin()+ modeIndex);
    return true;
}

bool get_numerical_question(GameButton *btn,Game* g){
    static QVector<QuestionMode> vector={QuestionMode::normal};
    if(vector.size()==0)
        return false;
    int modeIndex = QRandomGenerator::global()->bounded(vector.size());
    btn->setUpButtonQuestion(QuestionType::Numerical,vector[modeIndex]);
    vector.erase(vector.begin()+ modeIndex);
    return true;
}

bool get_short_answer_question(GameButton *btn,Game* g){
    static QVector<QuestionMode> vector={QuestionMode::bomb,QuestionMode::normal};
    if(vector.size()==0)
        return false;
    int modeIndex = QRandomGenerator::global()->bounded(vector.size());
    btn->setUpButtonQuestion(QuestionType::Short,vector[modeIndex]);
    vector.erase(vector.begin()+ modeIndex);
    return true;
}

void Game::showEvent(QShowEvent *event)
{
    bool (*func[3])(GameButton*,Game*) ={get_multiple_question , get_numerical_question,get_short_answer_question };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            GameButton* btn = qobject_cast<GameButton*>(ui->h->itemAtPosition(i,j)->widget());
            btn->setLoc(i,j);
            while(true){
                int random = QRandomGenerator::global()->bounded(3);
                if(func[random](btn,this))
                    break;
            }
        }
    }
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
    create_dialog("You Won");
}

void Game::player_lose()
{
    this->close();
    create_dialog("You Lose");
}

void Game::game_drawed()
{
    this->close();
    create_dialog("Game Drawed");
}

void Game::start(QString ChallengerName)
{
    ui->player1Username->setText(get_user_name());
    ui->player2Username->setText(ChallengerName);
}

void Game::play_again_handler()
{
    this->show();
}

void Game::back_to_home_handler()
{
    delete winnerPage;
    emit show_home_page();
}


