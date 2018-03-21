TEMPLATE = app
CONFIG += console c++11
CONFIG += thread
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    visualisation.cpp \
    fork.cpp \
    philosopher.cpp

LIBS += -lncurses

HEADERS += \
    visualisation.h \
    fork.h \
    philosopher.h
