#include "question.h"
#include <QMessageBox>
#include <QEvent>

bool Question::skipButtonActive = true;

Question::Question(QuestionMode mode):QDialog(nullptr),mode(mode){

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
    this->showEvent(event);

    if(timer)
        timer->deleteLater();

    timer = new Timer(20);

    QObject::connect(timer, &Timer::time_finished, [&](){
        emit answer_false();
        timer->deleteLater();
    });

    QObject::connect(timer, &Timer::timeout, [&](){
        emit display_timer(timer->get_rem_time());
    });

    QObject::connect(this , &Question::skiped_clicked , [&](){
        static int count = 0;
        if(++count == 2)
            lock_skip_button();
        timer->deleteLater();
    });
}

Question::~Question(){
    delete timer;
}

bool Question::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::Close) {

        timer->stop();

        return true;
    }

    return Question::eventFilter(obj, event);
}
