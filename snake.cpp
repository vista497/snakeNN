#include "snake.h"
#include <QRandomGenerator64>
#include <QGridLayout>
#include <QThread>
#include <QApplication>
#include <math.h>

Snake::Snake(QWidget *parent) :
    QFrame(parent)
{
    //resize(WIDTH*HEIGHT, WIDTH*HEIGHT); //размер поля
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setFocusPolicy(Qt::StrongFocus);
    v = new viper[COUNT_SNAKE];
    for (int i=0;i<COUNT_SNAKE;i++)
    {
        v[i].initSnake();
    }
    but=new QPushButton(tr("Обучить нейросеть"));
    label=new QLabel();
   QGridLayout *lay=new QGridLayout;

    lay->addWidget(label);
   lay->addWidget(but);
    setLayout(lay);


    setWindowTitle("Snake"); // название окна

   QObject::connect(but, SIGNAL(clicked()), this, SLOT(train()));
}

QSize Snake::sizeHint()const{
    return QSize(WIDTH * 15 + frameWidth() * 2,
                 HEIGHT * 15 + frameWidth() * 2);
}
QSize Snake::minimumSizeHint()const{
    return QSize(WIDTH * 5 + frameWidth() * 2,
                 HEIGHT * 5 + frameWidth() * 2);
}

void Snake::resizeEvent(QResizeEvent* e)
{
//    int h = e->size().height();
//    int w = e->size().width();
    //int h=e->size().height()+squareHeight();
    //int w=e->size().width()+squareWidth();

    //resize(w, h);

}



void Snake::MoveStep(int key)
{
    //dead=false;
    switch (key) {
    case 0: // лево
        if(dir!=right)
        {
            step=start;
            dir=left;
        }
        break;
    case 1: //право
        if(dir!=left)
        {
            step=start;
            dir=right;
        }
        break;
    case 2: //вверх
        if(dir!=down)
        {
            step=start;
            dir=up;
        }
        break;
    case 3: //вниз
        if(dir!=up)
        {
            step=start;
            dir=down;
        }
        break;
    }
    if(!brain)
    {
        checkApple();
        move();
        checkField();
        this->repaint();
    }


}

void Snake::paintEvent(QPaintEvent *)
{
    QPalette Pal(palette()); // содержит группы цветов для каждого состояния виджета.
    Pal.setColor(QPalette::Window,Qt::black); //устанавливаем цвет
    setAutoFillBackground(true); //Это свойство определяет, заполняется ли фон виджета автоматически
    setPalette(Pal);
    drawing();
}

void Snake::drawing()
{
    QPainter paint(this); //класс для рисования
    for (int s=0;s<COUNT_SNAKE;s++ ) {

    if (inGame)
    {
        if(!v[s].dead)
        {

        paint.setBrush(Qt::red);
        paint.drawEllipse(v[s].apple.x()*squareWidth() ,v[s].apple.y()*squareHeight() ,squareWidth(), squareHeight());
        if(v[s].snake.size()>1)
        {
            paint.setBrush(Qt::blue);
            paint.drawEllipse(v[s].apple.x()*squareWidth() ,v[s].apple.y()*squareHeight() ,squareWidth(), squareHeight());
        }
        }
        for(int i=0;i<v[s].snake.size();++i)
        {
            if(i==0)
            {
                if(!v[s].dead)
                {
                paint.setBrush(Qt::green);
                paint.drawEllipse(v[s].snake[i].x()*squareWidth() ,v[s].snake[i].y()*squareHeight() ,squareWidth(), squareHeight());


                }
            }
            else
            {
                if(!v[s].dead)
                {
                paint.setBrush(Qt::white);
                paint.drawEllipse(v[s].snake[i].x()*squareWidth() ,v[s].snake[i].y()*squareHeight() ,squareWidth(), squareHeight());

                }
            }
        }

    }
    else
    {
        gameOver();
    }
    }

}

