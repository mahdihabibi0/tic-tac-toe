#ifndef MAPITEM_H
#define MAPITEM_H
#include <QVector>
// ( 2 , 1 , 0 )
// ( 1 , 0 , 0 )
// ( 0 , 2 , 0 )
// ( 0 , 0 , 0 )
// ( 2 , 0 , 0 )
// ( 0 , 0 , 0 )
// ( 0 , 1 , 0 )
// ( 0 , 0 , 0 )
// ( 0 , 1 , 0 )
enum Situation{
    AnsweredByYou,
    AnsweredByOpponent,
    Normal,
    AnsweringByOpponent,
    AnsweringByYou,
    AnsweredFalseByYou
};

enum QuestionMode{
    normal,
    bomb,
    king
};

enum QuestionType{
    Multiple,
    Numerical,
    Short
};

struct MapItem
{
    void setMode(QuestionMode m);

    void setType(QuestionType t);

    void set(MapItem* left = nullptr,
             MapItem* leftOtop = nullptr,
             MapItem* top = nullptr,
             MapItem* topOright = nullptr,
             MapItem* right = nullptr,
             MapItem* rightObuttom= nullptr,
             MapItem* buttom = nullptr,
             MapItem* buttomOleft = nullptr);

    bool checkForWin();

    bool checkForWinChance();

    MapItem* left;
    MapItem* leftOtop;
    MapItem* top;
    MapItem* topOright;
    MapItem* right;
    MapItem* rightObuttom;
    MapItem* buttom;
    MapItem* buttomOleft;

    Situation sit;

    QuestionMode mode;

    QuestionType type;
};

void setup(QVector<QVector<MapItem>>& map , int i , int j);

#endif // MAPITEM_H
