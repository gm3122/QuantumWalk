QT += core
QT -= gui

CONFIG += c++11

TARGET = QuantumWalk
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    lattice.cpp

HEADERS += \
    lattice.h
