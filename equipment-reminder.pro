#-------------------------------------------------
#
# Project created by QtCreator 2012-03-25T19:10:34
#
#-------------------------------------------------

QT       += core gui sql

TARGET = equipment-reminder
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dbmanager.cpp \
    addeventdialog.cpp \
    eventinformer.cpp \
    eventform.cpp \
    settingsdialog.cpp \
    timermanager.cpp

HEADERS  += mainwindow.h \
    dbmanager.h \
    addeventdialog.h \
    eventinformer.h \
    eventform.h \
    settingsdialog.h \
    timermanager.h

FORMS    += mainwindow.ui \
    addeventdialog.ui \
    eventform.ui \
    settingsdialog.ui
