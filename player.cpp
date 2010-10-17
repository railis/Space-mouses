#include "player.h"
#include "window.h"
#include "projectile.h"

Player::Player()
{
    angle = 0;
    speed = 0;
    slide = 0;
    rot = 0;

    angle = 0;
    speed = 0;
    slide = 0;
    rot = 0;

    friction = 0.9925;
    slidefriction = 0.9850;
    rotfriction = 0.9750;

    acc = 0.15;
    slideacc = 0.12;
    brake = 0.10;
    rotacc = 0.4;

    weapon1barrel=0;
    weapon1state=0;
    weapon2state=0;
}

bool Player::isHittable()
{
    return false;
}

void Player::control()
{
    if(Window::Pressed_forward)
    {
        speed = speed + acc;
    }
    if(Window::Pressed_brake)
    {
        speed = speed - brake;
    }
    if(Window::Pressed_slideLeft)
    {
        slide = slide + slideacc;
    }
    if(Window::Pressed_slideRight)
    {
        slide = slide - slideacc;
    }
    if(Window::Pressed_rotateLeft)
    {
        rot = rot + rotacc;
    }
    if(Window::Pressed_rotateRight)
    {
        rot = rot - rotacc;
    }
    if(Window::Pressed_shoot)
    {
        if(weapon1state==0)
        {
            weapon1state=0;
            weapon1fire();
        }
    }
    if(Window::Pressed_canon)
    {
        if(weapon2state==0)
        {
            weapon2state=20;
            weapon2fire();
        }
    }
    if(Window::Pressed_missle)
    {
        if(weapon3state==0)
        {
            weapon3state=15;
            weapon3fire();
        }
    }

    if(weapon3state>0)
    {
        weapon3state--;
    }

    if(weapon1state>0)
    {
        weapon1state--;
    }

    if(weapon2state>0)
    {
        weapon2state--;
    }
}


void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPixmap ship(":/images/statek1.svg");
    painter->drawPixmap(QRect(-52, -40, 104, 60),ship);
    painter->setPen(Qt::NoPen);
    QRadialGradient *grad = new QRadialGradient(0,0,55);
    grad->setColorAt(0,QColor(0,0,0,0));
    grad->setColorAt(1,QColor(80,30,255,150));
    QBrush brush(*grad);
    painter->setBrush(brush);
    painter->drawEllipse(-55,-55,110,110);
}

void Player::weapon1fire()
{
    Projectile *pr = new Projectile;
    pr->setPos(this->scenePos());                            // pozycja playera we wspolrzednych sceny
    pr->rotate(this->angle - 4.0 + 8.0*rand()/RAND_MAX);
    pr->translate(10,-42);
    if(weapon1barrel==0)
    {
        weapon1barrel++;
    }
    else if(weapon1barrel==1)
    {
        weapon1barrel++;
        pr->translate(2,3);
    }
    else if(weapon1barrel==2)
    {
        weapon1barrel++;
        pr->translate(4,5);
    }
    else if(weapon1barrel==3)
    {
        weapon1barrel=0;
        pr->translate(6,7);
        weapon1state=10;
    }
    pr->speed = this->speed + 10.0 + 2.0*rand()/RAND_MAX;;
    pr->slide = this->slide - 0.1 + 0.3*rand()/RAND_MAX;
    Window::spawn.enqueue(pr);
}

void Player::weapon2fire()
{
    ProjectileCannon *pr = new ProjectileCannon;
    pr->setPos(this->scenePos());                            // pozycja playera we wspolrzednych sceny
    pr->rotate(this->angle - 0.5 + 1.0*rand()/RAND_MAX);
    pr->translate(-12,-58);
    pr->speed = this->speed + 18.0;
    pr->slide = this->slide - 0.08;
    Window::spawn.enqueue(pr);
}

void Player::weapon3fire()
{
    ProjectileMissle *pr = new ProjectileMissle;
    pr->setPos(this->scenePos());                            // pozycja playera we wspolrzednych sceny
    pr->rotate(this->angle - 0.5 + 1.0*rand()/RAND_MAX);
    pr->translate(-12,-58);
    pr->speed = this->speed + 39.0;
    pr->slide = this->slide - 0.08;
    Window::spawn.enqueue(pr);
}
