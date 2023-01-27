QT       += core gui
QT  += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    commissioning.cpp \
    customerorder.cpp \
    main.cpp \
    mainwindow.cpp \
    managestock.cpp \
    orderstock.cpp \
    reciveorder.cpp \
    stockoverview.cpp

HEADERS += \
    commissioning.h \
    customerorder.h \
    mainwindow.h \
    managestock.h \
    orderstock.h \
    reciveorder.h \
    stockoverview.h

FORMS += \
    commissioning.ui \
    customerorder.ui \
    mainwindow.ui \
    managestock.ui \
    orderstock.ui \
    reciveorder.ui \
    stockoverview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
