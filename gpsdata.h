#ifndef GPSDATA_H
#define GPSDATA_H

#include <QDateTime>

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

   QList<gpsstruct> gpslist;

   gpsstruct gpsbuffer;
   gpsstruct gpsitem;


};
#endif // GPSDATA_H
