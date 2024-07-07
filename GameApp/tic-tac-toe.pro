QT       += core gui
QT += network
QT += testlib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    getippage.cpp \
    multipleanswerquestion.cpp \
    numbericalanswerquestion.cpp \
    question.cpp \
    gamebutton.cpp \
    homepage.cpp \
    loginpage.cpp \
    lsmanager.cpp \
    main.cpp \
    game.cpp \
    shortanswerquestion.cpp \
    signuppage.cpp \
    tcpsocketmanager.cpp

HEADERS += \
    Errors.h \
    getippage.h \
    multipleanswerquestion.h \
    numbericalanswerquestion.h \
    question.h \
    game.h \
    gamebutton.h \
    homepage.h \
    loginpage.h \
    lsmanager.h \
    shortanswerquestion.h \
    signuppage.h \
    tcpsocketmanager.h

FORMS += \
    game.ui \
    getippage.ui \
    homepage.ui \
    loginpage.ui \
    multipleanswerquestion.ui \
    numbericalanswerquestion.ui \
    shortanswerquestion.ui \
    signuppage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
