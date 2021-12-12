QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = axis-tags-example
TEMPLATE = app

INCLUDEPATH += ../../
SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ../../qcustomplot.cpp \
    axistag.cpp

HEADERS += \
        mainwindow.h \
    ../../qcustomplot.h \
    axistag.h

FORMS += \
        mainwindow.ui
