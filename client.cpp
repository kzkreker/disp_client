#include "client.h"


//////////////////////////////////////////////////////
///конструктор клиента
//////////////////////////////////////////////////////
Client::Client(QObject* parent) : QObject(parent) {
    //	rpc = new MaiaXmlRpcClient(QUrl("http://phpxmlrpc.sourceforge.net/server.php"), this);
    //	rpc = new MaiaXmlRpcClient(QUrl("https://rpc.gandi.net/xmlrpc/2.0/"), this);

    //вундервафля работает поверх апача, однако цепляемся к самодельному серверу
    rpc = new MaiaXmlRpcClient(QUrl("http://dataBase:8083/RPC2"), this);
    // конфиги
    QSslConfiguration config = rpc->sslConfiguration();
    config.setProtocol(QSsl::AnyProtocol);


    rpc->setSslConfiguration(config);

    //сигнал-слот обработка ошибок
    connect(rpc, SIGNAL(sslErrors(QNetworkReply *, const QList<QSslError> &)),
            this, SLOT(handleSslErrors(QNetworkReply *, const QList<QSslError> &)));
 }

///////////////////////////////////////////////////
///Отправка кординат
///////////////////////////////////////////////////
void Client::resivActiveGPS() {

    QVariantList args;

    args<<queruDate.toString("yyyy-MM-dd hh:mm:ss");;

    rpc->call("transport.sendActiveGPS", args,
                this, SLOT(resivActiveGPSClient(QVariant&)),
                this, SLOT(testFault(int, const QString &)));

}

///////////////////////////////////////////////////
///Запрос трека пройденого ТС
///////////////////////////////////////////////////
void Client::getGPSTrack(QString dataStart,QString dataEnd,QString carID)
{
    QVariantList args;
    //формирование параметра запроса
    args<<dataStart;
    args<<dataEnd;
    args<<carID;
    //отправка запроса на сервер
    rpc->call("transport.getGPSTrack", args,
                this, SLOT(resivGPSTrack(QVariant &)),
                this, SLOT(testFault(int, const QString &)));
}

///////////////////////////////////////////////////
///Запрос показаний датчиков
///////////////////////////////////////////////////
void Client::getPicSensors(QString dataStart,QString dataEnd,QString carID)
{

    QVariantList args;
    //формирование параметра запроса
    args<<dataStart;
    args<<dataEnd;
    args<<carID;
    //отправка запроса на сервер
    rpc->call("transport.getPICSensors", args,
                this, SLOT(resivPicSensors(QVariant &)),
                this, SLOT(testFault(int, const QString &)));
}

/////////////////////////////////////////////////////
///Получение процйденого трека
/////////////////////////////////////////////////////
void Client::resivGPSTrack(QVariant &arg)
{
    qDebug()<<arg;

    QStringList gpslinelist = arg.toString().split("~");
QStringList gps_data;

    //Добавляем полученный список активных машин в
    //динамический список, предварительно его очищаем
    gpssender.gpstracklist.clear();

    foreach(QString gpsline, gpslinelist){


        gps_data= gpsline.split("|");

        if(gps_data.length()==5){
        gpssender.gpstrackbuffer.datatime= QDateTime::fromString(gps_data[0],
                                            "yyyy'-'MM'-'dd'T'hh':'mm':'ss");
        //физические показатели
        gpssender.gpstrackbuffer.speed=   gps_data[1].toDouble();
        gpssender.gpstrackbuffer.course=  gps_data[2].toDouble();
        gpssender.gpstrackbuffer.lat=     gps_data[3].toDouble();
        gpssender.gpstrackbuffer.lon=     gps_data[4].toDouble();

        gpssender.gpstracklist.append(gpssender.gpstrackbuffer);
        } else {
            gpssender.lightvisual = gps_data[0].toInt();}
        }

  emit   newGPSTrackResiv(gpssender);
}

