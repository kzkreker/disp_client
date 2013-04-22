#ifndef CARTABLEMODEL_H
#define CARTABLEMODEL_H



#include <QAbstractTableModel>
#include <QList>
#include <QStringList>
#include "cardata.h"

class CarTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CarTableModel(QObject *parent = 0);

       QList<CarData *> list; // список, в котором хранятся все данные
       QStringList header_data; // список заголовков столбцов

       QVariant data(const QModelIndex &index, int role) const;
       bool     setData(const QModelIndex &index, const QVariant &value, int role);

       int rowCount(const QModelIndex &parent=QModelIndex()) const;
       int columnCount(const QModelIndex &parent) const;

       QVariant headerData(int section,Qt::Orientation orientation,
                           int role=Qt::DisplayRole) const;
       Qt::ItemFlags flags(const QModelIndex &index) const;

signals:
    
public slots:
    
};

#endif // CARTABLEMODEL_H
