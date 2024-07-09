#include "mapstatements.h"
#include <QRandomGenerator>
#include <QDebug>

QVector<QuestionMode> vector_for_multiple_question;

QVector<QuestionMode> vector_for_short_question;

QVector<QuestionMode> vector_for_numerical_question;
MapStatements::MapStatements() {
    for (int i = 0; i < 3; i++) {
        MapStates.push_back(QVector<MapItem>());
        for (int j = 0; j < 3; j++) {
            MapStates[i].push_back(MapItem());
        }
    }

    setUpMap();
}

bool get_multiple_question(MapItem& item){

    if(vector_for_multiple_question.size() == 0)
        return false;

    item.setType(QuestionType::Multiple);

    int modeIndex = QRandomGenerator::global()->bounded(vector_for_multiple_question.size());

    item.setMode(vector_for_multiple_question[modeIndex]);

    vector_for_multiple_question.erase(vector_for_multiple_question.begin()+ modeIndex);

    return true;
}

bool get_numerical_question(MapItem& item){
    if(vector_for_numerical_question.size()==0)
        return false;

    item.setType(QuestionType::Numerical);

    int modeIndex = QRandomGenerator::global()->bounded(vector_for_numerical_question.size());

    item.setMode(vector_for_numerical_question[modeIndex]);

    vector_for_numerical_question.erase(vector_for_numerical_question.begin()+ modeIndex);

    return true;
}

bool get_short_answer_question(MapItem& item){
    if(vector_for_short_question.size()==0)
        return false;

    item.setType(QuestionType::Short);

    int modeIndex = QRandomGenerator::global()->bounded(vector_for_short_question.size());

    item.setMode(vector_for_short_question[modeIndex]);

    vector_for_short_question.erase(vector_for_short_question.begin()  + modeIndex);

    return true;
}

void MapStatements::setUpMap()
{
    vector_for_multiple_question = {QuestionMode::bomb,QuestionMode::bomb,QuestionMode::normal,QuestionMode::normal,QuestionMode::normal,QuestionMode::king};
    vector_for_short_question = {QuestionMode::bomb,QuestionMode::normal};
    vector_for_numerical_question = {QuestionMode::normal};
    bool (*func[3])(MapItem&) ={get_multiple_question , get_numerical_question,get_short_answer_question };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            while(true){
                int random = QRandomGenerator::global()->bounded(3);
                if(func[random](MapStates[i][j]))
                    break;
            }
        }
    }
}

QuestionMode MapStatements::getModeOfItemAt(int i, int j)
{
    return MapStates[i][j].mode;
}

QuestionType MapStatements::getTypeOfItemAt(int i, int j)
{
    return MapStates[i][j].type;
}

QVector<QVector<MapItem> > MapStatements::getAllMap()
{
    return MapStates;
}
