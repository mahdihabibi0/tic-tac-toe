#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMap>

class SocketManager : public QObject
{
    Q_OBJECT
private:
    QTcpSocket* socket;

    QString  username;
public:
    SocketManager(QTcpSocket*);
public slots:
    void read_handler();

    void disconnect_handler();
signals:
    QJsonObject start_game_request(QString);
};

#endif // SOCKETMANAGER_H
