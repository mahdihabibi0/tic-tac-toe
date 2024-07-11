#include "gamebutton.h"

#include "multipleanswerquestion.h"
#include "numbericalanswerquestion.h"
#include "shortanswerquestion.h"
#include "buttonSound.h"
//#include <QObject>
void GameButton::update_the_button(){

    switch (situation) {
    case Situation::AnsweredByYou:
        this->setStyleSheet("border-image: url(:/images/x.jpg);");
        this->setEnabled(false);
        break;
    case Situation::AnsweredByOpponent:
        this->setStyleSheet("border-image: url(:/images/o.jpg);");
        this->setEnabled(false);
        break;
    case Situation::AnsweringByOpponent:
        this->setStyleSheet("border-image: url(:/images/busy.jpg);");
        this->setEnabled(true);
        break;
    case Situation::AnsweringByYou:
        this->setStyleSheet("border-image: url(:/images/question.jpg);");
        this->setEnabled(false);
        break;
    case Situation::AnsweredFalseByYou:
        this->setStyleSheet("border-image: url(:/images/lock.jpg);");
        this->setEnabled(false);
        break;
    default:
        this->setStyleSheet("border-image: url(:/images/line.jpg);");
        this->setEnabled(true);
        break;
    }

}

GameButton::GameButton(QWidget* parent):QPushButton(parent) , situation(Situation::Normal) {
    update_the_button();
    QObject::connect(this,SIGNAL(clicked(bool)),this,SLOT(clicked_handeler(bool)));
}

void GameButton::setQuestion(QJsonObject Qobj , QuestionMode mode){
    qDebug() << "set buutton question is started";
    QString type = Qobj["type"].toString();

    if (type == "short")
        q = new ShortAnswerQuestion(Qobj , mode);

    else if(type == "multiple")
        q = new MultipleAnswerQuestion(Qobj , mode);

    else if(type == "number")
        q = new NumbericalAnswerQuestion(Qobj , mode);

    QObject::connect(q , SIGNAL(skiped_clicked()) , this , SLOT(skiped_clicked_handeler()));

    QObject::connect(q , SIGNAL(answer_true()) , this , SLOT(answer_true_handeler()));

    QObject::connect(q , SIGNAL(answer_false()) , this , SLOT(answer_false_handeler()));

    qDebug() << "set buutton question is fineshed";
}

void GameButton::set_situation(Situation s)
{
    this->situation = s;

    update_the_button();
}


void GameButton::answer_true_handeler(){
    situation = Situation::AnsweredByYou;

    update_the_button();

    emit answered_true_to_question(loc.first , loc.second);
}

void GameButton::answer_false_handeler(){
    situation = Situation::AnsweredFalseByYou;

    update_the_button();

    emit answered_false_to_question(loc.first , loc.second);
}

void GameButton::skiped_clicked_handeler(){
    emit set_back_button_to_normal(loc.first , loc.second);
    this->set_situation(Situation::Normal);
}

void GameButton::clicked_handeler(bool){
    playNormalSound();
    if(situation == Situation::AnsweringByOpponent)
        showMessageBoxForQuestion("bad select" , "it is answering\nby your opponent" , "color : rgba(255 , 0 , 0);");
    else{
        situation = Situation::AnsweringByYou;
        update_the_button();
        this->setQuestion(emit get_new_question(typeOfQuestion) , modeOfQuestion);
        q->setWindowModality(Qt::ApplicationModal);
        q->setWindowFlags(q->windowFlags() & ~Qt::WindowCloseButtonHint);
        qDebug() << "question show mothod called";
        q->show();

        qDebug() << "emited ansering to question";
        emit is_answering_to_question(loc.first , loc.second);
    }
}

void GameButton::setLoc(int i , int j){
    loc.first = i;
    loc.second  = j;
}

void GameButton::setUpButtonQuestion(QuestionType type, QuestionMode mode)
{
    typeOfQuestion = type;

    modeOfQuestion = mode;
}
