#ifndef MAINWINDOW_H
#define MAINWINDOW_H
<<<<<<< HEAD

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
=======
#include <QtCore/QtGlobal>
#include <QMainWindow>
#include <QGraphicsView>
#include <QtSerialPort/QSerialPort>
#include <QPainterPath>

#define NBVALMOY 250
namespace Ui {
class MainWindow;
}
>>>>>>> a0a1bff0564b3ae3d26e3fb633296c1a792d51c0

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
<<<<<<< HEAD
    void myupdate();
    void on_b_stop_clicked();
    void quadrillage();
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
    int conv_y(int to_convert);
    void draw_background(int posx);
    void on_sB_nbMoyenne_valueChanged(int arg1);

    void on_pushButton_clicked();

    void on_doubleSpinBox_valueChanged(double arg1);
=======
    void on_pushButton_2_clicked();
    void myupdate();
    void setmoyenne(int moyenne);
    void on_cBscroll_toggled(bool checked);
    void init_graph();
    void openSerialPort();
    //void writeData(const QByteArray &data);
    void closeSerialPort();
   // void about(){};

    void readData();
    void just_read();
    void handleError(QSerialPort::SerialPortError error);

   void on_send_clicked();


   void on_radioButton_clicked();
>>>>>>> a0a1bff0564b3ae3d26e3fb633296c1a792d51c0

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
<<<<<<< HEAD


    QPainterPath *m_moyennepath;
    QGraphicsPathItem *g_moyennepath;

    QGraphicsLineItem *g_axis_x;

    QPainterPath *yline;
    QPainterPath *yqline;
    QGraphicsPathItem *ypath;
    QGraphicsPathItem *yqpath;

    QGraphicsPathItem *g_datagraph;
    QPainterPath *m_datapath;
    QGraphicsLineItem *g_consigne;
    QGraphicsLineItem *g_quadx;
    QGraphicsLineItem *g_quady;

    QTimer *updateTimer;
    QGraphicsView *view;
    QFuture<int> future;
    serialengine *serial;

    int m_consigne;
    int m_oldconsigne;
    int m_nbvalmoy;
    int temperature;
    int *moytab;
    int moyenne;
    int m_olx;
    int m_oldy;
    int m_dep_x;
    int m_timereset;
    int quad_x= m_dep_x;

    bool m_stop;


=======
    QGraphicsLineItem *moyline;
    QTimer *updateTimer;
    QGraphicsView *view;
    QGraphicsLineItem *midline;
    QGraphicsPathItem *moypath;
    QGraphicsPathItem *tempath;
    QGraphicsPathItem *ypath;
    QGraphicsPathItem *yqpath;
    QPainterPath *pmoy;
    QPainterPath *line_temp;
    QPainterPath *yline;
    QPainterPath *yqline;
    int m_timereset;
    int temperature;
    int *moytab;
    int moyenne;
    int m_dep_x;
    int m_olx;
    int m_oldy;
    bool m_stop;


    struct Settings {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;

    };
        QSerialPort *serial;
        void initActionsConnections();
        char buffer[6]={'0','0','0','0','0','0'};
>>>>>>> a0a1bff0564b3ae3d26e3fb633296c1a792d51c0

};

#endif // MAINWINDOW_H
