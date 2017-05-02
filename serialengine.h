#ifndef SERIALENGINE_H
#define SERIALENGINE_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QTimer>

class serialengine : public QObject {
   Q_OBJECT
public:
    serialengine();
   ~serialengine();
signals:

public slots:
    int get_temp();
    void set_speed(const int value);
    void set_rand(const int value);
    void set_sername(const QString name);
    void set_baudrate(const int value);
    void set_parity(const int value);
    void set_stopBits(const int value);

    void set_databits(const int value);

    void write(QString str);
    void start();
    void stop();
    void openSerialPort();
    void closeSerialPort();
    void readData();
    void set_flowcontrol(const int value);

private:
    QSerialPort *serial;
    QTimer *reader;
    int m_speed_update;
    int temperature;
    int m_rand;
    char *buffer;
    int m_stopbits;
    QString m_ser_name;
    int m_baudrate;
    int m_databits;
    int m_parity;
    int m_flowcontrol;


};

#endif // SERIALENGINE_H
