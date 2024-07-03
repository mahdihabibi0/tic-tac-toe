#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H
#include <QPushButton>
enum situations{
    circle,
    triangle
};

class GameButton : QPushButton
{
    Q_OBJECT
public:
    GameButton();
private:
    // Questioon Q;
    situations situation;
};

#endif // GAMEBUTTON_H
