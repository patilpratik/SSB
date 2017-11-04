#-------------------------------------------------
#
# Project created by QtCreator 2017-08-14T19:30:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SecureStorageBox
TEMPLATE = app

INCLUDEPATH += /home/pratik/Programming/cpp/SSB/include
INCLUDEPATH += /home/pratik/Programming/cpp/SSB/GUID_interface/include

LIBS += -L/home/pratik/Programming/cpp/SSB/lib -lfile_splitter -lcryptopp_wrapper -lkey_mutator -lcryptopp -lauthenticator
QMAKE_CXXFLAGS += -std=c++11


SOURCES += main.cpp\
    mainwindow.cpp \
    addwindow.cpp \
    passwordwindow.cpp

HEADERS  += mainwindow.h \
    addwindow.h \
    passwordwindow.h

FORMS    += mainwindow.ui \
    addwindow.ui \
    passwordwindow.ui
