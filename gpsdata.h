#ifndef GPSDATA_H
#define GPSDATA_H

#include <QDateTime>
#include <QStringList>

class gpsdata {

public:

    gpsdata();

    struct gpsstruct
   {
   QDateTime datatime;
   double lat;
   double lon;
   double speed;
   double course;
   QString car_number;
   QString car_name;
   int car_id;
   int activiti_id;
   int gps_table_id;
   int pic_table_id;
   int crew_id;
   bool now_online;
   };

   struct gpssTrucTrack
   {
   QDateTime datatime;
   double lat;
   double lon;
   double speed;
   double course;
   };

   struct picSensors
   {
   QDateTime datatime;
   QStringList an;
   QStringList dg;
   QString pot;
   QString tmp;
   };


   QList<gpsstruct> gpslist;

   gpsstruct gpsbuffer;
   gpsstruct gpsitem;

   QList<gpssTrucTrack> gpstracklist;

   gpssTrucTrack gpstrackbuffer;
   gpssTrucTrack gpstrackitem;

   QList<picSensors> piclist;

   picSensors picbuffer;
   picSensors picitem;

   bool lightvisual;

};
#endif // GPSDATA_H
