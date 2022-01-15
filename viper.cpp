#include "viper.h"
#include <QRandomGenerator>

viper::viper()

{
    snake.resize(1); // размер змеки

    for (int i=0;i<snake.size();i++) //расположение змейки
    {

       snake[i].rx()=QRandomGenerator::global()->bounded(WIDTH);;
       snake[i].ry()=QRandomGenerator::global()->bounded(HEIGHT);;
    }

    direction=QRandomGenerator::global()->bounded(3);

    setApple();
    Net=new Neuron(1);
    Net->SetSizeLayer(2);
    Net->SetSizeLayer(18);
    Net->SetSizeLayer(3);
    Net->Init();
    inGame=true;
}

void viper::initSnake(Neuron *NN)
{
    direction=QRandomGenerator::global()->bounded(3);

    if(NN!=0)
    {
        Net=new Neuron(1);
        Net->SetSizeLayer(2);
        Net->SetSizeLayer(18);

        Net->SetSizeLayer(3);
        Net->Init();
        Net=NN;

    }
    for(int i=snake.size()-1;i>0;--i)
    {
        if(dir==0) {snake[i].rx()=snake[i-1].x()-1; snake[i].ry()=snake[i-1].y(); }
        if(dir==1) {snake[i].rx()=snake[i-1].x(); snake[i].ry()=snake[i-1].y()-1; }
        if(dir==2) {snake[i].rx()=snake[i-1].x()+1; snake[i].ry()=snake[i-1].y(); }
        if(dir==3) {snake[i].rx()=snake[i-1].x(); snake[i].ry()=snake[i-1].y()+1; }

    }
    inGame=true;
    setApple();
}

void viper::setApple()
{
    bool r=true;
    while(r){
        apple.rx()=QRandomGenerator::global()->bounded(WIDTH);
        apple.ry()=QRandomGenerator::global()->bounded(HEIGHT);
        int u=0;
        for(int i=0;i<snake.size();i++)
        {

            if(apple.x()!=snake[i].x() || apple.y()!=snake[i].y())
            {
                u++;
                if(u==snake.size())
                    r=false;
            }
        }
    }
}

void viper::Brain()
{
    double predict;  //ra- кол-во правильных ответов за эпоху, right-правильная цифра, predict-предсказаная цифра, maxra- макс кол-во правильных ответов
    double distanceToApple=1.0/DistanceToApple(); //дистанция до яблока
    double *distanceToBlock; //дистанция до блоков



        distanceToBlock=DistanceToBlock(direction);


//    double input[]={distanceToApple,distanceToBlock[0],distanceToBlock[1], distanceToBlock[2],distanceToBlock[3], double(direction)};
        double input[]={distanceToApple,distanceToBlock[0],distanceToBlock[1], distanceToBlock[2],distanceToBlock[3], 1.0/(direction+1)};

    Net->SetInputs(input); // загружаем входные данные в нейросеть


    predict = Net->ForwardFeed();
    Net->PrintValues();

    MoveStep(int(predict),double(direction));
}

double viper::DistanceToApple()
{
    return sqrt(pow((GetAppleX()-GetSnakeX()),2)+pow((GetAppleY()-GetSnakeY()),2));
}

double *viper::DistanceToBlock(int direction)
{

    static double distance[4];
    int blockX;
    int blockY;
    switch(direction){
    case(0):
        distance[0]=0;            //лево
        for (int i=GetSnakeX(); i<=HEIGHT; i++) {
                    if(BodySnakeInXY(i,GetSnakeY()))
                    {
                        blockX=i;

                    }
                    else
                        blockX=HEIGHT;
                }

        distance[1]=1.0/(-GetSnakeX()+blockX);            //право

                for (int i=GetSnakeX(); i>=0; i--) {
                    if(BodySnakeInXY(GetSnakeY(),i))
                        blockY=i;
                    else
                        blockY=0;
                }

        distance[2]=1.0/(GetSnakeY()-blockY);             //верх

                for (int i=GetSnakeX(); i<=WIDTH; i++) {
                    if(BodySnakeInXY(GetSnakeY(),i))
                        blockY=i;
                    else
                        blockY=WIDTH;
                }

        distance[3]= 1.0/( -GetSnakeY()+blockY);           //низ
        break;
    case(1):
                for (int i=GetSnakeX(); i>=0; i--) {
                    if(BodySnakeInXY(i,GetSnakeY()))
                    {
                        blockX=i;

                     }
                    else
                        blockX=0;
                }

        distance[0]=1.0/(GetSnakeX()-blockX);             //лево
        for (int i=GetSnakeX(); i<=HEIGHT; i++) {
                    if(BodySnakeInXY(i,GetSnakeY()))
                    {
                        blockX=i;

                    }
                    else
                        blockX=HEIGHT;
                }

        distance[1]=1.0/(-GetSnakeX()+blockX);           //право

                for (int i=GetSnakeX(); i>=0; i--) {
                    if(BodySnakeInXY(GetSnakeY(),i))
                        blockY=i;
                    else
                        blockY=0;
                }

        distance[2]=1.0/(GetSnakeY()-blockY);             //верх
        distance[3]=0;           //низ
        break;
    case(2):
        for (int i=GetSnakeX(); i>=0; i--) {
            if(BodySnakeInXY(i,GetSnakeY()))
            {
                blockX=i;

             }
            else
                blockX=0;
        }

        distance[0]=1.0/(GetSnakeX()-blockX);            //лево


        distance[1]=0;            //право

                for (int i=GetSnakeX(); i>=0; i--) {
                    if(BodySnakeInXY(GetSnakeY(),i))
                        blockY=i;
                    else
                        blockY=0;
                }

        distance[2]=1.0/(GetSnakeY()-blockY);           //верх

                for (int i=GetSnakeX(); i<=WIDTH; i++) {
                    if(BodySnakeInXY(GetSnakeY(),i))
                        blockY=i;
                    else
                        blockY=WIDTH;
                }

        distance[3]= 1.0/( -GetSnakeY()+blockY);           //низ
        break;
    case(3):
        for (int i=GetSnakeX(); i>=0; i--) {
            if(BodySnakeInXY(i,GetSnakeY()))
            {
                blockX=i;

             }
            else
                blockX=0;
        }

        distance[0]=1.0/(GetSnakeX()-blockX);           //лево
        for (int i=GetSnakeX(); i<=HEIGHT; i++) {
                    if(BodySnakeInXY(i,GetSnakeY()))
                    {
                        blockX=i;

                    }
                    else
                        blockX=HEIGHT;
                }

        distance[1]=1.0/(-GetSnakeX()+blockX);           //право
        distance[2]=0;            //верх

                for (int i=GetSnakeX(); i<=WIDTH; i++) {
                    if(BodySnakeInXY(GetSnakeY(),i))
                        blockY=i;
                    else
                        blockY=WIDTH;
                }

        distance[3]= 1.0/( -GetSnakeY()+blockY);          //низ
        break;
    }

    return distance;
}

