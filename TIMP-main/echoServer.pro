QT += core network sql testlib
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = echoServer

SOURCES += main.cpp \
           mytcpserver.cpp \
           Singleton.cpp \
           functionstoserver.cpp

HEADERS += mytcpserver.h \
           Singleton.h \
           functionstoserver.h

DISTFILES += \
    Doxyfile