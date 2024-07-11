#include "sounds.h"

sounds::sounds()
{


    NormalSound = new QSoundEffect();
    NormalSound->setLoopCount(1);
    NormalSound->setVolume(0.9f);
    QUrl path2 = QUrl::fromLocalFile(":/sounds/normal.wav");
    NormalSound->setSource(path2);

    GameButtonSount = new QSoundEffect();
    GameButtonSount->setLoopCount(1);
    GameButtonSount->setVolume(0.9f);
    QUrl path1 = QUrl::fromLocalFile(":/sounds/popular.wav");
    GameButtonSount->setSource(path1);

}

void sounds::play_normal_button_sound(){

    NormalSound->play();
}

void sounds::play_game_button_sound(){

    GameButtonSount->play();
}


