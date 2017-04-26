#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include "mygitem.h"
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <qdebug.h>


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
    temperature = 0;
    scene = new QGraphicsScene(this);
    serial = new QSerialPort(this);

    openSerialPort();
    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MainWindow::handleError);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);


    int nbitem = 0;
    int posx = 0;
    int posy = 0;
    m_oldy = scene->height()/2;
    m_olx = 0;
    while (nbitem--)
    {
        posx += 50;
        posy += 50;
        QColor color(Qt::blue);
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
    timer->start(200);
}

void MainWindow::myupdate()
{
    static int posx = 0;
    static int count = 0;
    static int posy = 0 ;
    static int moyarray[100] ;
    int moyenne = 0;
    QGraphicsLineItem *line;
    posx ++;
    /*if(serial->isOpen())
    qDebug() << "update open";
    else
    qDebug() << "update not open";*/
    readData();
     posy =500-temperature/10;
    qDebug() << temperature;

     QColor color(Qt::blue);
    //QGraphicsItem *item = new myGitem(color, posx, posy);
    //item->setPos(QPointF(posx, posy));
    if (count > 2 && posx > 2)
    {
        line = new QGraphicsLineItem(m_olx, m_oldy, posx, posy);
        scene->addItem(line);
    }
    moyarray[count%100] = temperature;
    count++;
    m_olx = posx;
    m_oldy = posy;

    int i;
    i = 100;
    while(i--)
    {
        moyenne = moyenne + moyarray[i];
    }
    ui->lcdNumber->display( moyenne / (count < 100 ? count : 100));
    //qDebug()<< scene->width();
    if (posx > ui->gridLayout->geometry().width() )
    {
        scene->clear();
        posx = 0;
    }

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


void MainWindow::openSerialPort()
{

    serial->setPortName("COM4");
    serial->setBaudRate(115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if(!serial->open(QIODevice::ReadWrite))
    {
        qDebug()<<"erreur";
    }

}

void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    //qDebug()<<"close";
}

void MainWindow::readData()
{
    //buffer[5]={'0','0','0','0','0'};
    serial->write("$");
    QTimer *timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(just_read()));
    timer2->start(100);
   // qDebug() << temperature ;

}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        qDebug() << serial->errorString();
        closeSerialPort();
    }
}

void MainWindow::on_send_clicked()
{

    double temp_send=ui->doubleSpinBox->value();
   // serial->write(temp_send);
    qDebug()<<temp_send;
}

void MainWindow::just_read()
{

serial->readLine(buffer, 5);
QString donnees=buffer;
if(donnees.size()<5)
{
    while(donnees.size()<5)
    {

       serial->readLine(buffer, 5);
       donnees=buffer;
       qDebug()<< donnees << " size: " << donnees.size();
    }

}
if(donnees.toInt()!=0)
temperature=donnees.toInt() ;
}
