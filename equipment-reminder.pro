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
    addeventdialog.cpp

HEADERS  += mainwindow.h \
    dbmanager.h \
    addeventdialog.h

FORMS    += mainwindow.ui \
    addeventdialog.ui
