#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>
class SocketManager : public QObject
{
    Q_OBJECT
public:
    SocketManager();
public slots:
    void read_handler();

    void disconnect_handler();
};

#endif // SOCKETMANAGER_H
