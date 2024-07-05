#ifndef SUBSERVER_H
#define SUBSERVER_H
#include <QTcpServer>
class SubServer :public QTcpServer
{
    Q_OBJECT
public:
    SubServer();
};

#endif // SUBSERVER_H
