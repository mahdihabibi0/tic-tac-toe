#include <QApplication>
#include "baseserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BaseServer server;
    return a.exec();
}
