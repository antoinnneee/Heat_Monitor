#ifndef GRAPHMOYENNELINE_H
#define GRAPHMOYENNELINE_H

#include <QColor>
#include <QGraphicsLineItem>
#include <QtGui>

class graphmoyenneLine : public QGraphicsLineItem
{
public:
    graphmoyenneLine();
    graphmoyenneLine( int x0, int y0, int x1, int y1, const QColor &color);
    void paintEvent(QPaintEvent *) {
//                QPainter p(this);

//                // Draw boring background
//                p.setPen(Qt::NoPen);
//                p.setBrush(color);
//                p.drawLine(x0, y0, x1, y1);
////                p.setWidth(10);
//                p.setPen(inverter);
            }
private:
    int x1;
    int y1;
    int x0;
    int y0;
    QColor color;


};

#endif // GRAPHMOYENNELINE_H
