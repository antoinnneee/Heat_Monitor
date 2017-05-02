#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
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
    ui->gridLayout_4->setGeometry(ui->centralWidget->rect());
    this->setWindowTitle("HeatMonitor");
    this->setWindowIcon(QIcon(":/icon/heatcon.png"));
//    QMainWindow::centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);

    moytab = (int*) malloc(sizeof(int) * NBVALMOY);
    int i = m_nbvalmoy;
    while (i--)
    {
        moytab[i] = 0;
    }

    m_stop = 0;
    g_axis_x = NULL;
    m_consigne = 0;
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
//    ui->gridLayout_4->
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
    if (scene)
        delete scene;
    scene = new QGraphicsScene(this);
    if (view)
        delete view;
    view = new QGraphicsView(scene);
    g_axis_x = NULL;
    g_moyennepath = NULL;
    g_datagraph = NULL;
    m_oldy = scene->height()/2;
    view->setGeometry(ui->gridLayout->geometry());
    ui->gridLayout->addWidget(view);


}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    qDebug() << " width :" << this->geometry().width() << "  height :" << this->geometry().height();
}

void MainWindow::myupdate()
{
    static long long count = 0;
    static int posx = 0;
    static int posy = 0;

    disconnect(updateTimer, SIGNAL(timeout()), this, SLOT(myupdate()));

    temperature = serial->get_temp();

    // Display temperature (lcdNumber)
    ui->lcdN_Data->display(temperature);

    // calc pos on view
    posy = ui->gridLayout->geometry().height()/2 -temperature;
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



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_b_stop_clicked()
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
        if (m_olx + m_dep_x >  ui->gridLayout->geometry().width())
            init_graph();
    }

}

void MainWindow::on_send_clicked()
{
    double temp_send=ui->doubleSpinBox->value();
    serial->write(ui->doubleSpinBox->text());
    m_consigne = (int)temp_send;
   // serial->write(temp_send);

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
    if (ui->radioButton->isChecked())
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
