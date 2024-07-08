#ifndef TIMER_H
#define TIMER_H
#include <QTimer>

class Timer : public QTimer
{
    Q_OBJECT
public:
    Timer(int time);

    int get_rem_time();

    bool get_time_valide();
private:
    int time;

    bool timeValide;
private slots:
    void time_handler();
signals:
    void time_finished();
};

#endif // TIMER_H
