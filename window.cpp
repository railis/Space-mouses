#include "window.h"
#include "explosion.h"

#include <iostream>

// poza deklaracj¹ zmiennych statycznych w pliku nag³ówkowym, potrzebne jest ich zdeklarowanie i zainicjalizowanie
bool Window::Pressed_forward = FALSE;
bool Window::Pressed_slideLeft = FALSE;
bool Window::Pressed_slideRight = FALSE;
bool Window::Pressed_rotateLeft = FALSE;
bool Window::Pressed_rotateRight = FALSE;
bool Window::Pressed_brake = FALSE;
bool Window::Pressed_shoot = FALSE;
bool Window::Pressed_canon = FALSE;
bool Window::Pressed_missle = FALSE;
QQueue<Object*> Window::spawn;
QQueue<Object*> Window::kill;

Window::Window(QWidget *parent) : QMainWindow(parent)
{ 
    setupUi(this); // wypelnienie interfejsu danymi z formularza graficznego
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    connect(&maintimer, SIGNAL(timeout()), this, SLOT(MainClockTick())); // polaczenie sygnalu ze slotem

    actions.insert( Qt::Key_W, Forward );
    actions.insert( Qt::Key_A, SlideLeft );
    actions.insert( Qt::Key_D, SlideRight );
    actions.insert( Qt::Key_E, Shoot );
    actions.insert( Qt::Key_S, Brake );
    actions.insert( Qt::Key_F, Missle);
    actions.insert( Qt::Key_Q, Canon);

    NewGame();
}


void Window::NewGame()
{
    scene.setSceneRect(-16000, -16000, 32000, 32000);
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    for (int i = 0; i < MouseCount; ++i)
    {
        Mouse *mouse = new Mouse;
        mouse->setPos(::sin((i * 6.28) / MouseCount) * 250,
                      ::cos((i * 6.28) / MouseCount) * 250);
        scene.addItem(mouse);
    }

    std::cout<<"cout";
    std::cerr<<"cerr";

    scene.installEventFilter(this); // instalacja w scenie filtru przechwytuj¹cego eventy
    //view->setBackgroundBrush(QPixmap(":/images/stars.jpg"));
    view->setBackgroundBrush(QBrush(Qt::black));
    view->setScene(&scene);
    maintimer.start(30);

    Object *justobject = new Object;            //test, zwykly obiekt, kwadracik
    justobject->setPos(40,32);
    scene.addItem(justobject);                  //test

    player = new Player();
    player->setPos(-40,-100);
    scene.addItem(player);
    processed = true;
}

void Window::MainClockTick()
{
    if(processed)
    {
        processed = false;
        ProcessPlayer();
        items = scene.items();
        foreach(QGraphicsItem *item, items) // prawie jak w Javie - konstrukcja dostarczona przez QT
        {
            Object *object = dynamic_cast<Object *> (item); // rzutowanie QGraphicsItem na Object
            object->control();
            object->physics();
            object->step();

            if(object->isExplosive())
            {
                foreach(QGraphicsItem *item2, items)
                {
                    Object *target = dynamic_cast<Object *> (item2);
                    if(target->isHittable())
                    {
                        if(object->collidesWithItem(target))
                        {
                            target->hit(object->attack);
                            Explosion* expl = new Explosion(25);
                            expl->setPos(object->scenePos());
                            Window::spawn.enqueue(expl);
                            Window::kill.enqueue(object);
                        }
                    }
                }// foreach target
            }// if explosive
        } //foreach object

        view->centerOn(player);
        scene.update(); // przerysowanie sceny

        //debug->textCursor().insertText(QString::number(spawn.size())); // test
        while(!spawn.empty())
        {
            scene.addItem(spawn.dequeue());
        }

        while(!kill.empty())
        {
            Object *dead = kill.dequeue();
            if(dead != NULL)
            {
                scene.removeItem(dead);
                delete dead;
            }
        }
        processed = true;
    }//if
}

