#include "dispwindow.h"
#include "ui_dispwindow.h"



dispWindow::dispWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::dispWindow)
{
    ui->setupUi(this);
}

dispWindow::~dispWindow()
{
    delete ui;
}
