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
    QPen pen,pen_conv;
    pen.setWidth(1);
    pen.setColor(Qt::gray);
    pen_conv.setWidth(2);
    pen_conv.setColor(Qt::darkGreen);

    if (g_axis_x)
        delete g_axis_x;
    if (g_consigne)
        delete g_consigne;

    if (posx > ui->gridLayout->geometry().width())
    {
        g_axis_x = new QGraphicsLineItem(0, conv_y(0), posx, conv_y(0));
        g_consigne = new QGraphicsLineItem(0,conv_y(m_consigne/10),posx,conv_y(m_consigne/10));
    }
    else
    {
        g_axis_x = new QGraphicsLineItem(ui->gridLayout->geometry().x(), conv_y(0), ui->gridLayout->geometry().width(), conv_y(0));
        g_consigne = new QGraphicsLineItem(ui->gridLayout->geometry().x(),conv_y(m_consigne/10),ui->gridLayout->geometry().width(),conv_y(m_consigne/10));
    }


    g_axis_x->setPen(pen);
    g_consigne->setPen(pen_conv);


    scene->addItem(g_axis_x);
    scene->addItem(g_consigne);
    //quadrillage();

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

            g_consigne= NULL;
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

int MainWindow::conv_y(int to_convert)
{
int convertisseur=3*ui->gridLayout->geometry().height()/4 -to_convert;
return convertisseur;
}

void MainWindow::quadrillage()
{
    QPen quadpen;
    quadpen.setWidth(1);
    quadpen.setColor(Qt::gray);

   if(quad_x < ui->gridLayout->geometry().width())
    {
       g_quady=new QGraphicsLineItem(quad_x, conv_y(0), quad_x, conv_y(ui->gridLayout->geometry().height()));
       qDebug()<<"quadrillage on";
       quad_x= quad_x+m_dep_x;
    }

   //    g_quady=new QGraphicsLineItem(0, conv_y(0), posx, conv_y(0));

    //g_quadx->setPen(quadpen);
    g_quady->setPen(quadpen);
    //scene->addItem(g_quadx);
    scene->addItem(g_quady);

}
