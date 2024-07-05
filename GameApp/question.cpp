#include "question.h"
#include <QMessageBox>
#include <QEvent>

Question::Question(QuestionMode mode):QDialog(nullptr),mode(mode),timer(new QTimer(this)){

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

    this->timer->start(1000);


    QObject::connect(timer, SIGNAL(timeout()), this , SLOT(send_time()));

}

Question::~Question(){
    delete timer;
}

void Question::send_time(){
    if(time == 0)
        emit answer_false();
    emit display_timer(time--);
}

bool Question::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::Close) {

        timer->stop();

        return true;
    }

    return Question::eventFilter(obj, event);
}
