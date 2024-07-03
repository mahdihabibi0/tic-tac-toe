#ifndef MULTIPLEANSWERQUESTION_H
#define MULTIPLEANSWERQUESTION_H

#include "question.h"

namespace Ui {
class MultipleAnswerQuestion;
}

class MultipleAnswerQuestion : public Question
{
    Q_OBJECT
public:
    MultipleAnswerQuestion(QJsonObject Qobj,QuestionMode mode);

    ~MultipleAnswerQuestion();

private slots:
    void on_submitBtn_clicked();

    void on_skipBtn_clicked();
private:
    void set_question_obj(QJsonObject Qobj) override;

    bool checkAnswer();

    int correctAnswer;

    Ui::MultipleAnswerQuestion *ui;
};

#endif // MULTIPLEANSWERQUESTION_H
