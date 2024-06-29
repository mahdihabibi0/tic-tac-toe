#ifndef TCPSOCKETMANAGER_H
#define TCPSOCKETMANAGER_H
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QMessageBox>


class TCPSocketManager : public QTcpSocket
{
    Q_OBJECT
    QMessageBox waitingForServerConnection;
public:
    TCPSocketManager();

private slots:

public slots:
    bool try_to_login(QJsonObject &user );

    bool try_to_signup(QJsonObject &user);

    bool try_to_start_game();

    void connected_to_server();

signals:
    void server_is_online();

};

#endif // TCPSOCKETMANAGER_H
