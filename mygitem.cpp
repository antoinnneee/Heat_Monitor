#include "mygitem.h"

#include <QtWidgets>


myGitem::myGitem(const QColor &color, int x, int y)
{
    this->x = x;
    this->y = y;
    this->color = color;
    setZValue((x + y) % 2);

    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

QRectF myGitem::boundingRect() const
{
    return QRectF(0, 0, 20, 20);
}




void myGitem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor = (option->state & QStyle::State_Selected) ? color.blue() : color;
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.light(125);

    QBrush b = painter->brush();
    painter->setBrush(fillColor);
    painter->drawRect(0, 0, 5, 5);
    painter->setBrush(b);
    return;

}
