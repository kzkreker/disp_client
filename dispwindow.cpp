#include "dispwindow.h"
#include "ui_dispwindow.h"


dispWindow::dispWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::dispWindow)
{
    ui->setupUi(this);

    //рисуем на слоях глобуса предварительно создав слой
    layer = new MyPaintLayer(ui->MarbleWidget);
    ui->MarbleWidget->addLayer(layer);
    ui->MarbleWidget->installEventFilter(layer);

    //отсечка прорисовки
    seconds.setInterval(1000);

    //перерисовка слоя по отсечке таймера
    QObject::connect(&seconds, SIGNAL(timeout()), ui->MarbleWidget, SLOT(update()));

    //обновление маркеров по отсечке таймера
    QObject::connect(&x, SIGNAL(newActiveGPSResiv(gpsdata)), layer, SLOT(newActiveGPSResiv(gpsdata)));
    QObject::connect(&seconds, SIGNAL(timeout()), &x , SLOT(resivActiveGPS()));
    QObject::connect(&seconds, SIGNAL(timeout()), this, SLOT(activeCarQTableUpdate()));

    //настройка элемента выбора даты
    ui->activDataTime->setMinimumDate(QDate::currentDate().addYears(-1));
    ui->activDataTime->setMaximumDate(QDate::currentDate().addYears(5));
    ui->activDataTime->setMinimumTime(QTime::currentTime().addSecs(-3600));
    ui->activDataTime->setMaximumTime(QTime::currentTime().addSecs(3600));
    ui->activDataTime->setDateTime(QDateTime::currentDateTime().addYears(-1));
    ui->activDataTime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    connect( ui->activDataTime,SIGNAL(dateTimeChanged(QDateTime)),&x,SLOT(setDate(QDateTime)));
    x.queruDate.setDate(QDate::currentDate().addYears(-1));
    x.queruDate.setTime(QTime::currentTime());

    //Настройка таблицы с активными машинами
    model = new QStandardItemModel(1,4,this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Авто")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Номер")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Экипаж")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Активность")));

    ui->activeCarQTable->setModel(model);
    seconds.start();

}

void dispWindow::activeCarQTableUpdate()
{
    int i=0;

     model = new QStandardItemModel(x.gpssender.gpslist.length(),3,this);
    i=0;
    QStandardItem *firstRow;

    foreach(x.gpssender.gpsitem, x.gpssender.gpslist)
     {

     firstRow= new QStandardItem(x.gpssender.gpsitem.car_name); model->setItem(i,0,firstRow);
     firstRow= new QStandardItem(x.gpssender.gpsitem.car_number); model->setItem(i,1,firstRow);
     firstRow= new QStandardItem(QString::number(x.gpssender.gpsitem.crew_id)); model->setItem(i,2,firstRow);
     if (x.gpssender.gpsitem.now_online==true){
         firstRow= new QStandardItem("Активен"); model->setItem(i,3,firstRow);
     } else {
          firstRow= new QStandardItem("Не активен"); model->setItem(i,3,firstRow);
     }

     i++;
    }

    ui->activeCarQTable->setModel(model);

}

dispWindow::~dispWindow()
{
    delete ui;
}

