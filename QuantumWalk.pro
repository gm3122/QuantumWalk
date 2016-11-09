QT += core
QT -= gui

CONFIG += c++11

TARGET = QuantumWalk
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    lattice.cpp \
    qw2c.cpp \
    qw3c.cpp

HEADERS += \
    lattice.h \
    qw2c.h \
    qw3c.h
