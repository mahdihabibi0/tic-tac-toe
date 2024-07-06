#ifndef BASESERVER_H
#define BASESERVER_H
#include <QTcpServer>
#include <QJsonObject>
#include <QVector>
#include "gameserver.h"
#include "getippage.h"

class BaseServer :public QTcpServer
{
    Q_OBJECT
public:
    BaseServer();
private slots:
    QJsonObject start_game(QString);

    bool setIp(QString ip);

    void new_connection();
private:    
    QVector<GameServer*> gameservers;

    GetIPpage gip;

    QHostAddress IP;

    int PORT;
};

#endif // BASESERVER_H
