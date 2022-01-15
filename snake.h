#ifndef SNAKE_H
#define SNAKE_H

#include <QWidget>
#include "matrix.h"
#include <QKeyEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QRandomGenerator64>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include "neuron.h"
#include <QFrame>
#include "viper.h"

#define COUNT_SNAKE 1000
class Snake: public QFrame
{
    Q_OBJECT

    QPushButton *but;
    QLabel *label;
    viper *v;
    Neuron **Net;
    Neuron **NN;
    int max_step;
    int snake_step[COUNT_SNAKE];

    bool eat=false;
    double DistanceToApple(int s);
    double *DistanceToBlock(int direction,int s);
    double Reward(int &, double &, int &,bool &,int s);

    enum Dir{left, right, up, down, stop, start};
    Dir dir=stop;
    Dir step=start;
    bool dead[COUNT_SNAKE];


    const int timer =150;
    int timerId;

    //QVector<QPoint> snake; //координаты змейки
    //QPoint apple; //координаты яблока

    bool inGame;
    void drawing();
    void move();
    void checkField();
    void checkApple();
    void gameOver();

    int squareWidth() { return contentsRect().width() / WIDTH; }
    int squareHeight() { return contentsRect().height() / HEIGHT; }

public:
    explicit Snake(QWidget *parent = nullptr);
    void initGame(viper *s);
    const int WIDTH =40; //ширина
    const int HEIGHT=40; //высота
    int score=0;
    bool brain=false;
//    int GetSnakeX();
//    int GetSnakeY();

//    int GetAppleX();
//    int GetAppleY();

//    int GetSnakeSize();

//    int GetSnakeBodyX(int);
//    int GetSnakeBodyY(int);
//    bool BodySnakeInXY(int x, int y);
    void MoveStep(int);

protected:
    void paintEvent(QPaintEvent*) override;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    void resizeEvent(QResizeEvent *) override;
public slots:
    void train();
};

#endif // SNAKE_H
