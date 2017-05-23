#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
<<<<<<< HEAD
#include <QTimer>
#include <QDebug>
#include <QScrollBar>
#include <QPen>
#include <QPainterPath>
#include <QFuture>
#include <QtConcurrent>
#include <QtSerialPort/QSerialPort>
#include "serialengine.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

     QThread* thread = new QThread;
    serial = new serialengine();
    serial->moveToThread(thread);
    QObject::connect(thread, SIGNAL (finished()), thread, SLOT (deleteLater()));
    thread->start();
    serial->start();
    m_dep_x = DEP_X;
    m_nbvalmoy = ui->sB_nbMoyenne->value();
    this->setWindowTitle("HeatMonitor");
    this->setWindowIcon(QIcon(":/icon/driftnet-1.png"));

    moytab = (int*) malloc(sizeof(int) * NBVALMOY);
    int i = m_nbvalmoy;
=======
#include "mygitem.h"
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <qdebug.h>
#include <QScrollBar>
#include <QPen>
#include <QPainterPath>


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

>>>>>>> a0a1bff0564b3ae3d26e3fb633296c1a792d51c0
    while (i--)
    {
        moytab[i] = 0;
    }
<<<<<<< HEAD

    m_stop = 0;
    g_axis_x = NULL;
    g_consigne = NULL;
    m_consigne = 0;
    m_oldconsigne = 0;
    m_olx = 0;
    m_moyennepath = NULL;
    moyenne = 0;
    m_timereset = 100;
//    future = new QFuture<int>;

    g_moyennepath = NULL;
    g_datagraph = NULL;

    yqpath = NULL;
    ypath = NULL;
    m_datapath= NULL;
    yline= NULL;
    yqline= NULL;
    scene = new QGraphicsScene(this);
    updateTimer = new QTimer(this);
    view = new QGraphicsView(scene);


    //    g_axis_x = new QGraphicsLineItem(ui->gridLayout->geometry().x(), ui->gridLayout->geometry().height()/2, ui->gridLayout->geometry().width(), ui->gridLayout->geometry().height()/2);

    init_graph();

    connect(updateTimer, SIGNAL(timeout()), this, SLOT(myupdate()), Qt::QueuedConnection);
//    connect(view->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(update_graph(int)));

    updateTimer->start(m_timereset);
}


void MainWindow::init_graph()
{
    if (g_moyennepath)
        delete g_moyennepath;
=======
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
>>>>>>> a0a1bff0564b3ae3d26e3fb633296c1a792d51c0
    if (scene)
        delete scene;
    scene = new QGraphicsScene(this);
    if (view)
        delete view;
<<<<<<< HEAD
    view = new QGraphicsView(scene);
    g_axis_x = NULL;
    g_moyennepath = NULL;
    g_datagraph = NULL;
=======

    view = new QGraphicsView(scene);
    midline = NULL;
>>>>>>> a0a1bff0564b3ae3d26e3fb633296c1a792d51c0
    m_oldy = scene->height()/2;
    view->setGeometry(ui->gridLayout->geometry());
    ui->gridLayout->addWidget(view);

<<<<<<< HEAD

=======
>>>>>>> a0a1bff0564b3ae3d26e3fb633296c1a792d51c0
}

