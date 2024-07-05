#ifndef GAMESOCKETMANAGER_H
#define GAMESOCKETMANAGER_H
#include <QTcpSocket>
#include <QObject>

class GameSocketManager : public QObject
{
    Q_OBJECT
public:
    GameSocketManager(QTcpSocket* socket);
private slots:

private:
    QTcpSocket* socket;
};

#endif // GAMESOCKETMANAGER_H
