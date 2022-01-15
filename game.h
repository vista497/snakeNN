#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include "snake.h"
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include "neuron.h"
#include "canvas.h"
#include "viper.h"
#include "snake.h"

class game : public QWidget
{
    Q_OBJECT

    Snake *s;
    viper *snake;
    QRadioButton *button;
    QPushButton *Neuro;
    QLabel *label;
    canvas *board;

public:
    explicit game(QWidget *parent = nullptr);

public slots:
    void brain();
    void NetShow();
};

#endif // GAME_H
