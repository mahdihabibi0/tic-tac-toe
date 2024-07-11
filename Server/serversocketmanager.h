#ifndef SERVERSOCKETMANAGER_H
#define SERVERSOCKETMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMap>

class ServerSocketManager : public QObject
{
    Q_OBJECT
private:
    QTcpSocket* socket;

    QString  username;
public:
    ServerSocketManager(QTcpSocket*);
public slots:
    void read_handler();

    void disconnect_handler();
signals:
    int get_player_statement(QString);

    QJsonObject start_game_request(QString);
};

#endif // SERVERSOCKETMANAGER_H
