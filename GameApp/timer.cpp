#include "timer.h"

Timer::Timer(int time) : time(time) , timeValide(time)
{
    this->start(1000);
    QObject::connect(this , SIGNAL(timeout()) , this , SLOT(time_handler()));
}

void Timer::reset(int time)
{
    this->time = time;

    this->timeValide = time;

    this->start(1000);
}

int Timer::get_rem_time()
{
    if(time <= 0)
        return 0;

    return time;
}

bool Timer::get_time_valide()
{
    return timeValide;
}

void Timer::time_handler()
{
    time--;
    if(time <= 0)
    {
        emit time_finished();

        this->deleteLater();
    }
    else
        emit timeout(time);
}
