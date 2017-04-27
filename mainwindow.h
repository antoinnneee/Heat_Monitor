#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include "graphmoyenneline.h"
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

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsLineItem *moyline;
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
