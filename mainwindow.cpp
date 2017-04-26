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
    timer->start(10);
}

void MainWindow::myupdate()
{
    static int posx = 0;
    static int count = 0;
    static int posy = 0 ;
    static int moyarray[100] ;
    int moyenne = 0;
    posx ++;
    /*if(serial->isOpen())
    qDebug() << "update open";
    else
    qDebug() << "update not open";*/
    readData();
     posy =500-temperature/10;
    //qDebug() << temperature;
    QColor color(Qt::blue);
    QGraphicsLineItem *line= new QGraphicsLineItem(m_olx, m_oldy, posx, posy);
    //QGraphicsItem *item = new myGitem(color, posx, posy);
    //item->setPos(QPointF(posx, posy));
    scene->addItem(line);
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
//     moyenne = moyenne / 100;
     ui->lcdNumber->display(moyenne/100);
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
    int posx = ui->tex->toPlainText().toInt();
    int posy = ui->tey->toPlainText().toInt();
    QColor color(Qt::black);
    QGraphicsItem *item = new myGitem(color, posx, posy);
    item->setPos(QPointF(posx, posy));
    scene->addItem(item);


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
    char buffer[5]={'0','0','0','0','0'};
    serial->write("$");
    _sleep(100);
    serial->readLine(buffer, 5);
    QString data=buffer;
    if(data.toInt()!=0)
    temperature= data.toInt() ;
    qDebug() << temperature ;

}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        qDebug() << serial->errorString();
        closeSerialPort();
    }
}













