#include "multipleanswerquestion.h"
#include "ui_multipleanswerquestion.h"

void MultipleAnswerQuestion::set_question_obj(QJsonObject Qobj){
    ui->QTitle->setText(Qobj["category"].toString());

    ui->questionText->setText(Qobj["questionText"].toString());

    QJsonArray answers =  Qobj["answers"].toArray();

    ui->answer1RB->setText(answers[0].toObject()["text"].toString());

    ui->answer2RB->setText(answers[1].toObject()["text"].toString());

    ui->answer3RB->setText(answers[2].toObject()["text"].toString());

    ui->answer4RB->setText(answers[3].toObject()["text"].toString());

    for (int i = 0; i < 4; ++i)
        if(answers[i].toObject()["isCorrect"].toBool())
            correctAnswer = i;
}

MultipleAnswerQuestion::MultipleAnswerQuestion(QJsonObject Qobj , QuestionMode mode)
    : Question(mode)
    , ui(new Ui::MultipleAnswerQuestion)
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
bool MultipleAnswerQuestion::checkAnswer(){
    switch (correctAnswer) {
    case 0:
        if(ui->answer1RB->isChecked())
            break;
    case 1:
        if(ui->answer2RB->isChecked())
            break;
    case 2:
        if(ui->answer3RB->isChecked())
            break;
    case 3:
        if(ui->answer4RB->isChecked())
            break;
    default:
        if(ui->answer1RB->isChecked() ||
            ui->answer2RB->isChecked() ||
            ui->answer3RB->isChecked() ||
            ui->answer4RB->isChecked())
            return false;
        throw QException();
    }
    return true;
}

void MultipleAnswerQuestion::on_skipBtn_clicked()
{
    playNormalSound();

    emit skiped_clicked();

    this->close();

    this->deleteLater();
}

void MultipleAnswerQuestion::showEvent(QShowEvent *event)
{
    qDebug()<<"skip button sit :" << Question::skipButtonActive;

    ui->skipBtn->setEnabled(Question::skipButtonActive);

    Question::showEvent(event);
}

void MultipleAnswerQuestion::on_submitBtn_clicked()
{
    playNormalSound();
    try {
        if(checkAnswer())
            emit answer_true();
        else
            emit answer_false();

        this->close();

        timer->deleteLater();

        this->deleteLater();
    } catch (...) {
        showMessageBoxForQuestion("invalide inputes" , "you should select an answer" , "color : rgba( 255, 0, 0)");
    }
}

MultipleAnswerQuestion::~MultipleAnswerQuestion(){
    delete ui;
}
