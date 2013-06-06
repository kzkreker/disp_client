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

    //рисуем на слоях глобуса для отчетов предварительно создав слой
    layerTrack = new myPaintLayerTrack(ui->MarblReport);
    ui->MarblReport->addLayer(layerTrack);
    ui->MarblReport->installEventFilter(layerTrack);

    //отсечка прорисовки
    seconds.setInterval(1000);
    minets.setInterval(5000);

    //настройка элемента выбора даты текущего положения
    ui->activDataTime->setMinimumDate(QDate::currentDate().addYears(-1));
    ui->activDataTime->setMaximumDate(QDate::currentDate().addYears(5));
    ui->activDataTime->setMinimumTime(QTime::currentTime().addSecs(-3600));
    ui->activDataTime->setMaximumTime(QTime::currentTime().addSecs(3600));
    ui->activDataTime->setDateTime(QDateTime::currentDateTime().addSecs(-3600));
    ui->activDataTime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");

    //настройка элемента выбора даты архива
    ui->periodEnd->setMinimumDate(QDate::currentDate().addYears(-1));
    ui->periodEnd->setMaximumDate(QDate::currentDate().addYears(5));
    ui->periodEnd->setMinimumTime(QTime::currentTime().addSecs(-3600));
    ui->periodEnd->setMaximumTime(QTime::currentTime().addSecs(3600));
    ui->periodEnd->setDateTime(QDateTime::currentDateTime());
    ui->periodEnd->setDisplayFormat("yyyy-MM-dd hh:mm:ss");

    ui->periodStart->setMinimumDate(QDate::currentDate().addYears(-1));
    ui->periodStart->setMaximumDate(QDate::currentDate().addYears(5));
    ui->periodStart->setMinimumTime(QTime::currentTime().addSecs(-3600));
    ui->periodStart->setMaximumTime(QTime::currentTime().addSecs(3600));
    ui->periodStart->setDateTime(QDateTime::currentDateTime().addDays(-1));
    ui->periodStart->setDisplayFormat("yyyy-MM-dd hh:mm:ss");

    //настройка элемента выбора даты архива (второй блок) говнокод
    ui->periodSensorsEnd->setMinimumDate(QDate::currentDate().addYears(-1));
    ui->periodSensorsEnd->setMaximumDate(QDate::currentDate().addYears(5));
    ui->periodSensorsEnd->setMinimumTime(QTime::currentTime().addSecs(-3600));
    ui->periodSensorsEnd->setMaximumTime(QTime::currentTime().addSecs(3600));
    ui->periodSensorsEnd->setDateTime(QDateTime::currentDateTime());
    ui->periodSensorsEnd->setDisplayFormat("yyyy-MM-dd hh:mm:ss");

    ui->periodSensorsStart->setMinimumDate(QDate::currentDate().addYears(-1));
    ui->periodSensorsStart->setMaximumDate(QDate::currentDate().addYears(5));
    ui->periodSensorsStart->setMinimumTime(QTime::currentTime().addSecs(-3600));
    ui->periodSensorsStart->setMaximumTime(QTime::currentTime().addSecs(3600));
    ui->periodSensorsStart->setDateTime(QDateTime::currentDateTime().addDays(-1));
    ui->periodSensorsStart->setDisplayFormat("yyyy-MM-dd hh:mm:ss");


    //Установка дефолтной даты
    x.queruDate.setDate(QDate::currentDate().addYears(-1));
    x.queruDate.setTime(QTime::currentTime());

    //настройка комбо для выбора экипажа
    ui->nomerSelector->addItem("Выберите номер авто ");

    //Настройка таблицы с данными о таблице "car"
    carEditModel = new CarTableModel;
    ui->carEditQTable->setItemDelegate(new CarTableDelegate); // устанавливаем делегат  представлению
    ui->carEditQTable->setModel(carEditModel); // устанавливаем модель

    //сигнал для кнопки построения маршрута
    QObject::connect(ui->getTrackButton, SIGNAL(clicked()), this, SLOT(getGPSTrackData()));

    //сигнал обновления слоя с треком и таблицы с теком
    QObject::connect(&x, SIGNAL(newGPSTrackResiv(gpsdata)), layerTrack, SLOT(GPSTrackResiv(gpsdata)));
    QObject::connect(layerTrack, SIGNAL(TrackResiv()), ui->MarblReport, SLOT(update()));
    QObject::connect(layerTrack, SIGNAL(TrackResiv()), this, SLOT(gpsTrackUpdate()));

    //сигнал кнопки получения показаний датчиков
    QObject::connect(ui->getPicStatus, SIGNAL(clicked()), this, SLOT(getPicStatusData()));
   //сигнал получения списка показаний датчиков
    QObject::connect(&x, SIGNAL(newPicSensorsResiv()), this, SLOT(getPicStatusTable()));

    //обновление маркеров по отсечке таймера
    QObject::connect(&x, SIGNAL(newActiveGPSResiv(gpsdata)), layer, SLOT(newActiveGPSResiv(gpsdata)));
    QObject::connect(&seconds, SIGNAL(timeout()), &x , SLOT(resivActiveGPS()));
    QObject::connect(&minets, SIGNAL(timeout()), this, SLOT(activeCarQTableUpdate()));

    //слот который будет изменять данные для БД
    QObject::connect( ui->activDataTime,SIGNAL(dateTimeChanged(QDateTime)),&x,SLOT(setDate(QDateTime)));

    //перерисовка слоя по отсечке таймера
    QObject::connect(&seconds, SIGNAL(timeout()), ui->MarbleWidget, SLOT(update()));

    //запуск таймеров
    seconds.start();
    minets.start();
}

