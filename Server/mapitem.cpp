#include "mapitem.h"

MapItem* getIndex(QVector<QVector<MapItem>>& map , int i , int j){
    if(i>=0 && i<3 && j >= 0 && j < 3)
        return &map[i][j];
    else
        return nullptr;
}

void setup(QVector<QVector<MapItem>>& map , int i , int j){
    MapItem* left;
    MapItem* leftOtop;
    MapItem* top;
    MapItem* topOright;
    MapItem* right;
    MapItem* rightObuttom;
    MapItem* buttom;
    MapItem* buttomOleft;

    left = getIndex(map , i , j - 1);
    leftOtop = getIndex(map , i - 1 , j - 1);
    top = getIndex(map , i - 1, j);
    topOright = getIndex(map , i - 1 , j + 1);
    right = getIndex(map , i, j + 1);
    rightObuttom = getIndex(map , i + 1 , j + 1);
    buttom = getIndex(map , i + 1 , j);
    buttomOleft = getIndex(map , i + 1  , j - 1 );

    map[i][j].set(left , leftOtop , top , topOright , right , rightObuttom , buttom , buttomOleft);

    map[i][j].sit = Situation::Normal;
}

void MapItem::setMode(QuestionMode m)
{
    mode = m;
}

void MapItem::setType(QuestionType t)
{
    type = t;
}

void MapItem::set(MapItem *left,
                  MapItem *leftOtop,
                  MapItem *top,
                  MapItem *topOright,
                  MapItem *right,
                  MapItem *rightObuttom,
                  MapItem *buttom,
                  MapItem *buttomOleft)
{
    this->left = left;
    this->leftOtop = leftOtop;
    this->top = top;
    this->topOright = topOright;
    this->right = right;
    this->rightObuttom = rightObuttom;
    this->buttom = buttom;
    this->buttomOleft = buttomOleft;
}

bool MapItem::checkForWin()
{
    if(sit == Situation::AnsweredByYou){
        if(left && right && left->sit == Situation::AnsweredByYou && right->sit == Situation::AnsweredByYou)
            return true;

        if(top && buttom && top->sit == Situation::AnsweredByYou && buttom->sit == Situation::AnsweredByYou)
            return true;

        if(topOright && buttomOleft && topOright->sit == Situation::AnsweredByYou && buttomOleft->sit == Situation::AnsweredByYou)
            return true;

        if(leftOtop && rightObuttom && leftOtop->sit == Situation::AnsweredByYou && rightObuttom->sit == Situation::AnsweredByYou)
            return true;

    }
    return false;
}

bool MapItem::checkForLose()
{
    if(sit == Situation::AnsweredByOpponent){
        if(left && right && left->sit == Situation::AnsweredByOpponent && right->sit == Situation::AnsweredByOpponent)
            return true;

        if(top && buttom && top->sit == Situation::AnsweredByOpponent && buttom->sit == Situation::AnsweredByOpponent)
            return true;

        if(topOright && buttomOleft && topOright->sit == Situation::AnsweredByOpponent && buttomOleft->sit == Situation::AnsweredByOpponent)
            return true;

        if(leftOtop && rightObuttom && leftOtop->sit == Situation::AnsweredByOpponent && rightObuttom->sit == Situation::AnsweredByOpponent)
            return true;

    }
    return false;
}

bool MapItem::checkForWinChance()
{
    static QVector<Situation> goodSits = {Situation::AnsweredByYou ,
                                   Situation::Normal ,
                                   Situation::AnsweringByYou  ,
                                   Situation::AnsweringByOpponent};

    if(goodSits.count(sit)){
        if(left && right && goodSits.count(left->sit) && goodSits.count(right->sit))
            return true;

        if(top && buttom && goodSits.count(top->sit) && goodSits.count(buttom->sit))
            return true;

        if(topOright && buttomOleft && goodSits.count(topOright->sit) && goodSits.count(buttomOleft->sit))
            return true;

        if(leftOtop && rightObuttom && goodSits.count(leftOtop->sit) && goodSits.count(rightObuttom->sit))
            return true;
    }

    return false;
}
