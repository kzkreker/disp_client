#include <QApplication>
#include "dispwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dispWindow w;
    w.show();
    
    return a.exec();
}
