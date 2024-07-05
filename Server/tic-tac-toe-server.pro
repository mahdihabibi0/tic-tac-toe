QT       += core gui
QT       += network
QT += testlib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    baseserver.cpp \
    doozemap.cpp \
    gameserver.cpp \
    gamesocketmanager.cpp \
    getippage.cpp \
    main.cpp \
    socketmanager.cpp

HEADERS += \
    baseserver.h \
    doozemap.h \
    gameserver.h \
    gamesocketmanager.h \
    getippage.h \
    socketmanager.h

FORMS += \
    getippage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
