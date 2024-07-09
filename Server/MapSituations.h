#ifndef MAPSITUATIONS_H
#define MAPSITUATIONS_H

#include <QObject>
#include "mapitem.h"

class MapSituations : public QObject
{
    Q_OBJECT
signals:
    void win();

    void thereIsNoChanceForWin();
public:
    MapSituations();

    void setItemAtPosition(int i , int j , Situation sit);

    Situation getSitOfItemAtPosition(int i , int j);

private:
    bool getChancForWin();

    void checkForWinChance();

    void checkForWin();

    QVector<QVector<MapItem>> map;
};

#endif // MAPSITUATIONS_H
