#ifndef CARTABLEDELEGATE_H
#define CARTABLEDELEGATE_H

#include <QItemDelegate>
#include <QDateTimeEdit>
#include <QTimeEdit>
#include <QLineEdit>
#include <QSpinBox>

class CarTableDelegate : public QItemDelegate
{
public:

    CarTableDelegate();

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif // CARTABLEDELEGATE_H
