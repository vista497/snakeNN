
int lay1=5;
#include <QApplication>
#include "game.h"
#include "snake.h"
#define COUNT_SNAKE 3
#define LAY1 7
#define LAY2 50
#define LAY3 4

int main(int argc, char *argv[])
{
   QApplication app(argc,argv);
   game snake;
   snake.show();
    return app.exec();
}
