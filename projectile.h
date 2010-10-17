#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "object.h"

class Projectile : public Object
{
public:
    Projectile();
    bool isExplosive();
    bool isHittable();
    void control();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    int lifetimer;
};

//__________________________________________________Cannon
class ProjectileCannon : public Projectile
{
public:
    ProjectileCannon();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
};

//__________________________________________________Missle
class ProjectileMissle : public Projectile
{
public:
    ProjectileMissle();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
};

#endif // PROJECTILE_H
