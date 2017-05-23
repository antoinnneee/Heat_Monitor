#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtCore/QtGlobal>
#include <QMainWindow>
#include <QGraphicsView>
#include <QtSerialPort/QSerialPort>
#include <QPainterPath>

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

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
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

};

#endif // MAINWINDOW_H
