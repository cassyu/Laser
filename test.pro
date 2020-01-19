#-------------------------------------------------
#
# Project created by QtCreator 2017-12-08T19:57:54
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test123
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    check.cpp \
    Gcode.cpp \
    ProcessImage.cpp

HEADERS  += mainwindow.h \
    image_gcode.h

FORMS    += \
    mainwindow.ui
LIBS += -LD:/che   -lADVAPI32
