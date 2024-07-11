QT       += core gui
QT += network
QT += testlib
QT += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    buttonSound.cpp \
    drawedpage.cpp \
    getippage.cpp \
    loserpage.cpp \
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
    sounds.cpp \
    tcpsocketmanager.cpp \
    timer.cpp \
    userHandler.cpp \
    winnerpage.cpp

HEADERS += \
    buttonSound.h \
    drawedpage.h \
    getippage.h \
    loserpage.h \
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
    sounds.h \
    tcpsocketmanager.h \
    timer.h \
    userHandler.h \
    winnerpage.h

FORMS += \
    drawedpage.ui \
    game.ui \
    getippage.ui \
    homepage.ui \
    loginpage.ui \
    loserpage.ui \
    multipleanswerquestion.ui \
    numbericalanswerquestion.ui \
    shortanswerquestion.ui \
    signuppage.ui \
    winnerpage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
