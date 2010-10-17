#include "projectile.h"
#include "window.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QPen>
#include <QColor>


Projectile::Projectile()
{
    angle = 0;
    speed = 0;
    slide = 0;
    rot = 0;
    attack = 3;

    friction = 0.9975;
    slidefriction = 0.96;
    rotfriction = 0.98;

    lifetimer = 100;
}

QRectF Projectile::boundingRect() const
{
    return QRectF(-1,0,1,10);
}

bool Projectile::isExplosive()
{
    return true;
}

bool Projectile::isHittable()
{
    return false;
}

void Projectile::control()
{
    lifetimer--;
    if(!lifetimer)
    {
        Window::kill.enqueue(this);
    }
}
void Projectile::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen pen(QColor(255,50,0), 1.0, Qt::SolidLine, Qt::RoundCap);
    painter->setPen(pen);
    painter->drawLine(0, 10, 0, 0);
    QPen pen1(QColor(255,225,175), 0);
    painter->setPen(pen1);
    painter->drawLine(0, 8, 0, 0);
}

//__________________________________________________Cannon
ProjectileCannon::ProjectileCannon()
        {
            angle = 0;
            speed = 0;
            slide = 0;
            rot = 0;
            attack = 10;

            friction = 0.9975;
            slidefriction = 0.96;
            rotfriction = 0.98;

            lifetimer = 175;
        }

void ProjectileCannon::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen pen(QColor(255,100,0), 2.5, Qt::SolidLine, Qt::RoundCap);
    painter->setPen(pen);
    painter->drawLine(0, 15, 0, 0);
    QPen pen1(QColor(255,200,125), 0);
    painter->setPen(pen1);
    painter->drawLine(0, 12, 0, 0);
}

//__________________________________________________Missle
ProjectileMissle::ProjectileMissle()
{
    angle = 0;
    speed = 0;
    slide = 0;
    rot = 0;
    attack = 20;

    friction = 0.9975;
    slidefriction = 0.96;
    rotfriction = 0.98;

    lifetimer = 200;
}

void ProjectileMissle::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(Qt::yellow);
    painter->drawEllipse(QRectF(0, 0, 18, 18));
}
