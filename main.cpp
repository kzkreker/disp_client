#include <QApplication>
#include "dispwindow.h"
#include "client.h"

int main(int argc, char *argv[])
{
     QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
     QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
     QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

    QApplication a(argc, argv);
    dispWindow w;
    w.show();

    return a.exec();
}
