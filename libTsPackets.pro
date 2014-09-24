#-------------------------------------------------
#
# Project created by QtCreator 2014-05-30T09:44:40
#
#-------------------------------------------------

QT -= core gui

TARGET = TsPackets
TEMPLATE = lib
#DESTDIR = /usr/lib
DESTDIR += ../lib

INCLUDEPATH += ../libCpp98

SOURCES += \
    Descriptor.cpp \
    Packet_TS.cpp

HEADERS += \
    Descriptor.h \
    Packet_TS.h