void Snake::initGame(viper *s)
{
    v=new viper[COUNT_SNAKE];
    v=s;
    inGame=true;
    for (int s=0;s<COUNT_SNAKE;s++ ) {
        dead[s]=false;
    }

    score=0;

    update();

}

void Snake::move()
{
    for (int s=0;s<COUNT_SNAKE;s++ ) {
    if(step==start)
    {
        if(eat)
            v[s].snake.push_back(QPoint(v[s].snake[v[s].snake.size()-1].x(),v[s].snake[v[s].snake.size()-1].y()));
        for(int i=v[s].snake.size()-1;i>0;--i)
        {
        v[s].snake[i]=v[s].snake[i-1];
        eat=false;
        }
    }
    switch (dir) {
    case left:  {if (step==start) v[s].snake[0].rx()-=1; step=stop; break; }
    case right: {if (step==start) v[s].snake[0].rx()+=1; step=stop; break; }
    case up:    {if (step==start) v[s].snake[0].ry()-=1; step=stop; break; }
    case down:  {if (step==start) v[s].snake[0].ry()+=1; step=stop; break; }
    case stop:  {break;}
    }
    }
}

void Snake::checkField()
{
    for (int s=0;s<COUNT_SNAKE;s++ ) {
    for(int i=1;i<v[s].snake.size();++i)
    {
        if (v[s].snake[0]==v[s].snake[i])
            inGame=false;
    }

    if(v[s].snake[0].x()>=WIDTH)
        inGame=false;
    if(v[s].snake[0].x()<0)
        inGame=false;
    if(v[s].snake[0].y()>=HEIGHT)
        inGame=false;
    if(v[s].snake[0].y()<0)
        inGame=false;

    if(!inGame)
        killTimer(timerId);
    }
}

void Snake::checkApple()
{
    for (int s=0;s<COUNT_SNAKE;s++ ) {
    if(v[s].snake[0]==v[s].apple)
    {
        eat=true;
        v[s].setApple();
        score+=1;
    }
    }
}

void Snake::gameOver()
{
    dir=stop;
    inGame=false;

}

