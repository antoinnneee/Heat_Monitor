#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtCore/QtGlobal>
#include <QMainWindow>
#include <QGraphicsView>
#include <QtSerialPort/QSerialPort>
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
    void on_pushButton_clicked();
    void myupdate();
    void on_pushButton_2_clicked();
    void openSerialPort();
    //void writeData(const QByteArray &data);
    void closeSerialPort();
   // void about(){};

    void readData();
   void handleError(QSerialPort::SerialPortError error);

   private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    int m_olx;
    int m_oldy;

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
        int temperature;
};

#endif // MAINWINDOW_H