/////////////////////////////////////////////////////
///Получение пройденого трека
/////////////////////////////////////////////////////
void Client::resivPicSensors(QVariant &arg)
{
    qDebug()<<arg;

   QStringList piclinelist = arg.toString().split("~");


    //Добавляем полученный список активных машин в
    //динамический список, предварительно его очищаем
    gpssender.piclist.clear();

    QStringList pic;
    QStringList intan;

    foreach(QString picline, piclinelist){

       pic = picline.split("|");

       qDebug()<<pic;

       if(pic.length()==5){

           intan=pic[0].mid(1,(pic[0].length()-2)).split(",");
           qDebug()<<intan;
           gpssender.picitem.an=intan;
           intan=pic[1].mid(1,(pic[1].length()-2)).split(",");
           qDebug()<<intan;

           gpssender.picitem.dg=intan;
           gpssender.picitem.pot=pic[2];
           gpssender.picitem.tmp=pic[3];
           gpssender.picitem.datatime=QDateTime::fromString(pic[4],
                                                             "yyyy'-'MM'-'dd'T'hh':'mm':'ss");
           gpssender.piclist.append( gpssender.picitem);
       }
    }
  emit   newPicSensorsResiv();
}

/////////////////////////////////////////////////////
///Получение списка активных машин
/////////////////////////////////////////////////////
void Client::resivActiveGPSClient(QVariant &arg) {

//получаем данные от сервера
qDebug() << arg;
//конвертируем их в список строк
QStringList gpslinelist = arg.toStringList();
QStringList gps_data;

//Добавляем полученный список активных машин в
//динамический список, предварительно его очищаем
gpssender.gpslist.clear();

foreach(QString gpsline, gpslinelist){

    gps_data = gpsline.split(",");

    qDebug()<<gps_data;
    //записываем и передаем данные м QList
    //дата последней синхронизации ТС с сервером
    gpssender.gpsbuffer.datatime= QDateTime::fromString(gps_data[0],
                                        "yyyy'-'MM'-'dd'T'hh':'mm':'ss");
    //физические показатели
    gpssender.gpsbuffer.speed=   gps_data[1].toDouble();
    gpssender.gpsbuffer.course=  gps_data[2].toDouble();
    gpssender.gpsbuffer.lat=     gps_data[3].toDouble();
    gpssender.gpsbuffer.lon=     gps_data[4].toDouble();

    //общие данные ТС
    gpssender.gpsbuffer.car_name=  gps_data[5];
    gpssender.gpsbuffer.car_number=gps_data[6];

    //параметры запросов
    gpssender.gpsbuffer.crew_id      = gps_data[7].toInt();
    gpssender.gpsbuffer.car_id       = gps_data[8].toInt();
    gpssender.gpsbuffer.activiti_id  = gps_data[9].toInt();
    gpssender.gpsbuffer.gps_table_id = gps_data[10].toInt();
    gpssender.gpsbuffer.pic_table_id = gps_data[10].toInt();

    //флаг текущей активности
    if (gpssender.gpsbuffer.datatime>=QDateTime::currentDateTime().addSecs(-3)){
               gpssender.gpsbuffer.now_online=true;
      } else {
        gpssender.gpsbuffer.now_online=false;
    }

    gpssender.gpslist.append(gpssender.gpsbuffer);

}
    //передаем новые кординаты в клас слоя
    emit newActiveGPSResiv(gpssender);
    // gpssender.gpslist.clear();
}

/////////////////////////////////////////////////////
///обработка сбоев: потеря соединения
/////////////////////////////////////////////////////
void Client::testFault(int error, const QString &message) {
        qDebug() << "EEE:" << error << "-" << message;

        //сбрасываем массив активных машин
        gpssender.gpslist.clear();
        emit newActiveGPSResiv(gpssender);
}

/////////////////////////////////////////////////////
///обработка сбоев: плохой сертификат
/////////////////////////////////////////////////////
void Client::handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {
    qDebug() << "SSL Error:" << errors;
    reply->ignoreSslErrors(); // don't do this in real code! Fix your certs!
}

/////////////////////////////////////////////////////
///передача данных из QdateTimeedit
/////////////////////////////////////////////////////
void Client::setDate(QDateTime date)
{
queruDate = date;
}
