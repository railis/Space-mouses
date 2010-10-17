#include "mouse.h"
#include "object.h"
#include "window.h"

#include <QtGui>

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    Window window;
    window.show();
    return a.exec();
}

