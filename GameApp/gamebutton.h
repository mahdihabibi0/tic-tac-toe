#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H

#include <QPushButton>
#include "question.h"
enum Situation{
    AnsweredByOpponent,
    AnsweredFalseByYou,
    AnsweredByYou,
    AnsweringByOpponent,
    Normal,
    AnsweringByYou
};

class GameButton :public QPushButton
{
    Q_OBJECT
public:
    GameButton(QWidget* parent = nullptr);

    void setQuestion(QJsonObject Qobj , QuestionMode mode);

    void set_situation(Situation s);

    void setLoc(int i , int j);

    void setUpButtonQuestion(QuestionType , QuestionMode);
private:
    QPair<int , int> loc;

    QuestionMode modeOfQuestion;

    QuestionType typeOfQuestion;

    Question* q;

    Situation situation;

    void update_the_button();
signals:
    QJsonObject get_new_question(QuestionType type);

    void answered_true_to_question(int i , int j);

    void answered_false_to_question(int i , int j);

    void is_answering_to_question(int i, int j);

    void set_back_button_to_normal(int i, int j);
private slots:
    void answer_true_handeler();

    void answer_false_handeler();

    void skiped_clicked_handeler();

    void clicked_handeler(bool);
};

#endif // GAMEBUTTON_H
