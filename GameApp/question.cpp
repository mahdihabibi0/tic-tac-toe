#include "question.h"
#include <QMessageBox>
#include <QEvent>

bool Question::skipButtonActive = true;

Question::Question(QuestionMode mode):QDialog(nullptr),mode(mode),timer(nullptr){

}

void Question::lock_skip_button()
{
    Question::skipButtonActive = false;
}

void showMessageBoxForQuestion(QString title , QString Text , QString styleSheet){
    QMessageBox mes;

    mes.setIcon(QMessageBox::Warning);

    mes.setWindowTitle(title);

    mes.setText(Text);

    mes.setStyleSheet(styleSheet);

    mes.setStandardButtons(QMessageBox::Ok);

    mes.exec();
}

void Question::showEvent(QShowEvent* event) {

    timer = new Timer(20);

    QObject::connect(timer, &Timer::time_finished, [&](){
        emit answer_false();
    });

    QObject::connect(timer, &Timer::timeout, [&](int time){
        emit display_timer(time);
    });

    QObject::connect(this , &Question::skiped_clicked , [&](){
        static int count = 0;

        if(++count == 2)
            lock_skip_button();

        timer->deleteLater();

    });

    QDialog::showEvent(event);

}


Question::~Question(){
}
