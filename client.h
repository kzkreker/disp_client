#ifndef CLIENT_H
#define CLIENT_H

#include "./maiaXmlRpcClient.h"
#include "gpsdata.h"

class Client : public QObject {
    Q_OBJECT

    public:
        Client(QObject* parent = 0);
        gpsdata gpssender;
        gpsdata gpstracksender;
        QDateTime queruDate;

    signals:
        void newActiveGPSResiv(gpsdata);
        void newGPSTrackResiv(gpsdata);
        void newPicSensorsResiv();

    public slots:
        void resivActiveGPS();

        void getGPSTrack(QString dataStart,QString dataEnd,QString carID);
        void getPicSensors(QString dataStart,QString dataEnd,QString carID);

        void setDate(QDateTime date);

    private slots:

        void testFault(int, const QString &);

        void resivActiveGPSClient(QVariant &arg);
        void resivGPSTrack(QVariant &arg);
        void resivPicSensors(QVariant &arg);

        void handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);

    private:
        MaiaXmlRpcClient *rpc;

};

#endif
