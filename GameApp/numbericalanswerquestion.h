#ifndef NUMBERICALANSWERQUESTION_H
#define NUMBERICALANSWERQUESTION_H

#include "question.h"

namespace Ui {
class NumbericalAnswerQuestion;
}

class NumbericalAnswerQuestion : public Question
{
    Q_OBJECT
public:
    NumbericalAnswerQuestion(QJsonObject Qobj,QuestionMode mode);

    ~NumbericalAnswerQuestion();

private slots:
    void on_submitBtn_clicked();

    void on_skipBtn_clicked();

protected:
    void showEvent(QShowEvent* event) override;

private:
    void set_question_obj(QJsonObject Qobj) override;

    int answer;

    Ui::NumbericalAnswerQuestion *ui;
};

#endif // NUMBERICALANSWERQUESTION_H
