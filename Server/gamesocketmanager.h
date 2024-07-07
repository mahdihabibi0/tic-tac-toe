#ifndef GAMESOCKETMANAGER_H
#define GAMESOCKETMANAGER_H
#include <QTcpSocket>
#include <QObject>
#include "doozemap.h"

// enum Situation{
//     answerdTrue,
//     answerdFalse,
//     normal,
//     answeredByOppenent
// };

class GameSocketManager : public QObject
{
    Q_OBJECT
public:
    GameSocketManager(QTcpSocket* socket);

    bool getChanceForWin();

private slots:
    void read_handler();

    void thereIsNoChanceForWin_handler();

    void win_handler(QString name);

private:
    QString username;

    QTcpSocket* socket;

    DoozeMap map;

    bool chanceForWin;
signals:
    void player_answered_true(QPair<int,int>);

    void player_set_button_normal(QPair<int,int>);

    void player_answering(QPair<int,int>);

    void noChanceForWin();

    void playerWin(QString name);

public slots:
    void challanger_answered_true(QPair<int,int>);

    void challanger_answering(QPair<int,int>);

    void challanger_set_button_back_to_normal(QPair<int,int>);


};

#endif // GAMESOCKETMANAGER_H
