#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QDebug>
#include <QScrollBar>
#include <QPen>
#include "graphmoyenneline.h"

#include <QPainterPath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    moytab = (int*) malloc(sizeof(int) * NBVALMOY);
    int i = NBVALMOY;

    while (i--)
    {
        moytab[i] = 0;
    }
    m_stop = 0;
    midline = NULL;
    m_olx = 0;
    pmoy = NULL;

    moyline = NULL;
    scene = new QGraphicsScene(this);
    updateTimer = new QTimer(this);
    view = new QGraphicsView(scene);
    moypath = NULL;

    //    midline = new QGraphicsLineItem(ui->gridLayout->geometry().x(), ui->gridLayout->geometry().height()/2, ui->gridLayout->geometry().width(), ui->gridLayout->geometry().height()/2);

    init_graph();

    connect(updateTimer, SIGNAL(timeout()), this, SLOT(myupdate()));
//    connect(view->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(update_graph(int)));

    updateTimer->start(1);
}

void MainWindow::update_graph(int val)
{
    if (midline)
    {
//        delete midline;


    }
}

void MainWindow::init_graph()
{
    if (scene)
        delete scene;
    scene = new QGraphicsScene(this);
    if (view)
        delete view;
//    if (moyline)
//        delete moyline;

    view = new QGraphicsView(scene);

    midline = NULL;
    m_oldy = scene->height()/2;
    view->setGeometry(ui->gridLayout->geometry());
    ui->gridLayout->addWidget(view);

}

void MainWindow::myupdate()
{
    static int count = 0;
    static int posx = 0;
    static int posy = 0 ;
    moyenne = 0;
    QGraphicsLineItem *line;
    temperature = qrand() % 200;
    posy = ui->gridLayout->geometry().height()/2 -temperature;
    posx += 1;

    if (count > 2 && posx > 2)
    {
        line = new QGraphicsLineItem(m_olx, m_oldy, posx, posy);
        scene->addItem(line);
        QPen mpen;
        mpen.setColor(Qt::gray);
        if (midline)
            delete midline;

        if (posx > ui->gridLayout->geometry().width())
            midline = new QGraphicsLineItem(ui->gridLayout->geometry().x(), ui->gridLayout->geometry().height()/2, posx, ui->gridLayout->geometry().height()/2);
        else
            midline = new QGraphicsLineItem(ui->gridLayout->geometry().x(), ui->gridLayout->geometry().height()/2, ui->gridLayout->geometry().width(), ui->gridLayout->geometry().height()/2);

        midline->setPen(mpen);
        scene->addItem(midline);

    }

    moytab[count%NBVALMOY] = posy;
    count++;
    m_olx = posx;
    m_oldy = posy;

    int i;
    i = NBVALMOY;

    while(i--)
    {
        moyenne = moyenne + moytab[i];
    }
    if (ui->cBscroll->isChecked() && posx > ui->gridLayout->geometry().width())
    {
        view->horizontalScrollBar()->setValue(view->horizontalScrollBar()->maximum());
//        midline->moveBy(1, 0);
    }
    else
    {
        if (posx > ui->gridLayout->geometry().width() )
        {
            scene->clear();
            if (pmoy)
                delete pmoy;
            pmoy = NULL;
            moypath = NULL;

            midline = NULL;
            moyline = NULL;
            posx = 0;
        }
    }
    if (!pmoy)
    {
        pmoy = new QPainterPath();
        pmoy->moveTo(posx, moyenne / ((count < NBVALMOY) ? count : NBVALMOY));
    }
    pmoy->lineTo(posx, moyenne / ((count < NBVALMOY) ? count : NBVALMOY));

    setmoyenne(moyenne / ((count < NBVALMOY) ? count : NBVALMOY));

}
void    MainWindow::setmoyenne(int moyenne)
{

    ui->lcdNumber->display(moyenne);
    if (moyline)
        delete moyline;
    if (moypath)
        delete moypath;
    QPen moypen;
    moypen.setColor(Qt::red);

    moypath = new QGraphicsPathItem();
    moypath->setPath(*pmoy);
    moypath->setPen(moypen);
    scene->addItem(moypath);

//    moyline = new QGraphicsLineItem(ui->gridLayout->geometry().x(), moyenne, (m_olx > ui->gridLayout->geometry().width()) ? m_olx : ui->gridLayout->geometry().width(), moyenne);
//    moyline->setPen(moypen);
//    //    moyline = new graphmoyenneLine(ui->gridLayout->geometry().x(), moyenne, (m_olx > ui->gridLayout->geometry().width()) ? m_olx : ui->gridLayout->geometry().width(), moyenne);

//    scene->addItem(moyline);

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{
    static int state = 0;
    state++;
    if (state%2)
    {
        disconnect(updateTimer, SIGNAL(timeout()), this, SLOT(myupdate()));
    }
    else
    {
        connect(updateTimer, SIGNAL(timeout()), this, SLOT(myupdate()));
    }
}

void MainWindow::on_cBscroll_toggled(bool checked)
{
    if (!checked)
    {
        init_graph();
    }
}

void MainWindow::on_send_clicked()
{

}
