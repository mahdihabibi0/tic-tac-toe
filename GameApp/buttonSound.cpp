#include "buttonSound.h"


void play_normal_button_sound(){
    QSoundEffect buttonSound;
    buttonSound.setSource(QUrl::fromLocalFile("normal.wav"));
    buttonSound.setLoopCount(1);
    buttonSound.setVolume(0.9f);
    buttonSound.play();
}

void play_game_button_sound(){
    QSoundEffect buttonSound;
    buttonSound.setSource(QUrl::fromLocalFile("popular.wav"));
    buttonSound.setLoopCount(1);
    buttonSound.setVolume(0.9f);
    buttonSound.play();
}
