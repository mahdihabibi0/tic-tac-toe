#ifndef BASESERVER_H
#define BASESERVER_H
#include <QTcpServer>
#include <QJsonObject>
#include <QVector>
#include <QMap>
#include "gameserver.h"
#include "getippage.h"

class BaseServer :public QTcpServer
{
    Q_OBJECT
public:
    BaseServer();

    void start();
private slots:
    QJsonObject start_game_requested(QString);

    bool setIp(QString ip);

    void new_connection();
private:    
    QMap<int , GameServer*> gameservers;

    GetIPpage gip;

    QHostAddress IP;

    int PORT;
};

#endif // BASESERVER_H