void MainWindow::myupdate()
{
    static long long count = 0;
    static int posx = 0;
<<<<<<< HEAD
    static int posy = 0;
    disconnect(updateTimer, SIGNAL(timeout()), this, SLOT(myupdate()));

    temperature = serial->get_temp();

    // Display temperature (lcdNumber)
    ui->lcdN_Data->display(temperature);

    // calc pos on view
    posy = conv_y(temperature/10);
    posx += m_dep_x;

    if (posx > ui->gridLayout->geometry().width())
        draw_background(posx);
    if (count > 1 && posx > m_dep_x)
    {
        drawfirststep(posx, posy);
    }
    else
    {
        // first loop after reset
        //  init part draw x axis
        draw_background(0);
    }

    //  calc moyenne (qfuture)
    moyenne_update(posx, posy, &count); // count++ here
    endpos_update(&posx);
    if (!m_moyennepath)
    {
        m_moyennepath = new QPainterPath();
        m_moyennepath->moveTo(posx, moyenne / ((count < m_nbvalmoy) ? count : m_nbvalmoy));
    }
    m_moyennepath->lineTo(posx, moyenne / ((count < m_nbvalmoy) ? count : m_nbvalmoy));
    setmoyenne( moyenne / ((count < m_nbvalmoy) ? count : m_nbvalmoy));

    connect(updateTimer, SIGNAL(timeout()), this, SLOT(myupdate()), Qt::QueuedConnection);



}



=======
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
        yqline->setPen(yqpen);
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
>>>>>>> a0a1bff0564b3ae3d26e3fb633296c1a792d51c0
MainWindow::~MainWindow()
{
    delete ui;
}

<<<<<<< HEAD

void MainWindow::on_b_stop_clicked()
=======
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
>>>>>>> a0a1bff0564b3ae3d26e3fb633296c1a792d51c0
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
<<<<<<< HEAD
        if (m_olx + m_dep_x >  ui->gridLayout->geometry().width())
            init_graph();
    }

}

void MainWindow::on_send_clicked()
{
    double temp_send=ui->doubleSpinBox->value();
    m_consigne = temp_send;
    scene->removeItem(g_consigne);
    if(g_consigne)
    delete g_consigne;
    g_consigne = NULL;
    draw_background(0);
    serial->write(ui->doubleSpinBox->text());
    qDebug()<<temp_send;
}

void MainWindow::on_sB_Rand_valueChanged(int arg1)
{
    serial->set_rand(arg1);
}

void MainWindow::on_sB_speed_valueChanged(int arg1)
{
    serial->set_speed(arg1);
}


void MainWindow::on_sB_update_valueChanged(int arg1)
{
    updateTimer->stop();
    m_timereset = arg1;
    updateTimer->start(m_timereset);

}

void MainWindow::on_b_open_clicked()
{
    serial->set_sername(ui->tE_Name->toPlainText());
    serial->set_speed(ui->sB_speed->value());
    serial->set_baudrate(ui->cB_BaudRate->currentText().toInt());
    switch (ui->cB_DataBits->currentIndex()) {
    case 0:
        serial->set_databits(8);
        break;
    case 1:
        serial->set_databits(5);
        break;
    case 2:
        serial->set_databits(6);
        break;
    case 3:
        serial->set_databits(7);
        break;
    case 4:
        serial->set_databits(-1);
        break;
    default:
        break;
    }
    serial->set_parity(ui->cB_Parity->currentIndex());
    serial->set_stopBits(ui->cB_StopBits->currentIndex());
    serial->set_flowcontrol(ui->cB_FlowControl->currentIndex());
    serial->openSerialPort();
}

void MainWindow::on_b_serClose_clicked()
{
    serial->closeSerialPort();
}

void MainWindow::on_sB_movex_valueChanged(int arg1)
{
    m_dep_x = arg1;
}


void MainWindow::on_radioButton_clicked()
{
    if (ui->radioButton->isEnabled())
       g_moyennepath->hide();
    else
       g_moyennepath->show();
}

void MainWindow::on_sB_nbMoyenne_valueChanged(int arg1)
{
    m_nbvalmoy = arg1;
       if (moytab)
    delete moytab;
       future.cancel();
    moytab = (int*) malloc(sizeof(int) * m_nbvalmoy);

    int i = m_nbvalmoy;
    while (i--)
    {
        moytab[i] = 0;
    }

}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{

=======
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
    sprintf(temp, "S%g", temp_send);
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
>>>>>>> a0a1bff0564b3ae3d26e3fb633296c1a792d51c0
}
