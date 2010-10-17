#ifndef MOUSE_H
#define MOUSE_H

#include <QGraphicsItem>

#include "object.h"

class Mouse : public Object
{
public:
    Mouse();
    QRectF boundingRect() const;
    void step();
    QPainterPath shape() const;
    void hit(int damage);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    qreal angle;
    qreal speed;
    qreal mouseEyeDirection;
    QColor color;
    qreal pulse; // do zmiany rozmiaru galki ocznej
};


#endif
