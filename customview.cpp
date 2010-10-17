#include "customview.h"
#include "window.h"

CustomView::CustomView(QWidget *parent )  : QGraphicsView(parent)
{
    x=0;
    y=0;
}

void CustomView::mouseMoveEvent ( QMouseEvent * event )
{
    x = event->x();
    y = event->y();
    event->accept();
}

void CustomView::mousePressEvent ( QMouseEvent * event )
{
    Window::Pressed_shoot=true;
    event->accept();
}

void CustomView::mouseReleaseEvent ( QMouseEvent * event )
{
    Window::Pressed_shoot=false;
    event->accept();
}
