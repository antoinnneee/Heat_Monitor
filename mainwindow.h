#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QPainterPath>

#include "graphmoyenneline.h"

#define NBVALMOY 250

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void myupdate();
    void on_pushButton_2_clicked();
    void setmoyenne(int moyenne);
    void on_cBscroll_toggled(bool checked);
    void init_graph();
    void on_send_clicked();
    void update_graph(int val);


private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsLineItem *moyline;
    QGraphicsPathItem *moypath;
    QPainterPath *pmoy;
    QTimer *updateTimer;
    QGraphicsView *view;
    QGraphicsLineItem *midline;
    int temperature;
    int *moytab;
    int moyenne;
    int m_olx;
    int m_oldy;
    bool m_stop;

};

#endif // MAINWINDOW_H
