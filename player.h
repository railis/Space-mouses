#ifndef PLAYER_H
#define PLAYER_H

#include <QtGui>

#include "object.h"

class Player : public Object
{
public:
    Player();
    bool isHittable();
    void control();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    int weapon1state;
    int weapon1barrel;
    int weapon2state;
    int weapon3state;
    void weapon1fire();
    void weapon2fire();
    void weapon3fire();

    
    
};

#endif // PLAYER_H
