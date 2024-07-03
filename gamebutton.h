#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H

#include <QPushButton>
#include "question.h"
enum Situation{
    AnsweredByYou,
    AnsweredByOpponent,
    Normal,
    AnsweringByOpponent,
    AnsweringByYou,
    AnsweredFalseByYou
};

class GameButton :public QPushButton
{
    Q_OBJECT
public:
    GameButton(QWidget* parent = nullptr);

    void setQuestion(QJsonObject Qobj , QuestionMode mode);
private:
    Question* q;

    Situation situation;

    void update_the_button();
signals:
    QJsonObject get_new_question(QuestionType type);

    void answered_true_to_question(QString objName);

    void answered_false_to_question(QString objName);
public slots:
    void set_button_situation(Situation s);
private slots:
    void answer_true_handeler();

    void answer_false_handeler();

    QJsonObject skiped_clicked_handeler(QuestionType type);

    void clicked_handeler(bool);
};

#endif // GAMEBUTTON_H
