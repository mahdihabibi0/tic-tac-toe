#include "MapSituations.h"

MapSituations::MapSituations() {
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
}

Situation MapSituations::getSitOfItemAtPosition(int i, int j)
{
    return map[i][j].sit;
}

void MapSituations::checkForWin()
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if(map[i][j].checkForWin())
                emit win();
}

void MapSituations::checkForWinChance()
{
    bool check = false;

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if(!check && map[i][j].checkForWinChance())
                check = true;

    if (!check)
        emit thereIsNoChanceForWin();
}
