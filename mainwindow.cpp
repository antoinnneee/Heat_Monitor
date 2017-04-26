#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include "mygitem.h"
#include <QTimer>


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

    scene = new QGraphicsScene(this);

    int nbitem = 4;
    int posx = 0;
    int posy = 0;
    m_oldy = scene->height()/2;
    m_olx = 0;
    while (nbitem--)
    {
        posx += 50;
        posy += 50;
        QColor color(Qt::black);
        QGraphicsItem *item = new myGitem(color, posx, posy);
        item->setPos(QPointF(posx, posy));
        scene->addItem(item);
    }

    QGraphicsView *view = new QGraphicsView(scene);
    view->setGeometry(ui->gridLayout->geometry());

    ui->gridLayout->addWidget(view);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(myupdate()));
    connect(timer, SIGNAL(timeout()), timer, SLOT(start()));
    timer->start(100);

}

void MainWindow::myupdate()
{
    static int count = 0;
    static int posx = 0;
    static int posy = 0 ;
    static int moyarray[100] ;
    int moyenne = 0;


    posy = qrand() % 100 + scene->height()/2;
    posx += 1;


    QColor color(Qt::black);

    QGraphicsLineItem *line= new QGraphicsLineItem(m_olx, m_oldy, posx, posy);
//    QGraphicsEllipseItem *point = new QGraphicsEllipseItem(posx, posy, 2, 2);

//    scene->children()
//    line->setPos(QPointF(oldx, oldy));
//    item->setPos(QPointF(posx, posy));
//    scene->addItem(point);
//    scene->addItem(item);
    scene->addItem(line);
    moyarray[count%100] = posy;
    count++;
    m_olx = posx;
    m_oldy = posy;

    int i;
    i = 100;
    while(i--)
    {
        moyenne = moyenne + moyarray[i];
    }
    // moyenne = moyenne / 100
//    ui->lcdNumber->display(moyenne/100);
//    oldx = posx;
//    oldy = posy;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
//    if (ui->graphicsView->isHidden())
//    {
////        ui->graphicsView->show();
//    }
//    else
//    {
////        ui->graphicsView->hide();

//    }
}

void MainWindow::on_pushButton_2_clicked()
{
    static int state = 0;
    state++;
//    QGraphicsLineItem *line= new QGraphicsLineItem(ui->x1->value(), ui->y1->value(), ui->x2->value(), ui->y2->value());

    scene->addItem(line);

}
