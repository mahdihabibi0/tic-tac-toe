#include "doozemap.h"

QVector<Situation> goodSits = {Situation::answerdTrue , Situation::normal};

mapItem* getIndex(QVector<QVector<mapItem>>& map , int i , int j){
    if(i>=0 && i<3 && j >= 0 && j < 3)
        return &map[i][j];
    else
        return nullptr;
}

void setup(QVector<QVector<mapItem>>& map , int i , int j){
    mapItem* left;
    mapItem* leftOtop;
    mapItem* top;
    mapItem* topOright;
    mapItem* right;
    mapItem* rightObuttom;
    mapItem* buttom;
    mapItem* buttomOleft;

    left = getIndex(map , i - 1 , j);
    leftOtop = getIndex(map , i - 1 , j - 1);
    top = getIndex(map , i , j - 1);
    topOright = getIndex(map , i + 1 , j + 1);
    right = getIndex(map , i , j + 1);
    rightObuttom = getIndex(map , i + 1, j + 1);
    buttom = getIndex(map , i + 1 , j);
    buttomOleft = getIndex(map , i + 1  , j -1 );

    map[i][j].set(left , leftOtop , top , topOright , right , rightObuttom , buttom , buttomOleft);
}

DoozeMap::DoozeMap() {
    for (int i = 0; i < 3; ++i) {
        map.push_back(QVector<mapItem>());
        for (int j = 0; j < 3; ++j) {
            map[i].push_back(mapItem());
        }
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            setup(map , i , j);
        }
    }
}

void DoozeMap::setItemAtPosition(int i, int j, Situation sit)
{
    map[i][j].sit = sit;
    checkForWinChance();
    checkForWin();
}

void DoozeMap::checkForWin()
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if(map[i][j].checkForWin())
                emit win();
}

void DoozeMap::checkForWinChance()
{
    bool check = false;

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if(!check && map[i][j].checkForWinChance())
                check = true;

    if (!check)
        emit thereIsNoChanceForWin();
}

void mapItem::set(mapItem *left,
                 mapItem *leftOtop,
                 mapItem *top,
                 mapItem *topOright,
                 mapItem *right,
                 mapItem *rightObuttom,
                 mapItem *buttom,
                 mapItem *buttomOleft)
{
    this->left = left;
    this->leftOtop = leftOtop;
    this->top = top;
    this->topOright = topOright;
    this->right = right;
    this->rightObuttom = rightObuttom;
    this->buttom = buttom;
    this->buttomOleft = buttomOleft;

    sit = Situation::normal;
}

bool mapItem::checkForWin()
{
    if(left && right && left->sit == Situation::answerdTrue && right->sit == Situation::answerdTrue)
        return true;

    if(top && buttom && top->sit == Situation::answerdTrue && buttom->sit == Situation::answerdTrue)
        return true;

    if(topOright && buttomOleft && topOright->sit == Situation::answerdTrue && buttomOleft->sit == Situation::answerdTrue)
        return true;

    if(leftOtop && rightObuttom && leftOtop->sit == Situation::answerdTrue && rightObuttom->sit == Situation::answerdTrue)
        return true;

    return false;
}

bool mapItem::checkForWinChance()
{
    if(left && right && goodSits.count(left->sit) && goodSits.count(right->sit))
        return true;

    if(top && buttom && goodSits.count(top->sit) && goodSits.count(buttom->sit))
        return true;

    if(topOright && buttomOleft && goodSits.count(topOright->sit) && goodSits.count(buttomOleft->sit))
        return true;

    if(leftOtop && rightObuttom && goodSits.count(leftOtop->sit) && goodSits.count(rightObuttom->sit))
        return true;

    return false;
}
