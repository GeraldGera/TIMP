QT += testlib sql network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += UNIT_TEST_MODE

SERVER_PATH = ..

SOURCES += \
    tst_funcforserver_test.cpp \
    $$SERVER_PATH/functionstoserver.cpp \
    $$SERVER_PATH/Singleton.cpp

HEADERS += \
    $$SERVER_PATH/functionstoserver.h \
    $$SERVER_PATH/Singleton.h

INCLUDEPATH += $$SERVER_PATH

win32 {
    CONFIG += console
    CONFIG -= app_bundle
}
