#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFuture>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QScrollBar>
#include <QPen>
#include <QPainterPath>
#include "serialengine.h"

#define NBVALMOY 250
#define DEP_X 5

namespace Ui {
class MainWindow;
}
extern int calc_moyenne(int *marray, int nb_val);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void myupdate();
    void on_b_stop_clicked();
    void setmoyenne(int moyenne);
    void on_cBscroll_toggled(bool checked);
    void init_graph();
    void on_send_clicked();
    void drawfirststep(int posx, int posy);
    void moyenne_update(int posx, int posy, long long *count);
    void endpos_update(int *posx);
    void on_sB_Rand_valueChanged(int arg1);
    void on_sB_speed_valueChanged(int arg1);
    void on_sB_update_valueChanged(int arg1);
    void on_b_open_clicked();
    void on_b_serClose_clicked();
    void on_radioButton_clicked();
    void on_sB_movex_valueChanged(int arg1);
    void resizeEvent(QResizeEvent *event);
    void draw_background(int posx);
    void on_sB_nbMoyenne_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;


    QPainterPath *m_moyennepath;
    QGraphicsPathItem *g_moyennepath;

    QGraphicsLineItem *g_axis_x;

    QPainterPath *yline;
    QPainterPath *yqline;
    QGraphicsPathItem *ypath;
    QGraphicsPathItem *yqpath;

    QGraphicsPathItem *g_datagraph;
    QPainterPath *m_datapath;


    QTimer *updateTimer;
    QGraphicsView *view;
    QFuture<int> future;
    serialengine *serial;

    int m_consigne;
    int m_nbvalmoy;
    int temperature;
    int *moytab;
    int moyenne;
    int m_olx;
    int m_oldy;
    int m_dep_x;
    int m_timereset;


    bool m_stop;

};

#endif // MAINWINDOW_H
