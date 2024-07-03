#include "shortanswerquestion.h"
#include "ui_shortanswerquestion.h"

void ShortAnswerQuestion::set_question_obj(QJsonObject Qobj){
    ui->QTitle->setText(Qobj["category"].toString());

    ui->questionText->setText(Qobj["questionText"].toString());

    answer =  Qobj["answers"].toString();

    this->time = 20;
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
    if(!ui->answerLineEdite->text().size())
        showMessageBoxForQuestion("invalide inputes" , "you should select an answer" , "color : rgba( 255, 0, 0)");
    if(ui->answerLineEdite->text() == answer)
        emit answer_true();
    else
        emit answer_false();
}


void ShortAnswerQuestion::on_skipBtn_clicked()
{
    QJsonObject newQuestion = emit skiped_clicked(QuestionType::Short);

    set_question_obj(newQuestion);
}

ShortAnswerQuestion::~ShortAnswerQuestion(){
    delete ui;
}