void Window::ProcessPlayer()
{
    double centerx = view->width()/2.0;
    double centery = view->height()/2.0;
    double linex = (view->x - centerx);
    double liney = (view->y - centery);

    double arc = atan2(linex,-liney);
    arc = arc * 180.0 / Pi;
    if(arc<0) arc+=360;
    double diff = player->angle - arc;
    if(diff<-180) diff+=360;
    if(diff>180) diff-=360;

    if(diff>-1 && diff<1)
    {
        Window::Pressed_rotateLeft = FALSE;
        Window::Pressed_rotateRight = FALSE;
        player->rot*=0.920;
    }
    else if(diff>1 && player->rot>0 && player->rot/diff>0.185)
    {
        Window::Pressed_rotateLeft = FALSE;
        Window::Pressed_rotateRight = TRUE;
    }
    else if(diff<-1 && player->rot<0 && player->rot/diff>0.185)
    {
        Window::Pressed_rotateRight = FALSE;
        Window::Pressed_rotateLeft = TRUE;
    }
    else if(diff>0)
    {
        Window::Pressed_rotateRight = FALSE;
        Window::Pressed_rotateLeft = TRUE;
    }
    else
    {
        Window::Pressed_rotateLeft = FALSE;
        Window::Pressed_rotateRight = TRUE;
    }
}

void Window::keyPressEvent( QKeyEvent *event )
{
    if ( event->isAutoRepeat() || !actions.contains( event->key() ) )
    {   // autorepeat to wirtualne przycisniecia kiedy trzymamy klawisz
        event->ignore();
        return;
    }
    Action action = actions[event->key()];
    switch ( action )
    {
    case Forward:
        Pressed_forward = TRUE;
        break;
    case SlideLeft:
        Pressed_slideLeft = TRUE;
        break;
    case SlideRight:
        Pressed_slideRight = TRUE;
        break;
    case RotateLeft:
        Pressed_rotateLeft = TRUE;
        break;
    case RotateRight:
        Pressed_rotateRight = TRUE;
        break;
    case Shoot:
        Pressed_shoot = TRUE;
        break;
    case Canon:
        Pressed_canon = TRUE;
        break;
    case Missle:
        Pressed_missle = TRUE;
        break;
    case Brake:
        Pressed_brake= TRUE;
        break;
    default:
        event->ignore();
        return;
    }
    event->accept();
}


void Window::keyReleaseEvent( QKeyEvent *event )
{
    if ( event->isAutoRepeat() || !actions.contains( event->key() ) )
    {
        event->ignore();
        return;
    }
    Action action = actions[event->key()];
    switch ( action )
    {
    case Forward:
        Pressed_forward = FALSE;
        break;
    case SlideLeft:
        Pressed_slideLeft = FALSE;
        break;
    case SlideRight:
        Pressed_slideRight = FALSE;
        break;
    case RotateLeft:
        Pressed_rotateLeft = FALSE;
        break;
    case RotateRight:
        Pressed_rotateRight = FALSE;
        break;
    case Shoot:
        Pressed_shoot = FALSE;
        break;
    case Canon:
        Pressed_canon = FALSE;
        break;
    case Missle:
        Pressed_missle = FALSE;
        break;
    case Brake:
        Pressed_brake= FALSE;
        break;
    default:
        event->ignore();
        return;
    }
    event->accept();
}

void Window::wheelEvent( QWheelEvent *event )
{
    float scale = 1.0 + event->delta()*0.001;
    view->scale(scale,scale);
    event->accept();
}

// niestety QWheelEvent i QGraphicsSceneWheelEvent to
// sporo ró¿ni¹ce siê zdarzenia - ogolnie jest to sprytne,
// ale nam przeszkadza
void Window::wheelEvent( QGraphicsSceneWheelEvent *event )
{
    float scale = 1.0 + event->delta()*0.001;
    view->scale(scale,scale);
    event->accept();
}

// robimy filtr, który ka¿de zdarzenie u¿ycia rolki myszy
// (w obiekcie gdzie zainstalujemy ten filtr) przekieruje na Window
bool Window::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type()==QEvent::GraphicsSceneWheel)
    {
        this->wheelEvent(static_cast<QGraphicsSceneWheelEvent*>(event));
        return true;
    }
    else
    {
        return false;
    }
}
