#include "qintspinboxdelegate.h"

QIntSpinBoxDelegate::QIntSpinBoxDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

QIntSpinBoxDelegate::QIntSpinBoxDelegate(int max_, int min_, QObject *parent)
{
    max = max_;
    min = min_;
}

QWidget *QIntSpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSpinBox * editor = new QSpinBox(parent);
    editor->setFrame(false);
    editor->setMinimum(min);
    editor->setMaximum(max);
    return editor;
}

void QIntSpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index,Qt::EditRole).toInt();
    QSpinBox * spinBox = static_cast<QSpinBox *>(editor);
    spinBox->setValue(value);
}

void QIntSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox * spinBox = static_cast<QSpinBox *>(editor);
    spinBox->interpretText();
    int value = spinBox->value();
    model->setData(index,value,Qt::EditRole);
}

void QIntSpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
