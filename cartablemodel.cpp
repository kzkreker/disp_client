#include "cartablemodel.h"

CarTableModel::CarTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    header_data << QString::fromUtf8("id машины")
                << QString::fromUtf8("Марка")
                << QString::fromUtf8("Госномер")
                << QString::fromUtf8("Номер кузова")
                << QString::fromUtf8("Номер двигателя")
                << QString::fromUtf8("Экипаж")
                << QString::fromUtf8("Таблица GPS")
                << QString::fromUtf8("Таблица PIC");

    for(int i = 0; i < 20; i++){
         CarData * it = new CarData;
          list.append(it);
      }

}


QVariant CarTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= list.size())
        return QVariant();
    // для каждого столбца возвращаем нужные данные
    if (role == Qt::DisplayRole || role == Qt::EditRole){
        if (index.column() == 0 )
            return list.at(index.row())->car_id;
        if (index.column() == 1 )
            return list.at(index.row())->car_name;
        if (index.column() == 2 )
            return list.at(index.row())->car_number;
        if (index.column() == 3 )
            return list.at(index.row())->car_body;
        if (index.column() == 4 )
            return list.at(index.row())->car_engine;
        if (index.column() == 5 )
            return list.at(index.row())->crew_id;
        if (index.column() == 6 )
            return list.at(index.row())->gps_table_id;
        if (index.column() == 7 )
            return list.at(index.row())->pic_table_id;
    }
    return QVariant();
}

bool CarTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        // записываем данные из каждого столбца
        if(index.column()==0){
                   list.at(index.row())->car_id = value.toInt();
               }
               if(index.column()==1){
                   list.at(index.row())->car_name = value.toString();
               }
               if(index.column()==2){
                   list.at(index.row())->car_number = value.toString();
               }
        return true;
    }
    return false;
}

int CarTableModel::rowCount(const QModelIndex &parent) const
{
    return list.size(); // размер списка - это количество строк
}

int CarTableModel::columnCount(const QModelIndex &parent)    const
{
    return 8; // 8 столбца
}

QVariant CarTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
   if(role != Qt::DisplayRole)
           return QVariant();
   if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
       return header_data.at(section); // заголовки столбцов
   }else{
       return QString("%1").arg( section + 1 ); // возвращаем номера строк
   }
}

Qt::ItemFlags CarTableModel::flags(const QModelIndex &index) const
{
   if (!index.isValid())
        return Qt::ItemIsEnabled;

   return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}
