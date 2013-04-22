#include "cartabledelegate.h"

CarTableDelegate::CarTableDelegate()
{
}

QWidget *CarTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 0) {
        QSpinBox *editor = new QSpinBox(parent);
        int currentValue = index.model()->data(index, Qt::DisplayRole).toInt();
        editor->setValue(currentValue);
        return editor;
    }else if (index.column() == 1) {
        QLineEdit *editor = new QLineEdit(parent);
        QString currentText = index.model()->data(index, Qt::DisplayRole).toString();
        editor->setText(currentText);
        return editor;
    }else if (index.column() == 2) {
        QLineEdit *editor = new QLineEdit(parent);
        QString currentText = index.model()->data(index, Qt::DisplayRole).toString();
        editor->setText(currentText);
        return editor;
    }else   if (index.column() == 3) {
        QSpinBox *editor = new QSpinBox(parent);
        int currentValue = index.model()->data(index, Qt::DisplayRole).toInt();
        editor->setValue(currentValue);
        return editor;
    }else   if (index.column() == 4) {
        QSpinBox *editor = new QSpinBox(parent);
        int currentValue = index.model()->data(index, Qt::DisplayRole).toInt();
        editor->setValue(currentValue);
        return editor;
    }else   if (index.column() == 5) {
        QSpinBox *editor = new QSpinBox(parent);
        int currentValue = index.model()->data(index, Qt::DisplayRole).toInt();
        editor->setValue(currentValue);
        return editor;
    }else   if (index.column() == 6) {
        QSpinBox *editor = new QSpinBox(parent);
        int currentValue = index.model()->data(index, Qt::DisplayRole).toInt();
        editor->setValue(currentValue);
        return editor;
    }else   if (index.column() == 7) {
        QSpinBox *editor = new QSpinBox(parent);
        int currentValue = index.model()->data(index, Qt::DisplayRole).toInt();
        editor->setValue(currentValue);
        return editor;
    }
}

void CarTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.column() == 0) {
        QSpinBox *dateEditor = qobject_cast<QSpinBox *>(editor);
        if (dateEditor) {
            dateEditor->setValue(index.model()->data(index, Qt::EditRole).toInt());
        }
    } else if (index.column() == 1) {
        QLineEdit *textEditor = qobject_cast<QLineEdit *>(editor);
        if (textEditor) {
            textEditor->setText(index.model()->data(index, Qt::EditRole).toString());
        }
    } else if (index.column() == 2) {
        QLineEdit *textEditor = qobject_cast<QLineEdit *>(editor);
        if (textEditor) {
            textEditor->setText(index.model()->data(index, Qt::EditRole).toString());
        }
    } else if (index.column() == 3) {
        QSpinBox *dateEditor = qobject_cast<QSpinBox *>(editor);
        if (dateEditor) {
          dateEditor->setValue(index.model()->data(index, Qt::EditRole).toInt());
        }
    } else if (index.column() == 4) {
        QSpinBox *dateEditor = qobject_cast<QSpinBox *>(editor);
        if (dateEditor) {
   dateEditor->setValue(index.model()->data(index, Qt::EditRole).toInt());
        }
    } else if (index.column() == 5) {
        QSpinBox *dateEditor = qobject_cast<QSpinBox *>(editor);
        if (dateEditor) {
            dateEditor->setValue(index.model()->data(index, Qt::EditRole).toInt());
        }
    } else if (index.column() == 6) {
        QSpinBox *dateEditor = qobject_cast<QSpinBox *>(editor);
        if (dateEditor) {
            dateEditor->setValue(index.model()->data(index, Qt::EditRole).toInt());
        }
    } else   if (index.column() == 7) {
        QSpinBox *dateEditor = qobject_cast<QSpinBox *>(editor);
        if (dateEditor) {
            dateEditor->setValue(index.model()->data(index, Qt::EditRole).toInt());
        }
    }
}

void CarTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.column() == 0) {
        QSpinBox *dateEditor = qobject_cast<QSpinBox *>(editor);
        if (dateEditor) {
            model->setData(index, QString::number(dateEditor->value()));
        }
    } else   if (index.column() == 1) {
        QLineEdit *textEditor = qobject_cast<QLineEdit *>(editor);
        if (textEditor) {
            model->setData(index, textEditor->text());
        }
    } else   if (index.column() == 2) {
        QLineEdit *textEditor = qobject_cast<QLineEdit *>(editor);
        if (textEditor) {
            model->setData(index, textEditor->text());
        }
    } else    if (index.column() == 3) {
        QSpinBox *dateEditor = qobject_cast<QSpinBox *>(editor);
        if (dateEditor) {
            model->setData(index, QString::number(dateEditor->value()));
        }
    }else     if (index.column() == 4) {
        QSpinBox *dateEditor = qobject_cast<QSpinBox *>(editor);
        if (dateEditor) {
            model->setData(index, QString::number(dateEditor->value()));
        }
    } else    if (index.column() == 5) {
          QSpinBox *dateEditor = qobject_cast<QSpinBox *>(editor);
          if (dateEditor) {
            model->setData(index, QString::number(dateEditor->value()));
          }
      }else  if (index.column() == 6) {
          QSpinBox *dateEditor = qobject_cast<QSpinBox *>(editor);
          if (dateEditor) {
            model->setData(index, QString::number(dateEditor->value()));
          }
      } else  if (index.column() == 7) {
          QSpinBox *dateEditor = qobject_cast<QSpinBox *>(editor);
          if (dateEditor) {
            model->setData(index, QString::number(dateEditor->value()));
          }
      }
}