void Snake::train()
{
  qApp->processEvents(QEventLoop::ExcludeUserInputEvents);

    NN=new Neuron*[COUNT_SNAKE];
    Net=new Neuron*[COUNT_SNAKE];
    for(int n=0;n<COUNT_SNAKE;n++)
    {
        Net[n]=new Neuron(0);
        Net[n]->SetSizeLayer(5);
        Net[n]->SetSizeLayer(5);
        Net[n]->SetSizeLayer(3);

        NN[n]=new Neuron(0);
        NN[n]->SetSizeLayer(5);
        NN[n]->SetSizeLayer(5);
        NN[n]->SetSizeLayer(3);
    }

    int episod=200;
    max_step=400;

    int predict;

    int *score=new int[COUNT_SNAKE];
    int *reward=new int[COUNT_SNAKE];
    int *score_apple=new int[COUNT_SNAKE];
    bool flagBreak;
   // int max_step_snake=20;

    for (int x=0;x<episod;x++)
    {
       cout <<x<<endl;
       flagBreak=false;

        for (int n=0;n< COUNT_SNAKE;n++ ) {
            if (x!=0)
            {
                Net[n]->Init();
                Net[n]=NN[n];
            }
            else
            {
                NN[n]->Init();

                Net[n]->Init();
            }
        }

        v=new viper[COUNT_SNAKE];
        for (int i=0;i<COUNT_SNAKE;i++ ) {
            v[i].initSnake();
            snake_step[i]=100;
            score[i]=0;
            reward[i]=0;
            score_apple[i]=0;
        }

        initGame(v);
        this->repaint();
        int all_dead=0;
        double max_dist=sqrt(pow(WIDTH,2)+pow(HEIGHT,2));
        qApp->processEvents();
         for (int y=0;y<max_step;y++){

             if(flagBreak)
                 break;
             for (int s=0;s< COUNT_SNAKE;s++) {


                if(dead[s]||snake_step[s]==0)
                {
                    v[s].inGame=false;
                    v[s].dead=true;
                    all_dead++;
                    if(all_dead/COUNT_SNAKE==COUNT_SNAKE)
                        flagBreak=true;
                }
                else
                {


            double distanceToApple=1.0/DistanceToApple(s); //дистанция до яблока
            double *distanceToBlock; //дистанция до блоков = 3 переменных
            double direction=1.0/(v[s].direction+1);

                distanceToBlock=DistanceToBlock(v[s].direction,s);

            double input[]={distanceToApple,distanceToBlock[0],distanceToBlock[1], distanceToBlock[2],distanceToBlock[3]};
                //double input[]={g,g};

            Net[s]->SetInputs(input); // загружаем входные данные в нейросеть

            predict= Net[s]->ForwardFeed();
//            Net[s]->PrintValues();
//            cout<<"+++++"<<s<<"+++++"<<endl;
            score_apple[s]=v[s].score;
            //
            v[s].MoveStep(int(predict),v[s].direction);

            //sa[s]=v[s].score;

            dead[s]=v[s].dead;

            reward[s]=Reward(reward[s], distanceToApple, score_apple[s], dead[s], s);

            score[s]=reward[s];

        }


}

           QThread::msleep(10);
             this->repaint();


    }
        int sortNumSnake[COUNT_SNAKE];
        int sort[COUNT_SNAKE];
        for (int i=0;i<COUNT_SNAKE ;i++ ) {
            sortNumSnake[i]=score[i];
            sort[i]=score[i];
        }

        for(int snakeI=0;snakeI<COUNT_SNAKE;snakeI++)
        {
            for(int snakeJ=0;snakeJ<COUNT_SNAKE-snakeI-1;snakeJ++)
                if (sortNumSnake[snakeJ]>sortNumSnake[snakeJ+1])
                {
                    int temp=sortNumSnake[snakeJ];
                    sortNumSnake[snakeJ]=sortNumSnake[snakeJ+1];
                    sortNumSnake[snakeJ+1]=temp;
                }
        }
        int d;
        for (int i=0;i<COUNT_SNAKE ;i++ ) {

             if(sort[i]==sortNumSnake[COUNT_SNAKE-1])
                d=i;
        }
        cout<<"====="<<sortNumSnake[COUNT_SNAKE-1]<<"====="<<endl;

        for(int i=0;i<COUNT_SNAKE;i++)
        {
            NN[i]=Net[d];
            NN[i]->Qwerty();
        }

        if(x==episod-1)
         NN[COUNT_SNAKE-1]->SaveNet();

    }

}

double Snake::DistanceToApple(int s)
{
    return sqrt(pow((v[s].GetAppleX()-v[s].GetSnakeX()),2)+pow((v[s].GetAppleY()-v[s].snake[0].y()),2));
}

