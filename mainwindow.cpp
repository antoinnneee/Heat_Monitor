#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include "mygitem.h"
#include <QTimer>
#include <QDebug>
#include <QScrollBar>
#include <QPen>
#include "graphmoyenneline.h"
//class GraphicsView : public QGraphicsView
//{
//    Q_OBJECT
//public:
//    GraphicsView(QGraphicsScene *scene, QWidget *parent = NULL) : QGraphicsView(scene, parent)
//    {
//    }

//protected:
//    void resizeEvent(QResizeEvent *event) override
//    {
//        fitInView(scene()->sceneRect());
//        QGraphicsView::resizeEvent(event);
//    }
//};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    moytab = (int*) malloc(sizeof(int) * 100);
    int i = 100;

    while (i--)
    {
        moytab[i] = 0;
    }
    m_stop = 0;
    midline = NULL;
    m_olx = 0;

    moyline = NULL;
    scene = new QGraphicsScene(this);

    updateTimer = new QTimer(this);
    view = new QGraphicsView(scene);
//    midline = new QGraphicsLineItem(ui->gridLayout->geometry().x(), ui->gridLayout->geometry().height()/2, ui->gridLayout->geometry().width(), ui->gridLayout->geometry().height()/2);

    init_graph();

    connect(updateTimer, SIGNAL(timeout()), this, SLOT(myupdate()));


    updateTimer->start(1);
}

void MainWindow::init_graph()
{
    if (scene)
        delete scene;
    scene = new QGraphicsScene(this);
    if (view)
        delete view;

    view = new QGraphicsView(scene);
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
    temperature = qrand() % 150;
    posy = ui->gridLayout->geometry().height()/2 -temperature;
    posx += 1;

    if (count > 2 && posx > 2)
    {
        line = new QGraphicsLineItem(m_olx, m_oldy, posx, posy);
        scene->addItem(line);
    }
    else
    {

        QPen mpen;
        mpen.setColor(Qt::gray);
        if (midline)
            delete midline;

        midline = new QGraphicsLineItem(ui->gridLayout->geometry().x(), ui->gridLayout->geometry().height()/2, ui->gridLayout->geometry().width(), ui->gridLayout->geometry().height()/2);

        midline->setPen(mpen);
        scene->addItem(midline);
    }
    moytab[count%100] = posy;
    count++;
    m_olx = posx;
    m_oldy = posy;

    int i;
    i = 100;

    while(i--)
    {
        moyenne = moyenne + moytab[i];
    }
    if (ui->cBscroll->isChecked())
    {
        view->horizontalScrollBar()->setValue(view->horizontalScrollBar()->maximum());
        midline->moveBy(1, 0);
    }
    else
    {
        if (posx > ui->gridLayout->geometry().width() )
        {
            scene->clear();
            midline = NULL;
            moyline = NULL;
            posx = 0;
        }
    }
    setmoyenne(moyenne / ((count < 100) ? count : 100));
}
void    MainWindow::setmoyenne(int moyenne)
{

    ui->lcdNumber->display(moyenne);
    if (moyline)
        delete moyline;
    QPen moypen;
    moypen.setColor(Qt::red);
    moyline = new QGraphicsLineItem(ui->gridLayout->geometry().x(), moyenne, (m_olx > ui->gridLayout->geometry().width()) ? m_olx : ui->gridLayout->geometry().width(), moyenne);
    moyline->setPen(moypen);
    //    moyline = new graphmoyenneLine(ui->gridLayout->geometry().x(), moyenne, (m_olx > ui->gridLayout->geometry().width()) ? m_olx : ui->gridLayout->geometry().width(), moyenne);
    scene->addItem(moyline);

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
