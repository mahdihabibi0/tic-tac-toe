QT       += core gui
QT       += network
QT += testlib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    mapsituations.cpp \
    baseserver.cpp \
    gameserver.cpp \
    gamesocketmanager.cpp \
    getippage.cpp \
    main.cpp \
    mapitem.cpp \
    mapstatements.cpp \
    servermanager.cpp \
    serversocketmanager.cpp \
    timer.cpp \
    usersHandler.cpp

HEADERS += \
    mapsituations.h \
    baseserver.h \
    gameserver.h \
    gamesocketmanager.h \
    getippage.h \
    mapitem.h \
    mapstatements.h \
    servermanager.h \
    serversocketmanager.h \
    timer.h \
    usersHandler.h

FORMS += \
    getippage.ui \
    servermanager.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
