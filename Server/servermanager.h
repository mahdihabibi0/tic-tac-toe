#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QDialog>

namespace Ui {
class ServerManager;
}

class ServerManager : public QDialog
{
    Q_OBJECT

public:
    explicit ServerManager(QWidget *parent = nullptr);
    ~ServerManager();

private:
    Ui::ServerManager *ui;
};

#endif // SERVERMANAGER_H
