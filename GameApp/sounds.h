#ifndef SOUNDS_H
#define SOUNDS_H
#include <QSoundEffect>
class sounds : public QObject
{
public:
    sounds();

    void play_normal_button_sound();

    void play_game_button_sound();
private:
    QSoundEffect * NormalSound;

    QSoundEffect * GameButtonSount;
};



#endif // SOUNDS_H
