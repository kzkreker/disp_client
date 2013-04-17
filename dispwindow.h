#ifndef DISPWINDOW_H
#define DISPWINDOW_H

#include <QMainWindow>
#include <marble/MarbleWidget.h>
#include <marble/MarbleMap.h>
#include <marble/MarbleModel.h>
#include <marble/GeoPainter.h>
#include <marble/LayerInterface.h>

#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtGui/QApplication>
#include <QtGui/QKeyEvent>
#include <QDebug>

using namespace Marble;

namespace Ui {
class dispWindow;
}

class dispWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit dispWindow(QWidget *parent = 0);
    ~dispWindow();
    
private:
    Ui::dispWindow *ui;
};

#endif // DISPWINDOW_H
