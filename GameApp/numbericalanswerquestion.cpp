#include "numbericalanswerquestion.h"
#include "ui_numbericalanswerquestion.h"

void NumbericalAnswerQuestion::set_question_obj(QJsonObject Qobj){
    ui->QTitle->setText(Qobj["category"].toString());

    ui->questionText->setText(Qobj["questionText"].toString());

    answer = Qobj["answer"].toInt();
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
    playNormalSound();
    if(ui->answerSpinBox->value() == answer)
        emit answer_true();
    else
        emit answer_false();


    timer->deleteLater();

    this->close();

    this->deleteLater();
}

void NumbericalAnswerQuestion::on_skipBtn_clicked(){
    playNormalSound();

    emit skiped_clicked();

    this->close();

    this->deleteLater();
}

void NumbericalAnswerQuestion::showEvent(QShowEvent *event)
{
    qDebug()<<"skip button sit :" << Question::skipButtonActive;

    ui->skipBtn->setEnabled(Question::skipButtonActive);

    Question::showEvent(event);
}

NumbericalAnswerQuestion::~NumbericalAnswerQuestion(){
    delete ui;
}