////////////////////////////////////////////////////////
///обновление полей таблицы активных машин
////////////////////////////////////////////////////////
void dispWindow::activeCarQTableUpdate()
{
    int i=0;

    //создаем модель
    model = new QStandardItemModel(x.gpssender.gpslist.length(),3,this);

    //Настройка таблицы с активными машинами
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Авто")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Номер")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Активность")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Экипаж")));


    QStandardItem *firstRow;

    //заполняем модель данными
    foreach(x.gpssender.gpsitem, x.gpssender.gpslist)
     {
         //заполнение по строкам
         firstRow= new QStandardItem(x.gpssender.gpsitem.car_name); model->setItem(i,0,firstRow);
         firstRow= new QStandardItem(x.gpssender.gpsitem.car_number); model->setItem(i,1,firstRow);
         firstRow= new QStandardItem(QString::number(x.gpssender.gpsitem.crew_id)); model->setItem(i,3,firstRow);

         //статус машины
         if (x.gpssender.gpsitem.now_online==true){
              firstRow= new QStandardItem("Активен"); model->setItem(i,2,firstRow);
         } else {
              firstRow= new QStandardItem("Не активен"); model->setItem(i,2,firstRow);
         }
              i++;
     }

    //применяем модель к таблице
    ui->activeCarQTable->setModel(model);
    //обновляем селектор номера
    nomerSelektorGet();
}

////////////////////////////////////////////////////////
///обновление полей таблиц показаний датчиков трека
////////////////////////////////////////////////////////
void dispWindow::getPicStatusTable()
{

    int i=0;

    //создаем модель
    model = new QStandardItemModel(x.gpssender.gpstracklist.length(),8,this);

    model->setHorizontalHeaderItem(0, new QStandardItem(QString("An0")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("An1")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("An2")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("An3")));
    model->setHorizontalHeaderItem(4, new QStandardItem(QString("An4")));
    model->setHorizontalHeaderItem(5, new QStandardItem(QString("An5")));
    model->setHorizontalHeaderItem(6, new QStandardItem(QString("An6")));
    model->setHorizontalHeaderItem(7, new QStandardItem(QString("Дата")));


    QStandardItem *firstRow;

    //заполняем строки таблицы
    foreach(x.gpssender.picitem, x.gpssender.piclist)
     {
        firstRow= new QStandardItem(x.gpssender.picitem.an[0]);       model->setItem(i,0,firstRow);
        firstRow= new QStandardItem(x.gpssender.picitem.an[1]);       model->setItem(i,1,firstRow);
        firstRow= new QStandardItem(x.gpssender.picitem.an[2]);       model->setItem(i,2,firstRow);
        firstRow= new QStandardItem(x.gpssender.picitem.an[3]);       model->setItem(i,3,firstRow);
        firstRow= new QStandardItem(x.gpssender.picitem.an[4]);       model->setItem(i,4,firstRow);
        firstRow= new QStandardItem(x.gpssender.picitem.an[5]);       model->setItem(i,5,firstRow);
        firstRow= new QStandardItem(x.gpssender.picitem.an[6]);       model->setItem(i,6,firstRow);
        firstRow= new QStandardItem(x.gpssender.picitem.datatime.toString("yyyy-MM-dd hh:mm:ss"));  model->setItem(i,7,firstRow);

        i++;
     }
    //применяем модель к таблице
    ui->picViewAn->setModel(model);

    i=0;

    //создаем модель
    model = new QStandardItemModel(x.gpssender.gpstracklist.length(),9,this);

    model->setHorizontalHeaderItem(0, new QStandardItem(QString("DG0")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("DG1")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("DG2")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("DG3")));
    model->setHorizontalHeaderItem(4, new QStandardItem(QString("DG4")));
    model->setHorizontalHeaderItem(5, new QStandardItem(QString("DG5")));
    model->setHorizontalHeaderItem(6, new QStandardItem(QString("DG6")));
    model->setHorizontalHeaderItem(7, new QStandardItem(QString("DG7")));
    model->setHorizontalHeaderItem(8, new QStandardItem(QString("Дата")));

    //заполняем строки таблицы
    foreach(x.gpssender.picitem, x.gpssender.piclist)
     {
        firstRow= new QStandardItem(x.gpssender.picitem.dg[0]);       model->setItem(i,0,firstRow);
        firstRow= new QStandardItem(x.gpssender.picitem.dg[1]);       model->setItem(i,1,firstRow);
        firstRow= new QStandardItem(x.gpssender.picitem.dg[2]);       model->setItem(i,2,firstRow);
        firstRow= new QStandardItem(x.gpssender.picitem.dg[3]);       model->setItem(i,3,firstRow);
        firstRow= new QStandardItem(x.gpssender.picitem.dg[4]);       model->setItem(i,4,firstRow);
        firstRow= new QStandardItem(x.gpssender.picitem.dg[5]);       model->setItem(i,5,firstRow);
        firstRow= new QStandardItem(x.gpssender.picitem.dg[6]);       model->setItem(i,6,firstRow);
        firstRow= new QStandardItem(x.gpssender.picitem.dg[7]);       model->setItem(i,7,firstRow);
        firstRow= new QStandardItem(x.gpssender.picitem.datatime.toString("yyyy-MM-dd hh:mm:ss"));  model->setItem(i,8,firstRow);

        i++;
     }
    //применяем модель к таблице
    ui->picViewDg->setModel(model);


    i=0;
    //создаем модель
    model = new QStandardItemModel(x.gpssender.gpstracklist.length(),3,this);

    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Потентометр")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Температура")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Дата")));

    //заполняем строки таблицы
    foreach(x.gpssender.picitem, x.gpssender.piclist)
     {
        firstRow= new QStandardItem(x.gpssender.picitem.pot);       model->setItem(i,0,firstRow);
        firstRow= new QStandardItem(x.gpssender.picitem.tmp);        model->setItem(i,1,firstRow);
        firstRow= new QStandardItem(x.gpssender.picitem.datatime.toString("yyyy-MM-dd hh:mm:ss"));  model->setItem(i,2,firstRow);
        i++;
     }
    //применяем модель к таблице
    ui->picViewTmpPot->setModel(model);

}