double *Snake::DistanceToBlock(int direction, int s)
{

    static double distance[4];
    int headX=v[s].snake[0].x();
    int headY=v[s].snake[0].y();
    int blockX=0;
    int blockY=0;

    switch(direction){
    case(0):
        distance[0]=0;            //лево
        for (int i=v[s].GetSnakeX(); i<=HEIGHT; i++) {
                    if(v[s].BodySnakeInXY(i,v[s].GetSnakeY()))
                    {
                        blockX=i;

                    }
                    else
                        blockX=HEIGHT;
                }

        distance[1]=(-v[s].GetSnakeX()+blockX);            //право

                for (int i=v[s].GetSnakeX(); i>=0; i--) {
                    if(v[s].BodySnakeInXY(v[s].GetSnakeY(),i))
                        blockY=i;
                    else
                        blockY=0;
                }

        distance[2]=(v[s].GetSnakeY()-blockY);            //верх

                for (int i=v[s].GetSnakeX(); i<=WIDTH; i++) {
                    if(v[s].BodySnakeInXY(v[s].GetSnakeY(),i))
                        blockY=i;
                    else
                        blockY=WIDTH;
                }

        distance[3]=( -v[s].GetSnakeY()+blockY);           //низ
        break;
    case(1):
                for (int i=v[s].GetSnakeX(); i>=0; i--) {
                    if(v[s].BodySnakeInXY(i,v[s].GetSnakeY()))
                    {
                        blockX=i;

                     }
                    else
                        blockX=0;
                }

        distance[0]=(v[s].GetSnakeX()-blockX);            //лево
        for (int i=v[s].GetSnakeX(); i<=HEIGHT; i++) {
                    if(v[s].BodySnakeInXY(i,v[s].GetSnakeY()))
                    {
                        blockX=i;

                    }
                    else
                        blockX=HEIGHT;
                }

        distance[1]=(-v[s].GetSnakeX()+blockX);            //право

                for (int i=v[s].GetSnakeX(); i>=0; i--) {
                    if(v[s].BodySnakeInXY(v[s].GetSnakeY(),i))
                        blockY=i;
                    else
                        blockY=0;
                }

        distance[2]=(v[s].GetSnakeY()-blockY);            //верх
        distance[3]=0;           //низ
        break;
    case(2):
        for (int i=v[s].GetSnakeX(); i>=0; i--) {
            if(v[s].BodySnakeInXY(i,v[s].GetSnakeY()))
            {
                blockX=i;

             }
            else
                blockX=0;
        }

        distance[0]=(v[s].GetSnakeX()-blockX);            //лево


        distance[1]=0;            //право

                for (int i=v[s].GetSnakeX(); i>=0; i--) {
                    if(v[s].BodySnakeInXY(v[s].GetSnakeY(),i))
                        blockY=i;
                    else
                        blockY=0;
                }

        distance[2]=(v[s].GetSnakeY()-blockY);            //верх

                for (int i=v[s].GetSnakeX(); i<=WIDTH; i++) {
                    if(v[s].BodySnakeInXY(v[s].GetSnakeY(),i))
                        blockY=i;
                    else
                        blockY=WIDTH;
                }

        distance[3]=( -v[s].GetSnakeY()+blockY);           //низ
        break;
    case(3):
        for (int i=v[s].GetSnakeX(); i>=0; i--) {
            if(v[s].BodySnakeInXY(i,v[s].GetSnakeY()))
            {
                blockX=i;

             }
            else
                blockX=0;
        }

        distance[0]=(v[s].GetSnakeX()-blockX);            //лево
        for (int i=v[s].GetSnakeX(); i<=HEIGHT; i++) {
                    if(v[s].BodySnakeInXY(i,v[s].GetSnakeY()))
                    {
                        blockX=i;

                    }
                    else
                        blockX=HEIGHT;
                }

        distance[1]=(-v[s].GetSnakeX()+blockX);            //право
        distance[2]=0;            //верх

                for (int i=v[s].GetSnakeX(); i<=WIDTH; i++) {
                    if(v[s].BodySnakeInXY(v[s].GetSnakeY(),i))
                        blockY=i;
                    else
                        blockY=WIDTH;
                }

        distance[3]=( -v[s].GetSnakeY()+blockY);           //низ
        break;
    }

    for (int i=0;i<4 ;i++ ) {
        if(distance[i]!=0)
            distance[i]/=40;
    }
return distance;
}

double Snake::Reward(int &rew, double &distance, int &score_apple, bool &d,int s)
{
    int reward=rew;
    //double max_dist=sqrt(pow(WIDTH,2)+pow(HEIGHT,2));
    int one_step=0;
    int dead=-100;
    double distanceNow=DistanceToApple(s);
    int score_apple_now=v[s].score;

    if (score_apple_now>score_apple)
    {
        reward+=100*(score_apple_now+1);
        snake_step[s]+=100;

    }
    if (d){ reward+=dead; }
    else{ reward+=0;  }
    if (distanceNow<distance)
    {
        reward+=one_step+2 ;
        snake_step[s]+=1;
    }
    else
    {
        reward+=one_step-3;
        snake_step[s]-=1;
    }
    return reward;
}
