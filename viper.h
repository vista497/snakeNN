#ifndef VIPER_H
#define VIPER_H

#include "neuron.h"
#include <QKeyEvent>

class viper
{

    enum Dir{left, right, up, down, stop, start};
    Dir dir=stop;
    Dir step=start;

public:
    QVector<QPoint> snake; //координаты змейки
    QPoint apple;
    Neuron *Net;

    viper();
    const int WIDTH =40; //ширина
    const int HEIGHT=40; //высота
       bool brain=false;
        bool eat=false;
        bool inGame=true;
        int score=0;
        bool dead;
        int direction=start;

    void setApple();
    void initSnake(Neuron *Net=0);
    void MoveStep(int key, int direction);
    void Brain();

    void move();
    void checkField();
    void checkApple();
    void gameOver();

    double DistanceToApple();
    double *DistanceToBlock(int direction);
    double Reward(int &, double &, int &,bool &);

    int GetSnakeX();
    int GetSnakeY();

    int GetAppleX();
    int GetAppleY();

    int GetSnakeSize();

    int GetSnakeBodyX(int);
    int GetSnakeBodyY(int);
    bool BodySnakeInXY(int x, int y);


};

#endif // VIPER_H
