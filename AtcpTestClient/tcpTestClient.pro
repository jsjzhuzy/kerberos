#-------------------------------------------------
#
# Project created by QtCreator 2017-08-07T15:03:10
#
#-------------------------------------------------

QT       += core gui
QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tcpTestClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    descrypt.cpp \
    packetup.cpp \
    rsa.cpp

HEADERS  += mainwindow.h \
    descrypt.h \
    packetup.h \
    rsa.h

FORMS    += mainwindow.ui
LIBS += -LC:/OpenSSL-Win64/lib -llibcrypto
LIBS += -LC:/OpenSSL-Win64/lib -llibssl
QMAKE_CFLAGS += -fpermissive
QMAKE_CXXFLAGS += -fpermissive
INCLUDEPATH += C:/OpenSSL-Win64/include