double viper::Reward(int &reward, double &distance, int &score_apple, bool &d)
{
    int one_step=-10;
    int dead=-300;
    int distanceNow=DistanceToApple();

    if (d){ reward+=dead; }
    if (distanceNow<distance)
    {
        reward+=one_step+distanceNow+20;
    }
    else
    {
        reward+=one_step-distanceNow;
    }

    return reward;
}

int viper::GetSnakeX()
{
    return snake[0].x();
}

int viper::GetSnakeY()
{
    return  snake[0].y();
}

int viper::GetAppleX()
{
    return  apple.x();
}

int viper::GetAppleY()
{
    return apple.y();
}

int viper::GetSnakeSize()
{
    return snake.size();
}

int viper::GetSnakeBodyX(int x)
{
    return snake[x].x();
}

int viper::GetSnakeBodyY(int x)
{
    return snake[x].y();
}

bool viper::BodySnakeInXY(int x, int y)
{
    if(snake.size()<4)
        return false;
    for(int i=0;i<snake.size();i++)
    {
        if(snake[i].x()==x || snake[i].y()==y)
            return true;
        else
            return false;
    }
}

void viper::MoveStep(int key, int direc)
{
    dead=false;
    switch (direc){
        case 0:

            if(key==0){ step=start; dir=right; direction=0;}
            if(key==1){ step=start; dir=up; direction=1;}
            if(key==2){ step=start; dir=down; direction=3;}
            break;
        case 1:
            if(key==0){ step=start; dir=up; direction=1;}
            if(key==1){ step=start; dir=left; direction=2;}
            if(key==2){ step=start; dir=right; direction=0;}
            break;
        case 2:
            if(key==0){ step=start; dir=left; direction=2;}
            if(key==1){ step=start; dir=down; direction=3;}
            if(key==2){ step=start; dir=up; direction=1;}
            break;
        case 3:
            if(key==0){ step=start; dir=down; direction=3;}
            if(key==1){ step=start; dir=right; direction=0;}
            if(key==2){ step=start; dir=left; direction=2;}
            break;
    }


        checkApple();
        move();
        checkField();
}

void viper::move()
{
    if(step==start)
    {
        if(eat)
            snake.push_back(QPoint(snake[snake.size()-1].x(),snake[snake.size()-1].y()));
        for(int i=snake.size()-1;i>0;--i)
        {
        snake[i]=snake[i-1];
        eat=false;
        }
    }
    switch (dir) {
    case left:  {if (step==start) snake[0].rx()-=1; step=stop; break; }
    case right: {if (step==start) snake[0].rx()+=1; step=stop; break; }
    case up:    {if (step==start) snake[0].ry()-=1; step=stop; break; }
    case down:  {if (step==start) snake[0].ry()+=1; step=stop; break; }
    case stop:  {break;}
    }

}

void viper::checkField()
{
    for(int i=1;i<snake.size();++i)
    {
        if (snake[0]==snake[i])
            inGame=false;
    }

    if(snake[0].x()>=WIDTH)
        inGame=false;
    if(snake[0].x()<0)
        inGame=false;
    if(snake[0].y()>=HEIGHT)
        inGame=false;
    if(snake[0].y()<0)
        inGame=false;

    if(inGame==false)
        gameOver();
}

void viper::checkApple()
{
    if(snake[0]==apple)
    {
        eat=true;
        setApple();
        score+=1;
    }
}

void viper::gameOver()
{
    dir=stop;
    dead=true;
    inGame=false;
    //initGame();


}


