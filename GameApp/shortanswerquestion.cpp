#include "shortanswerquestion.h"
#include "ui_shortanswerquestion.h"

void ShortAnswerQuestion::set_question_obj(QJsonObject Qobj){
    ui->QTitle->setText(Qobj["category"].toString());

    ui->questionText->setText(Qobj["questionText"].toString());

    answer =  Qobj["answers"].toString();
}

ShortAnswerQuestion::ShortAnswerQuestion(QJsonObject Qobj,QuestionMode mode)
    : Question(mode)
    , ui(new Ui::ShortAnswerQuestion)
{
    ui->setupUi(this);

    switch (mode) {
    case QuestionMode::bomb:
    {
        ui->QMode->setText("Bomb");
        break;
    }
    case QuestionMode::king:
    {
        ui->QMode->setText("King");
        break;
    }
    case QuestionMode::normal:
    {
        ui->QMode->setText("Normal");
        break;
    }
    }

    this->set_question_obj(Qobj);

    QObject::connect(this , SIGNAL(display_timer(int)) , ui->Timer , SLOT(display(int)));
}

void ShortAnswerQuestion::on_submitBtn_clicked()
{
    play_game_button_sound();
    if(!ui->answerLineEdite->text().size())
        showMessageBoxForQuestion("invalide inputes" , "you should select an answer" , "color : rgba( 255, 0, 0)");
    if(ui->answerLineEdite->text() == answer)
        emit answer_true();
    else
        emit answer_false();
}


void ShortAnswerQuestion::on_skipBtn_clicked()
{
    play_normal_button_sound();

    emit skiped_clicked();

    this->close();
}

void ShortAnswerQuestion::showEvent(QShowEvent *event)
{
    qDebug()<<"skip button sit :" << Question::skipButtonActive;

    ui->skipBtn->setEnabled(Question::skipButtonActive);

    Question::showEvent(event);

}

ShortAnswerQuestion::~ShortAnswerQuestion(){
    delete ui;
}