////////////////////////////////////////////////////////
///обновление полей таблицы GPS трека
////////////////////////////////////////////////////////
void dispWindow::gpsTrackUpdate()
{
    int i=0;

    //создаем модель
    model = new QStandardItemModel(x.gpssender.gpstracklist.length(),5,this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Широта")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Долгота")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Скортсть")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Азимут")));
    model->setHorizontalHeaderItem(4, new QStandardItem(QString("Дата")));

    QStandardItem *firstRow;

    //заполняем модель данными
    foreach(x.gpssender.gpstrackitem, x.gpssender.gpstracklist)
     {

         firstRow= new QStandardItem(QString::number(x.gpssender.gpstrackitem.lat));       model->setItem(i,0,firstRow);
         firstRow= new QStandardItem(QString::number(x.gpssender.gpstrackitem.lon));       model->setItem(i,1,firstRow);
         firstRow= new QStandardItem(QString::number(x.gpssender.gpstrackitem.speed));     model->setItem(i,2,firstRow);
         firstRow= new QStandardItem(QString::number(x.gpssender.gpstrackitem.course));    model->setItem(i,3,firstRow);
         firstRow= new QStandardItem(x.gpssender.gpstrackitem.datatime.toString("yyyy-MM-dd hh:mm:ss"));    model->setItem(i,4,firstRow);

         i++;
     }
    //применяем модель к таблице
    ui->gpsTrackView->setModel(model);
}

////////////////////////////////////////////////
///обновление селектора номера и ID машин
////////////////////////////////////////////////
void dispWindow::nomerSelektorGet()
 {
     ui->nomerSelector->clear();
     ui->sensorsNomer->clear();
     NumberCarID.clear();

     foreach(x.gpssender.gpsitem, x.gpssender.gpslist)
      {
         ui->nomerSelector->addItem(x.gpssender.gpsitem.car_number);
         ui->sensorsNomer->addItem(x.gpssender.gpsitem.car_number);
         NumberCarID[x.gpssender.gpsitem.car_number]=QString::number(x.gpssender.gpsitem.car_id);
      }
 }

////////////////////////////////////////////////
///Получение данных для поиска тоеков
////////////////////////////////////////////////
void dispWindow::getGPSTrackData()
 {

  QString dataStart = ui->periodStart->dateTime().toString("yyyy-MM-dd hh:mm:ss");
  QString dataEnd =   ui->periodEnd->dateTime().toString("yyyy-MM-dd hh:mm:ss");
  QString carID  =    NumberCarID[ui->nomerSelector->currentText()];

  x.getGPSTrack(dataStart, dataEnd,carID);

 }

///////////////////////////////////////////////////
///Получение данных для заполнения таблицы датчиков
///////////////////////////////////////////////////
void dispWindow::getPicStatusData()
{
    QString dataStart = ui->periodSensorsStart->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString dataEnd =   ui->periodSensorsEnd->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    qDebug()<<ui->sensorsNomer->currentText();

    QString carID  =    NumberCarID[ui->sensorsNomer->currentText()];
    qDebug()<< carID;

    x.getPicSensors(dataStart, dataEnd,carID);
}

dispWindow::~dispWindow()
{
    delete ui;
}


