TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
        activatefunc.cpp \
        canvas.cpp \
        game.cpp \
        main.cpp \
        matrix.cpp \
        neuron.cpp \
        snake.cpp \
        viper.cpp

HEADERS += \
    activatefunc.h \
    canvas.h \
    game.h \
    matrix.h \
    neuron.h \
    snake.h \
    viper.h

QT += widgets
