#include "mouse.h"
#include "window.h"
#include "explosion.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

#include <math.h>

static double TwoPi = 6.3031;

static qreal normalizeAngle(qreal angle)
{
    while (angle < 0)
        angle += TwoPi;
    while (angle > TwoPi)
        angle -= TwoPi;
    return angle;
}

Mouse::Mouse()
    : angle(0), speed(0), mouseEyeDirection(0),
    color(qrand() % 256, qrand() % 256, qrand() % 256), pulse(3)
{
    setRotation(qrand() % (360 * 16));
    health=100;
    armor=100;
}

QRectF Mouse::boundingRect() const
{
    qreal adjust = 0.5;
    return QRectF(-18 - adjust, -22 - adjust,
                  36 + adjust, 60 + adjust);
}

QPainterPath Mouse::shape() const
{
    QPainterPath path;
    if (armor > 0)
    {
        path.addRect(-30,-30,60,60);
        return path;
    } else {
        path.addRect(-10, -20, 20, 40);
        return path;
    }
}

void Mouse::hit(int damage)
{
    if (armor > 0)
    {
        armor -= damage;
    } else {
        health -= damage;
    }
}

void Mouse::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Body
    painter->setBrush(color);
    painter->drawEllipse(-10, -20, 20, 40);

    // Eyes
    painter->setBrush(Qt::white);
    if ( health > 40) // normalne oczy zdrowej myszy
    {
        painter->drawEllipse(-10, -17, 8, 8);
        painter->drawEllipse(2, -17, 8, 8);
    }
    else // pulsujace oczy
    {   pulse += 0.25;
        qreal oko = 4*sin(pulse);
        painter->drawEllipse(-13-oko, -19-oko, 12+oko, 12+oko);
        painter->drawEllipse(2-oko, -19-oko, 12+oko, 12+oko);
    }

    // Nose
    painter->setBrush(Qt::black);
    painter->drawEllipse(QRectF(-2, -22, 4, 4));

    // life left
    if  ( health > 60) painter->setBrush(Qt::green);
    else if ( health <= 60 && health > 40) painter->setBrush(Qt::yellow);
    else painter->setBrush(Qt::red);
    painter->drawRect(21, 21, 5, -(health/2.5));

    // armor barier
    if ( armor > 0)
    {
        painter->setPen(Qt::NoPen);
        QRadialGradient *grad = new QRadialGradient(0,0,40);
        int vanish = armor; // zanik barw i kanau alfa, od 255 do 0
        grad->setColorAt(0,QColor(255,vanish,0,vanish));
        grad->setColorAt(1,QColor(0,0,0,0));
        QBrush brush(*grad);
        painter->setBrush(brush);
        painter->drawEllipse(-30,-30,60,60);
    }

    // Pupils
    painter->drawEllipse(QRectF(-8.0 + mouseEyeDirection, -17, 4, 4));
    painter->drawEllipse(QRectF(4.0 + mouseEyeDirection, -17, 4, 4));

    // Ears
    painter->setBrush(Qt::darkYellow);
    painter->drawEllipse(-17, -12, 16, 16);
    painter->drawEllipse(1, -12, 16, 16);

    // Tail
    QPainterPath path(QPointF(0, 20));
    path.cubicTo(-5, 22, -5, 22, 0, 25);
    path.cubicTo(5, 27, 5, 32, 0, 30);
    path.cubicTo(-5, 32, -5, 42, 0, 35);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);
}

void Mouse::step()
{
    if(health<=0) // mysz umiera
    {
        Explosion* expl = new Explosion(50 + rand()%50);
        expl->setPos(this->scenePos());                            // pozycja playera we wspolrzednych sceny
        Window::spawn.enqueue(expl);
        Window::kill.enqueue(this);
    }

    // Don't move too far away
    QLineF lineToCenter(QPointF(0, 0), mapFromScene(0, 0));
    if (lineToCenter.length() > 1000) {
        qreal angleToCenter = ::acos(lineToCenter.dx() / lineToCenter.length());
        if (lineToCenter.dy() < 0)
            angleToCenter = TwoPi - angleToCenter;
        angleToCenter = normalizeAngle((Pi - angleToCenter) + Pi / 2);

        if (angleToCenter < Pi && angleToCenter > Pi / 4) {
            // Rotate left
            angle += (angle < -Pi / 2) ? 0.25 : -0.25;
        } else if (angleToCenter >= Pi && angleToCenter < (Pi + Pi / 2 + Pi / 4)) {
            // Rotate right
            angle += (angle < Pi / 2) ? 0.25 : -0.25;
        }
    } else if (::sin(angle) < 0) {
        angle += 0.25;
    } else if (::sin(angle) > 0) {
        angle -= 0.25;
    }


    // Try not to crash with any other mice
    QList<QGraphicsItem *> dangerMice = scene()->items(QPolygonF()
                                                       << mapToScene(0, 0)
                                                       << mapToScene(-30, -50)
                                                       << mapToScene(30, -50));
    foreach (QGraphicsItem *item, dangerMice) {
        if (item == this)
            continue;

        QLineF lineToMouse(QPointF(0, 0), mapFromItem(item, 0, 0));
        qreal angleToMouse = ::acos(lineToMouse.dx() / lineToMouse.length());
        if (lineToMouse.dy() < 0)
            angleToMouse = TwoPi - angleToMouse;
        angleToMouse = normalizeAngle((Pi - angleToMouse) + Pi / 2);

        if (angleToMouse >= 0 && angleToMouse < Pi / 2) {
            // Rotate right
            angle += 0.5;
        } else if (angleToMouse <= TwoPi && angleToMouse > (TwoPi - Pi / 2)) {
            // Rotate left
            angle -= 0.5;
        }
    }

    // Add some random movement
    if (dangerMice.size() > 1 && (qrand() % 10) == 0)
    {
        if (qrand() % 1)
            angle += (qrand() % 100) / 500.0;
        else
            angle -= (qrand() % 100) / 500.0;
    }

    speed += (-50 + qrand() % 100) / 100.0;
    qreal dx = ::sin(angle) * 10;
    mouseEyeDirection = (qAbs(dx / 5) < 1) ? 0 : dx / 5;
    setRotation(rotation() + dx);
    setPos(mapToParent(0, -(3 + sin(speed) * 3)));
}

