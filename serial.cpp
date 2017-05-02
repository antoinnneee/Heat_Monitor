
#include <QtSerialPort/QSerialPort>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QDebug>
#include <QScrollBar>
#include <QPen>
#include <QtConcurrent>
#include <QPainterPath>
#include <QThread>
#include "serialengine.h"

void serialengine::readData()
{

        if (serial->isOpen())
        {
            serial->write("$");
            serial->readLine(buffer, 5);
            QString donnees=buffer;

            while(donnees.size()<5)
            {

                serial->readLine(buffer, 5);
                donnees=buffer;
                qDebug()<< donnees << " size: " << donnees.size();
            }
            if(donnees.toInt()!=0)
                temperature=donnees.toInt() ;

        }
        else
        {
            temperature = qrand() % m_rand;
        }

}


void serialengine::openSerialPort()
{

    serial->setPortName(m_ser_name);
    serial->setBaudRate(m_baudrate);
    serial->setDataBits(QSerialPort::DataBits(m_databits));
    serial->setParity(QSerialPort::Parity(m_parity));
    serial->setStopBits(QSerialPort::StopBits(m_stopbits));
    serial->setFlowControl(QSerialPort::FlowControl(m_flowcontrol));
    if(!serial->open(QIODevice::ReadWrite))
    {
        qDebug()<<"erreur";
    }

}

void serialengine::write(QString str)
{
    serial->write((const char*)str.data());
}

