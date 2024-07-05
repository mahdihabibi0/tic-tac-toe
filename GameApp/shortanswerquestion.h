#ifndef SHORTANSWERQUESTION_H
#define SHORTANSWERQUESTION_H

#include "question.h"

namespace Ui {
class ShortAnswerQuestion;
}

class ShortAnswerQuestion : public Question
{
    Q_OBJECT
public:
    ShortAnswerQuestion(QJsonObject Qobj,QuestionMode mode);

    ~ShortAnswerQuestion();
private slots:
    void on_submitBtn_clicked();

    void on_skipBtn_clicked();

private:
    QString answer;

    void set_question_obj(QJsonObject Qobj) override;

    Ui::ShortAnswerQuestion *ui;
};

#endif // SHORTANSWERQUESTION_H
