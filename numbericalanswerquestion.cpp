#include "numbericalanswerquestion.h"
#include "ui_numbericalanswerquestion.h"

void NumbericalAnswerQuestion::set_question_obj(QJsonObject Qobj){
    ui->QTitle->setText(Qobj["category"].toString());

    ui->questionText->setText(Qobj["questionText"].toString());

    answer = Qobj["answer"].toInt();

    this->time = 20;
}

NumbericalAnswerQuestion::NumbericalAnswerQuestion(QJsonObject Qobj,QuestionMode mode) :
    Question(mode),
    ui(new Ui::NumbericalAnswerQuestion)
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

void NumbericalAnswerQuestion::on_submitBtn_clicked()
{
    if(ui->answerSpinBox->value() == answer)
        emit answer_true();
    else
        emit answer_false();
}

void NumbericalAnswerQuestion::on_skipBtn_clicked(){
    QJsonObject newQuestion = emit skiped_clicked(QuestionType::Numerical);

    set_question_obj(newQuestion);
}

NumbericalAnswerQuestion::~NumbericalAnswerQuestion(){
    delete ui;
}
