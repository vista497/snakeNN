#include "game.h"
#include <QLayout>

game::game(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle("Snake"); // название окна
    button=new QRadioButton(tr("Включить нейросеть"));
    Neuro=new QPushButton("Обучить нейросеть");
    board=new canvas;
    s=new Snake();
    board->initGame(1);

    QGridLayout *lay=new QGridLayout;
    lay->addWidget(board);
    lay->addWidget(button);
    lay->addWidget(Neuro);
    setLayout(lay);

    QObject::connect(button, SIGNAL(clicked()), this, SLOT(brain()));
    QObject::connect(Neuro, SIGNAL(clicked()), this, SLOT(NetShow()));
}

void game::brain()
{
    board->brain=button->isChecked();
    board->brII=button->isChecked();

    if(button->isChecked()==false)
    {
        //board->initGame(1);
    }
}

void game::NetShow()
{

    s->show();
}





