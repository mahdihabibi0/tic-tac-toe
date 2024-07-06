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
    void read_handler();
private:
    QString username;
    QTcpSocket* socket;
};

#endif // GAMESOCKETMANAGER_H
