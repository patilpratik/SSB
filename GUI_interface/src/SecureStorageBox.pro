#-------------------------------------------------
#
# Project created by QtCreator 2017-08-12T23:36:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SecureStorgeBox
TEMPLATE = app

INCLUDEPATH += /home/pratik/Programming/cpp/SSB/include
INCLUDEPATH += /home/pratik/Programming/cpp/SSB/GUID_interface/include

LIBS += -L/home/pratik/Programming/cpp/SSB/lib -lfile_splitter -lcryptopp_wrapper -lkey_mutator -lcryptopp
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
    mainwindow.cpp \
    addwindow.cpp

HEADERS  += mainwindow.h \
    addwindow.h

FORMS    += mainwindow.ui \
    addwindow.ui
