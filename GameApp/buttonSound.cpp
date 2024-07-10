#include "buttonSound.h"
#include "sounds.h"

void playPup()
{
    sounds s;
    s.play_game_button_sound();
}

void playNormalSound()
{
    sounds s;
    s.play_normal_button_sound();
}
