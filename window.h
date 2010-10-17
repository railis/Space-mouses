#ifndef WINDOW_H
#define WINDOW_H

#include <QtGui>
#include <QWheelEvent>

#include <cmath>

#include "ui_mainwindow.h"
#include "customview.h"
#include "mouse.h"
#include "object.h"
#include "player.h"

static const double Pi = 3.14159265358979323846264338327950288419717;

class Window : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT
public:
    explicit Window(QWidget *parent = 0);
    static bool Pressed_forward;
    static bool Pressed_slideLeft;
    static bool Pressed_slideRight;
    static bool Pressed_rotateLeft;
    static bool Pressed_rotateRight;
    static bool Pressed_brake;
    static bool Pressed_shoot;
    static bool Pressed_canon;
    static bool Pressed_missle;
    Player *player;
    static QQueue<Object*> spawn;  // kolejka obiektow do dodania
    static QQueue<Object*> kill;   // kolejka obiektow do usuniecia

protected:
     bool eventFilter(QObject *obj, QEvent *ev);

private:
    static const int MouseCount = 12;
    QList<QGraphicsItem *> items;
    QGraphicsScene scene;
    enum Action { Forward, SlideLeft, SlideRight, RotateLeft, RotateRight, Brake, Shoot, Canon, Missle };
    QMap<int,Action> actions;
    QTimer maintimer;
    bool processed;
    //qreal currentScale;
    void ProcessPlayer();

signals:

public slots:
    void keyPressEvent( QKeyEvent *event );
    void keyReleaseEvent( QKeyEvent *event );
    void wheelEvent( QWheelEvent *event);
    void wheelEvent( QGraphicsSceneWheelEvent *event);

private slots:
    void NewGame();
    void MainClockTick();
};

#endif // WINDOW_H
