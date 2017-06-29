#-------------------------------------------------
#
# Project created by QtCreator 2017-03-07T21:27:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_gui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    ../src/classes.cpp \
    ../src/core.cpp \
    ../src/error.cpp \
    ../src/filters.cpp \
    ../src/IO.cpp \
    ../src/html_addon.cpp

HEADERS  += mainwindow.h \
    ../src/core.h \
    ../src/IO.h \
    ../src/error.h \
    ../src/classes.h \
    ../src/filters.h \
    ../src/html_addon.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -std=c++0x
CONFIG += c++11

RESOURCES += \
    resources.qrc
