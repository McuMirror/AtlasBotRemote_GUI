#-------------------------------------------------
#
# Project created by QtCreator 2015-12-09T16:36:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AtlasBotRemote_GUI
TEMPLATE = app


SOURCES += GUI/atlasbotremote.cpp \
           GUI/main.cpp \
    GUI/dialogpreferences.cpp \
    PROCESS/qmodbustcp.cpp

HEADERS  += GUI/atlasbotremote.h \
    GUI/dialogpreferences.h \
    LIB/modbus.h \
    LIB/modbus-rtu.h \
    LIB/modbus-tcp.h \
    LIB/modbus-version.h \
    PROCESS/qmodbustcp.h \
    PROCESS/dicoModbus.h
FORMS    += GUI/atlasbotremote.ui \
    GUI/dialogpreferences.ui

unix:!macx: LIBS += -L$$PWD/LIB/ -lModbus

INCLUDEPATH += $$PWD/LIB
DEPENDPATH += $$PWD/LIB
