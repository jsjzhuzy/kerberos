#-------------------------------------------------
#
# Project created by QtCreator 2019-04-28T11:10:14
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AS_test_3
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
        as.cpp \
    myserver.cpp \
    mythread.cpp \
    packetup.cpp \
    descrypt.cpp \
    rsa.cpp

HEADERS += \
        as.h \
    myserver.h \
    mysocketthread.h \
    C:/Users/Administrator/Documents/Tencent Files/446868505/FileRecv/descrypt.h \
    C:/Users/Administrator/Documents/Tencent Files/446868505/FileRecv/descrypt.h \
    packetup.h \
    descrypt.h \
    rsa.h

FORMS += \
        as.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -LC:/OpenSSL-Win64/lib -llibcrypto
LIBS += -LC:/OpenSSL-Win64/lib -llibssl
QMAKE_CFLAGS += -fpermissive
QMAKE_CXXFLAGS += -fpermissive
INCLUDEPATH += C:/OpenSSL-Win64/include
