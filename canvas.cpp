#include "canvas.h"
#include <QResizeEvent>
#include <QPainter>
#include <QRandomGenerator>

canvas::canvas(QWidget *parent) :
    QFrame(parent)
{
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setFocusPolicy(Qt::StrongFocus);

    setWindowTitle("Snake");
    Net=new Neuron(1);
    Net->SetSizeLayer(2);
    Net->SetSizeLayer(18);
    Net->SetSizeLayer(3);
    Net->Init();
    initGame();


}

void canvas::initGame(int count_snake)
{
    count=count_snake;
    snake=new viper[count];
    for (int s=0;s<count;s++ ) {
        snake[s].initSnake();
        snake[s].inGame=true;
    }
    inGame=true;
    score=0;
    timerId=startTimer(timer);
}

QSize canvas::sizeHint()const{
    return QSize(WIDTH * 15 + frameWidth() * 2,
                 HEIGHT * 15 + frameWidth() * 2);
}
QSize canvas::minimumSizeHint()const{
    return QSize(WIDTH * 5 + frameWidth() * 2,
                 HEIGHT * 5 + frameWidth() * 2);
}

void canvas::resizeEvent(QResizeEvent* e)
{
    int h = e->size().height();
    int w = e->size().width();
    h=w;

    resize(w, h);

}

void canvas::timerEvent(QTimerEvent *)
{
    if(inGame)
    {
        if(brain){
            if(brII)
            {
               Net->LoadNet();
               //snake=new viper();
               snake->initSnake(Net);
               brII=false;

            }
            snake->Brain();
        }
        if(!brain){
        checkApple();
        move();
        checkField();
        }
    }
    this->repaint();
}



void canvas::paintEvent(QPaintEvent *)
{
    QPalette Pal(palette()); // содержит группы цветов для каждого состояния виджета.
    Pal.setColor(QPalette::Window,Qt::black); //устанавливаем цвет
    setAutoFillBackground(true); //Это свойство определяет, заполняется ли фон виджета автоматически
    setPalette(Pal);
    drawing();
}

void canvas::drawing()
{
    QPainter paint(this); //класс для рисования


        if (snake->inGame)
        {

            paint.setBrush(Qt::red);
            paint.drawEllipse(snake->apple.x()*squareWidth() ,snake->apple.y()*squareHeight() ,squareWidth(), squareHeight());
            for(int i=0;i<snake->snake.size();++i)
            {
                if(i==0)
                {
                    paint.setBrush(Qt::green);
                    paint.drawEllipse(snake->snake[i].x()*squareWidth() ,snake->snake[i].y()*squareHeight() ,squareWidth(), squareHeight());
                }
                else
                {
                    paint.setBrush(Qt::white);
                    paint.drawEllipse(snake->snake[i].x()*squareWidth() ,snake->snake[i].y()*squareHeight() ,squareWidth(), squareHeight());
                }
            }

        }
        else
        {
            gameOver();
        }


}

void canvas::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    switch (key) {
    case Qt::Key_Left:
        if(dir!=right)
        {
            step=start;
            dir=left;
        }
        break;
    case Qt::Key_Right:
        if(dir!=left)
        {
            step=start;
            dir=right;
        }
        break;
    case Qt::Key_Up:
        if(dir!=down)
        {
            step=start;
            dir=up;
        }
        break;
    case Qt::Key_Down:
        if(dir!=up)
        {
            step=start;
            dir=down;
        }
        break;
    }
}

void canvas::move()
{
    dead=false;
    for (int s=0;s<count;s++ ) {
    if(step==start)
    {
        if(eat)
            snake[s].snake.push_back(QPoint(snake[s].snake[snake[s].snake.size()-1].x(),snake[s].snake[snake[s].snake.size()-1].y()));
        for(int i=snake[s].snake.size()-1;i>0;--i)
        {
        snake[s].snake[i]=snake[s].snake[i-1];
        eat=false;
        }
    }
    switch (dir) {
    case left:  {if (step==start) snake[s].snake[0].rx()-=1; step=stop; break; }
    case right: {if (step==start) snake[s].snake[0].rx()+=1; step=stop; break; }
    case up:    {if (step==start) snake[s].snake[0].ry()-=1; step=stop; break; }
    case down:  {if (step==start) snake[s].snake[0].ry()+=1; step=stop; break; }
    case stop:  {break;}
    }
    }
}

void canvas::checkField()
{
    for (int s=0;s<count;s++ ) {
    for(int i=1;i<snake[s].snake.size();++i)
    {
        if (snake[s].snake[0]==snake[s].snake[i])
            inGame=false;
    }

    if(snake[s].snake[0].x()>=WIDTH)
        inGame=false;
    if(snake[s].snake[0].x()<0)
        inGame=false;
    if(snake[s].snake[0].y()>=HEIGHT)
        inGame=false;
    if(snake[s].snake[0].y()<0)
        inGame=false;

    if(!inGame)
        killTimer(timerId);
    }
}

void canvas::checkApple()
{
    for (int s=0;s<count;s++ ) {
    if(snake[s].snake[0]==snake[s].apple)
    {
        eat=true;
        snake[s].setApple();
        score+=1;
    }
    }
}

void canvas::gameOver()
{
    dir=stop;
    dead=true;

    initGame(1);

}
