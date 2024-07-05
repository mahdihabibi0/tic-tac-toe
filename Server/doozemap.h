#ifndef DOOZEMAP_H
#define DOOZEMAP_H

#include <QObject>
#include <QVector>

enum Situation{
    answerdTrue,
    answerdFalse,
    normal,
    answeredByOppenent
};
struct mapItem
{
    void set(mapItem* left = NULL,
            mapItem* leftOtop = NULL,
            mapItem* top = NULL,
            mapItem* topOright = NULL,
            mapItem* right = NULL,
            mapItem* rightObuttom= NULL,
            mapItem* buttom = NULL,
            mapItem* buttomOleft = NULL);

    bool checkForWin();

    bool checkForWinChance();

    mapItem* left;
    mapItem* leftOtop;
    mapItem* top;
    mapItem* topOright;
    mapItem* right;
    mapItem* rightObuttom;
    mapItem* buttom;
    mapItem* buttomOleft;

    Situation sit;
};
class DoozeMap : public QObject
{
    Q_OBJECT
signals:
    void win();

    void thereIsNoChanceForWin();

public:
    DoozeMap();

    void setItemAtPosition(int i , int j , Situation sit);
private:
    void checkForWin();

    void checkForWinChance();

    QVector<QVector<mapItem>> map;
};

#endif // DOOZEMAP_H
