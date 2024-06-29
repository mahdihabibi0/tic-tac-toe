#ifndef TCPSOCKETMANAGER_H
#define TCPSOCKETMANAGER_H
#include <QTcpSocket>
class TCPSocketManager : public QTcpSocket
{
    Q_OBJECT
public:
    TCPSocketManager();
};

#endif // TCPSOCKETMANAGER_H
