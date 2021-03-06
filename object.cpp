#include "object.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>


Object::Object()
{
    radius = 15;

    angle = 0;
    speed = 0;
    slide = 0;
    rot = 0;

    friction = 0.9925;
    slidefriction = 0.9850;
    rotfriction = 0.9800;

    acc = 0.15;
    slideacc = 0.08;
    brake = 0.10;
    rotacc = 0.5;
}

QRectF Object::boundingRect() const
{
    return QRectF(-10,-10,20,20);
}

bool Object::isExplosive()
{
    return false;
}

bool Object::isHittable()
{
    return true;
}

void Object::hit(int damage)
{
    ;
}

void Object::control()
{
    ;
}

void Object::physics()
{
    if((speed >= 0.025) || (speed <= -0.025))
    {
        speed = speed * friction;
    }
    else
    {
        speed = 0;
    }
    if((rot >= 0.025) || (rot <= -0.025))
    {
        rot = rot * rotfriction;
    }
    else
    {
        rot = 0;
    }
    if(rot!=0)
    {
        speed += rot*slide*0.01;
        slide -= rot*speed*0.01;
    }
    if((slide >= 0.025) || (slide <= -0.025))
    {
        slide = slide * slidefriction;
    }
    else
    {
        slide = 0;
    }
}

void Object::step()
{
    rotate(-rot);
    angle-=rot;
    if(angle<0) angle+=360;
    if(angle>360) angle-=360;
    translate(0,-speed);
    translate(-slide,0);
}

void Object::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(Qt::gray);
    painter->drawRect(-10, -10, 20, 20);
}
