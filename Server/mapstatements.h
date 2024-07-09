#ifndef MAPSTATEMENTS_H
#define MAPSTATEMENTS_H
#include "mapitem.h"

class MapStatements
{
public:
    MapStatements();

    void setUpMap();

    QuestionMode getModeOfItemAt(int i , int j);

    QuestionType getTypeOfItemAt(int i , int j);

    QVector<QVector<MapItem>> getAllMap();
private:
    QVector<QVector<MapItem>> MapStates;
};

#endif // MAPSTATEMENTS_H
