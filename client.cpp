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
                this, SLOT(resivActiveGPSClient(QVariant &)),
                this, SLOT(testFault(int, const QString &)));

}

/////////////////////////////////////////////////////
///Получение списка активных машин
/////////////////////////////////////////////////////
void Client::resivActiveGPSClient(QVariant &arg) {

//получаем данные от сервера
 qDebug() << arg;
//конвертируем их в список строк
QStringList gpslinelist = arg.toStringList();


//Добавляем полученный список активных машин в
//динамический список, предварительно его очищаем
gpssender.gpslist.clear();

foreach(QString gpsline, gpslinelist){

    QStringList gps_data = gpsline.split(",");

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
}


/////////////////////////////////////////////////////
///обработка сбоев
/////////////////////////////////////////////////////
void Client::testFault(int error, const QString &message) {
        qDebug() << "EEE:" << error << "-" << message;
        gpssender.gpslist.clear();
}

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
