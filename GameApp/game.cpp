#include "game.h"
#include "ui_game.h"
#include "gamebutton.h"
#include <QRandomGenerator>
#include <QVector>

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
    int modeIndex = QRandomGenerator::global()->bounded(0, vector.size()-1);
    btn->setQuestion(get_http_request_question(QuestionType::Multiple,g),vector[modeIndex]);
    vector.erase(vector.begin()+ modeIndex);
    return true;
}

bool get_numerical_question(GameButton *btn,Game* g){
    static QVector<QuestionMode> vector={QuestionMode::normal};
    if(vector.size()==0)
        return false;
    int modeIndex=0;
    btn->setQuestion(get_http_request_question(QuestionType::Multiple,g),vector[modeIndex]);
    vector.erase(vector.begin()+ modeIndex);
    return true;
}

bool get_short_answer_question(GameButton *btn,Game* g){
    static QVector<QuestionMode> vector={QuestionMode::bomb,QuestionMode::normal};
    if(vector.size()==0)
        return false;
    int modeIndex=QRandomGenerator::global()->bounded(0, vector.size()-1);
    btn->setQuestion(get_http_request_question(QuestionType::Multiple,g),vector[modeIndex]);
    vector.erase(vector.begin()+ modeIndex);
    return true;
}

void Game::checking_player_win(int i,int j){
    static int mainDiameter = 0;
    static int subDiameter = 0;
    static QVector<int> rows;
    static QVector<int> cols;
    rows.push_back(i);
    cols.push_back(j);
    if(i==j)
        mainDiameter++;
    if(i+j==2)
        subDiameter++;
    if(rows.count(0)==3||rows.count(1)==3||rows.count(2)==3||cols.count(0)==3||cols.count(1)==3||cols.count(2)==3||mainDiameter==3||subDiameter==3)
        return;
}

void Game::showEvent(QShowEvent *event)
{
    this->showEvent(event);
    bool (*func[3])(GameButton*,Game*) ={get_multiple_question , get_numerical_question,get_short_answer_question };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            GameButton* btn=qobject_cast<GameButton*>(ui->h->itemAtPosition(i,j)->widget());
            btn->setLoc(i,j);
            while(true){
                int random=QRandomGenerator::global()->bounded(0,2);
                if(func[random](btn,this))
                    break;
            }
        }
    }

}

Game::Game(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Game)
{
    ui->setupUi(this);
}

Game::~Game()
{
    delete ui;
}

QJsonObject Game::get_new_question(QuestionType type)
{
    return get_new_question_from_tcSocket(type);
}

void Game::answered_true_to_question_handler(int i, int j)
{
    checking_player_win(i,j);
    emit answered_true(i,j);
}

QJsonObject Game::get_new_question_handler(QuestionType type)
{
    return emit this->get_question(type);
}

void Game::set_button_situation(int i, int j, Situation sit)
{
    GameButton *btn=qobject_cast<GameButton*>(this->ui->h->itemAtPosition(i,j)->widget());

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

void Game::play_again_handler()
{
    this->show();
}

void Game::back_to_home_handler()
{
    delete winnerPage;
    emit show_home_page();
}


