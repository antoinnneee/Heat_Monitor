#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QDebug>
#include <QScrollBar>
#include <QPen>
#include <QGraphicsLineItem>
#include <QtConcurrent>
#include <QPainterPath>
#include <QtSerialPort/QSerialPort>


int calc_moyenne(int *marray, int nb_val)
{
    int i;
    i = nb_val;

    int moy = 0;;

    while(i--)
    {
        moy = moy + marray[i];
    }
    return moy;
}

void MainWindow::draw_background(int posx)
{
    //Affichage axe x
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::gray);
    if (g_axis_x)
        delete g_axis_x;
    if (posx > ui->gridLayout->geometry().width())
        g_axis_x = new QGraphicsLineItem(ui->gridLayout->geometry().x(), ui->gridLayout->geometry().height()/2, posx, ui->gridLayout->geometry().height()/2);
    else
        g_axis_x = new QGraphicsLineItem(ui->gridLayout->geometry().x(), ui->gridLayout->geometry().height()/2, ui->gridLayout->geometry().width(), ui->gridLayout->geometry().height()/2);
    g_axis_x->setPen(pen);
    scene->addItem(g_axis_x);



}

void MainWindow::drawfirststep(int posx, int posy)
{

    //Affichage courbe température
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::darkMagenta);
    if (!m_datapath)
    {
        m_datapath = new QPainterPath();
        m_datapath->moveTo(posx, posy);
    }
    m_datapath->lineTo(posx, posy );
    if (g_datagraph)
        delete g_datagraph;
    g_datagraph = new QGraphicsPathItem();
    g_datagraph->setPath(*m_datapath);
    g_datagraph->setPen(pen);
    scene->addItem(g_datagraph);



    //Affichage axe y
//    if (!yline)
//    {
//        yline = new QPainterPath();
//        yline->moveTo(ui->gridLayout->geometry().width()/2,0);
//    }
//    yline->lineTo(ui->gridLayout->geometry().width()/2, ui->gridLayout->geometry().height() );
//    if (ypath)
//        delete ypath;
//    ypath = new QGraphicsPathItem();
//    ypath->setPath(*yline);
//    g_axis_x->setPen(pen);
//    scene->addItem(ypath);

//    // Quadrillage y
//    // y=m_dep_x*(1000/m_timereset)

//    QPen yqpen;
//    yqpen.setColor(Qt::gray);
//    yqpen.setWidth(1);
//    if (!yqline)
//    {
//        yqline = new QPainterPath();
//        yqline->moveTo(m_dep_x*(1000/m_timereset),0);
//    }
//    yqline->lineTo(m_dep_x*(1000/m_timereset), ui->gridLayout->geometry().height() );
//    if (yqpath)
//        delete yqpath;
//    yqpath = new QGraphicsPathItem();
//    yqpath->setPath(*yqline);
//    yqpath->setPen(yqpen);
//    scene->addItem(yqpath);
}

void MainWindow::moyenne_update(const int posx, const int posy, long long *count)
{

    moytab[*count%m_nbvalmoy] = posy;
    *count = *count + 1;
    m_olx = posx;
    m_oldy = posy;
    /* calc moyenne */
    if (future.isFinished() || future.isPaused())
    {
        if (future.isFinished() && *count > 1 )
            moyenne = future.result();
        future = QtConcurrent::run(calc_moyenne,moytab, m_nbvalmoy);
    }

}

void    MainWindow::setmoyenne(int moyenne)
{

    ui->lcdN_Moyenne->display(moyenne);

    if (g_moyennepath)
        delete g_moyennepath;

    QPen moypen;
    moypen.setWidth(2);
    moypen.setColor(Qt::red);

    g_moyennepath = new QGraphicsPathItem();
    g_moyennepath->setPath(*m_moyennepath);
    g_moyennepath->setPen(moypen);
    if (ui->radioButton->isChecked())
        scene->addItem(g_moyennepath);

}

void MainWindow::endpos_update(int *posx)
{
    if (ui->cBscroll->isChecked() && *posx > ui->gridLayout->geometry().width())
    {
        view->horizontalScrollBar()->setValue(view->horizontalScrollBar()->maximum());
    }
    else
    {
        if (*posx > ui->gridLayout->geometry().width() )
        {
            scene->clear();


            g_datagraph = NULL;
            m_datapath = NULL;
            if (m_moyennepath)
                delete m_moyennepath;
            m_moyennepath = NULL;
            g_moyennepath = NULL;

            g_axis_x = NULL;
            *posx = 0;
        }
    }
}
