#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include "mygitem.h"
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <qdebug.h>
#include <QScrollBar>
#include <QPen>
#include <QPainterPath>
#include "serialengine.h"
#include <QtConcurrent>
#include <QFuture>





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),

    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    temperature = 0;
    m_timereset=200;
    m_dep_x = 1;
    serial = new QSerialPort(this);
    openSerialPort();
    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MainWindow::handleError);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
    moytab = (int*) malloc(sizeof(int) * NBVALMOY);
    int i = NBVALMOY;

    while (i--)
    {
        moytab[i] = 0;
    }
    m_stop = 0;
    midline = NULL;
    m_olx = 0;
    line_temp= NULL;
    yline= NULL;
    yqline= NULL;
    pmoy = NULL;
    moyline = NULL;
    scene = new QGraphicsScene(this);

    updateTimer = new QTimer(this);
    view = new QGraphicsView(scene);
    moypath = NULL;
    tempath = NULL;
    yqpath = NULL;
    ypath = NULL;
    //    midline = new QGraphicsLineItem(ui->gridLayout->geometry().x(), ui->gridLayout->geometry().height()/2, ui->gridLayout->geometry().width(), ui->gridLayout->geometry().height()/2);

    init_graph();
    readData();
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(myupdate()), Qt::QueuedConnection);
    connect(updateTimer, SIGNAL(timeout()), updateTimer, SLOT(start()), Qt::QueuedConnection);
    updateTimer->start(m_timereset);

}

void MainWindow::init_graph()
{
    if (scene)
        delete scene;
    scene = new QGraphicsScene(this);
    if (view)
        delete view;

    view = new QGraphicsView(scene);
    midline = NULL;
    m_oldy = scene->height()/2;
    view->setGeometry(ui->gridLayout->geometry());
    ui->gridLayout->addWidget(view);

}

void MainWindow::myupdate()
{
    static long long count = 0;
    static int posx = 0;
    static int posy = 0 ;
    moyenne = 0;
    posy = ui->gridLayout->geometry().height()/2 -temperature/10;
    posx += m_dep_x;
    ui->lcdnumber3->display(temperature);


    if (count > 2 && posx > 2)
    {

        //Affichage courbe température
        QPen tempen;
        tempen.setWidth(2);
        tempen.setColor(Qt::darkMagenta);
        if (!line_temp)
        {
        line_temp = new QPainterPath();
        line_temp->moveTo(posx, posy);
        }
        line_temp->lineTo(posx, posy );
        if (tempath)
            delete tempath;
        tempath = new QGraphicsPathItem();
        tempath->setPath(*line_temp);
        tempath->setPen(tempen);
        scene->addItem(tempath);

        //Affichage axe x
        QPen mpen;
        mpen.setColor(Qt::black);
        mpen.setWidth(1);
        if (midline)
           delete midline;
        if (posx > ui->gridLayout->geometry().width())
           midline = new QGraphicsLineItem(ui->gridLayout->geometry().x(), ui->gridLayout->geometry().height()/2, posx, ui->gridLayout->geometry().height()/2);
        else
            midline = new QGraphicsLineItem(ui->gridLayout->geometry().x(), ui->gridLayout->geometry().height()/2, ui->gridLayout->geometry().width(), ui->gridLayout->geometry().height()/2);
        midline->setPen(mpen);
        scene->addItem(midline);

        //Affichage axe y

        if (!yline)
        {
        yline = new QPainterPath();
        yline->moveTo(ui->gridLayout->geometry().width()/2,0);
        }
        yline->lineTo(ui->gridLayout->geometry().width()/2, ui->gridLayout->geometry().height() );
        if (ypath)
            delete ypath;
        ypath = new QGraphicsPathItem();
        ypath->setPath(*yline);
        midline->setPen(mpen);
        scene->addItem(ypath);

        // Quadrillage y
        // y=m_dep_x*(1000/m_timereset)

        QPen yqpen;
        yqpen.setColor(Qt::gray);
        yqpen.setWidth(1);
        if (!yqline)
        {
        yqline = new QPainterPath();
        yqline->moveTo(m_dep_x*(1000/m_timereset),0);
        }
        yqline->lineTo(m_dep_x*(1000/m_timereset), ui->gridLayout->geometry().height() );
        if (yqpath)
            delete yqpath;
        yqpath = new QGraphicsPathItem();
        yqpath->setPath(*yqline);
        // yqline->setPen(yqpen);
        scene->addItem(yqpath);
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
MainWindow::~MainWindow()
{
    delete ui;
}

void    MainWindow::setmoyenne(int moyenne)
{
    ui->lcdNumber->display(moyenne);
    if (moyline)
        delete moyline;
    if (moypath)
        delete moypath;
    QPen moypen;
    moypen.setWidth(2);
    moypen.setColor(Qt::red);

    moypath = new QGraphicsPathItem();
    moypath->setPath(*pmoy);
    moypath->setPen(moypen);
    scene->addItem(moypath);
    if(ui->radioButton->isChecked())
    {
        moypath->hide();
    }
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
    serial->write("$");
    QTimer *timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(just_read()), Qt::QueuedConnection);
    timer2->setSingleShot(true);
    timer2->start(100);
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
    char temp[9];
    double temp_send=ui->doubleSpinBox->value();
    sprintf(temp, "C%g", temp_send);
    serial->write(temp);
    //test
//    char test[10];
//    serial->readLine(test, 10);
//    QString recept_test=test;
//    qDebug()<<recept_test;
}

void MainWindow::just_read()
{

serial->readLine(buffer, 6);
QString donnees=buffer;
//qDebug()<< " size: " << donnees.size();

 if(donnees.size()<5 || donnees[0]!='B')
    {
     readData();
    }
 else
{
donnees[0]='0';
temperature=donnees.toInt() ;
}
//qDebug()<< temperature;
}



void MainWindow::on_radioButton_clicked()
{
   moypath->hide();
}
