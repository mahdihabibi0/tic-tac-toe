#include "MapSituations.h"
#include <QDebug>

MapSituations::MapSituations():wined(false),losed(false),chanceForWin(false) {
    for (int i = 0; i < 3; ++i) {
        map.push_back(QVector<MapItem>());
        for (int j = 0; j < 3; ++j) {
            map[i].push_back(MapItem());
        }
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            setup(map , i , j);
        }
    }
}

void MapSituations::setItemAtPosition(int i, int j, Situation sit)
{
    map[i][j].sit = sit;

    checkForWinChance();

    checkForWin();

    checkForLose();
}

Situation MapSituations::getSitOfItemAtPosition(int i, int j)
{
    return map[i][j].sit;
}

void MapSituations::checkForWin()
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
            qDebug() << "checking for " << i << "," << j << " wining item that is " << map[i][j].sit;
            if(map[i][j].checkForWin()){
                wined = true;
            }
        }
}

void MapSituations::checkForLose()
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
            qDebug() << "checking for " << i << "," << j << " losing item that is " << map[i][j].sit;
            if(map[i][j].checkForLose()){
                losed = true;
            }
        }
}

void MapSituations::checkForWinChance()
{
    bool check = false;

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if(!check && map[i][j].checkForWinChance())
                check = true;

    if(!check)
        emit thereIsNoChanceForWin();
}
