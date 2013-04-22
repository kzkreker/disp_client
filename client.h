#ifndef CLIENT_H
#define CLIENT_H

#include "./maiaXmlRpcClient.h"
#include "gpsdata.h"

class Client : public QObject {
    Q_OBJECT

    public:
        Client(QObject* parent = 0);
        gpsdata gpssender;
        QDateTime queruDate;

    signals:
        void newActiveGPSResiv(gpsdata);

    public slots:
        void resivActiveGPS();
        void setDate(QDateTime date);
    private slots:

        void testFault(int, const QString &);
        void resivActiveGPSClient(QVariant &arg);
        void handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);


    private:
        MaiaXmlRpcClient *rpc;

};

#endif
