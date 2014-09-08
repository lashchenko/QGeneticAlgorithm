#include <QApplication>
#include "gwidget.h"

#include <cmath>

int main(int argc, char *argv[])
{
    srand(time(NULL));

    QApplication a(argc, argv);
    GWidget w;
    w.setPalette(QPalette(Qt::gray));
    w.show();
    return a.exec();
}
