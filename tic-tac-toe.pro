QT       += core gui
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    homepage.cpp \
    loginpage.cpp \
    lsmanager.cpp \
    main.cpp \
    game.cpp \
    signuppage.cpp \
    tcpsocketmanager.cpp

HEADERS += \
    Errors.h \
    game.h \
    homepage.h \
    loginpage.h \
    lsmanager.h \
    signuppage.h \
    tcpsocketmanager.h

FORMS += \
    game.ui \
    homepage.ui \
    loginpage.ui \
    signuppage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
