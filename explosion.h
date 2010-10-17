#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "object.h"

class Explosion : public Object
{
public:
    Explosion();
    Explosion(int size);
    bool isHittable();

    int size;
    int lifetimer;
    int lifemax;

    void control();
    void physics();
    void step();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
};

#endif // EXPLOSION_H
