QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += network
QT += network widgets

CONFIG += c++17


SOURCES += \
    ApiClient.cpp \
    AuthRegForm.cpp \
    ProfilePage.cpp \
    TasksPage.cpp \
    main.cpp \
    mainwindow.cpp \
    singletonclient.cpp

HEADERS += \
    ApiClient.h \
    AuthRegForm.h \
    ProfilePage.h \
    SingletonClient.h \
    TasksPage.h \
    mainwindow.h \
    userinfo.h

FORMS += \
    AuthRegForm.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
