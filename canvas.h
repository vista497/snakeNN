#ifndef CANVAS_H
#define CANVAS_H
#include <QWidget>
#include <QFrame>
#include "viper.h"
#include "neuron.h"
#include <QKeyEvent>
#include <QPaintEvent>

class canvas:public QFrame
{

    Q_OBJECT


    const int WIDTH =40; //ширина
    const int HEIGHT=40; //высота

    Neuron *Net;
    bool eat=false;
    int count;

public:
    explicit canvas(QWidget *parent = nullptr);
    void initGame(int count_snake=1);
    viper *snake;

    int squareWidth() { return contentsRect().width() / WIDTH; }
    int squareHeight() { return contentsRect().height() / HEIGHT; }

    void drawing();
    void move();
    void checkField();
    void checkApple();
    void gameOver();


    enum Dir{left, right, up, down, stop, start};
    Dir dir=stop;
    Dir step=start;

    const int timer =150;
    int timerId;

    int score=0;
    bool dead;
    bool brain=false;
    bool brII;
    bool inGame;
   // void MoveStep(int key);

protected:
    void timerEvent(QTimerEvent *) override;
    void keyPressEvent(QKeyEvent*) override;
    void paintEvent(QPaintEvent*) override;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    void resizeEvent(QResizeEvent *) override;
};

#endif // CANVAS_H
