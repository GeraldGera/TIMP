QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = echoServer
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mytcpserver.cpp \
    functionstoserver.cpp

HEADERS += \
    mytcpserver.h \
    functionstoserver.h

unix:!android: target.path = /opt/$${TARGET}/bin

!isEmpty(target.path): INSTALLS += target
