#include "serialengine.h"
#include <QtSerialPort/QSerialPort>
#include <QObject>

serialengine::serialengine()
{
    serial = new QSerialPort;

    openSerialPort();
    m_flowcontrol = 0;
    m_rand = 200;
    m_parity = 0;
    m_speed_update = 100;
    m_stopbits = 1;
    temperature = 0;
    m_ser_name = "COM4";
    m_databits = 8;
    m_baudrate = 115200;
    buffer = (char*)malloc(sizeof(char) * 5);
    int i = 5;
    while(i--)
    {
        buffer[i] = '\0';
    }

    reader = new QTimer;
//    reader->setSingleShot(false);

    connect(reader, SIGNAL(timeout()), this, SLOT(readData()));


}
serialengine::~serialengine()
{

}

void serialengine::start()
{
    reader->start(m_speed_update);
}
void serialengine::stop()
{
    reader->stop();
}

int serialengine::get_temp()
{
    return temperature;
}

void serialengine::set_speed(const int value)
{
    stop();
    m_speed_update = value;
    start();
}

void serialengine::set_rand(const int value)
{
    m_rand = value;
}

void serialengine::set_sername(const QString name)
{
    m_ser_name = name;
}

void serialengine::set_baudrate(const int value)
{
    m_baudrate = value;
}

void serialengine::set_databits(const int value)
{
    m_databits = value;
}

void serialengine::set_stopBits(const int value)
{
    m_databits = value;
}

void serialengine::set_parity(const int value)
{
    m_parity = value;
}

void serialengine::set_flowcontrol(const int value)
{
    m_flowcontrol = value;
}

void serialengine::closeSerialPort()
{
    if(serial->isOpen())
        serial->close();
}
