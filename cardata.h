#ifndef CARDATA_H
#define CARDATA_H
#include <QString>

class CarData
{
public:
    //конструктор
    CarData();

    //информационная часть
    int car_id;
    int gps_table_id;
    int pic_table_id;
    int crew_id;
    int car_body;
    int car_engine;
    QString car_number;
    QString car_name;

 };

#endif // CARDATA_H
