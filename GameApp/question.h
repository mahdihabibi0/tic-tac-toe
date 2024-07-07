#ifndef QUESTION_H
#define QUESTION_H
#pragma once
#include <QException>
#include <QDialog>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>

enum QuestionMode{
    normal,
    bomb,
    king
};

enum QuestionType{
    Multiple,
    Numerical,
    Short
};

void showMessageBoxForQuestion(QString title , QString Text , QString styleSheet);

class Question : public QDialog
{
    Q_OBJECT
signals:
    void answer_true();

    void answer_false();

    void display_timer(int);

    QJsonObject skiped_clicked(QuestionType);
public:
    Question(QuestionMode mode);

    static void lock_skip_button();

    ~Question();

protected:
    int time;

    QuestionMode mode;

    virtual void showEvent(QShowEvent* event) override;

    bool eventFilter(QObject* obj, QEvent* event) override;

    virtual void set_question_obj(QJsonObject Qobj) = 0;

    static bool skipButtonActive;
private:
    QTimer* timer;

private slots:
    void send_time();

};

#endif // QUESTION_H
