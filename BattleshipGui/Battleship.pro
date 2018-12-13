#-------------------------------------------------
#
# Project created by QtCreator 2018-12-11T15:23:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Battleship
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    pvpwindow.cpp \
    pvbwindow.cpp \
    selectshipswindow.cpp

HEADERS += \
        mainwindow.h \
    ../ITCS3112Module7Project-master/BattleShipGame/include/BotAi.h \
    ../ITCS3112Module7Project-master/BattleShipGame/include/Client.h \
    ../ITCS3112Module7Project-master/BattleShipGame/include/GameMech.h \
    ../ITCS3112Module7Project-master/BattleShipGame/include/Grid.h \
    ../ITCS3112Module7Project-master/BattleShipGame/include/Gui.h \
    ../ITCS3112Module7Project-master/BattleShipGame/include/Player.h \
    ../ITCS3112Module7Project-master/BattleShipGame/include/Server.h \
    ../ITCS3112Module7Project-master/BattleShipGame/include/Ship.h \
    ../ITCS3112Module7Project-master/BattleShipGame/include/Square.h \
    ../ITCS3112Module7Project-master/BattleShipGame/include/User.h \
    ../ITCS3112Module7Project-master/BattleShipGame/include/BotAi.h \
    ../ITCS3112Module7Project-master/BattleShipGame/include/Client.h \
    ../ITCS3112Module7Project-master/BattleShipGame/include/GameMech.h \
    ../ITCS3112Module7Project-master/BattleShipGame/include/Grid.h \
    ../ITCS3112Module7Project-master/BattleShipGame/include/Gui.h \
    ../ITCS3112Module7Project-master/BattleShipGame/include/Player.h \
    ../ITCS3112Module7Project-master/BattleShipGame/include/Server.h \
    ../ITCS3112Module7Project-master/BattleShipGame/include/Ship.h \
    ../ITCS3112Module7Project-master/BattleShipGame/include/Square.h \
    ../ITCS3112Module7Project-master/BattleShipGame/include/User.h \
    pvpwindow.h \
    pvbwindow.h \
    selectshipswindow.h

FORMS += \
        mainwindow.ui \
    pvpwindow.ui \
    pvbwindow.ui \
    selectshipswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    PvpWindowForm.ui.qml \
    PvpWindow.qml
