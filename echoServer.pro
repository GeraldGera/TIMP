QT -= gui
QT += network sql

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = echoServer
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    Singleton.cpp \
    functionstoserver.cpp \
    main.cpp \
    mytcpserver.cpp
    Singleton.cpp

HEADERS += \
    Singleton.h \
    functionstoserver.h \
    mytcpserver.h
    Singleton.h

unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
