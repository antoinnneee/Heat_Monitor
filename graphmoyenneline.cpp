#include "graphmoyenneline.h"
#include <QColor>
#include <QGraphicsItem>

graphmoyenneLine::graphmoyenneLine()
{

}

graphmoyenneLine::graphmoyenneLine(int x0, int y0, int x1, int y1, const QColor &color)
{
    this->x0 = x0;
    this->x1 = x1;
    this->y0 = y0;
    this->y1 = y1;
    this->color = color;

}
