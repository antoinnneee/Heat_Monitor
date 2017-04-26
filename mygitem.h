#ifndef MYGITEM_H
#define MYGITEM_H


#include <QColor>
#include <QGraphicsItem>

class myGitem : public QGraphicsItem
{
public:
    myGitem(const QColor &color, int x, int y);


    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

private:
    int x;
    int y;
    QColor color;
    QVector<QPointF> stuff;

};

#endif // MYGITEM_H
