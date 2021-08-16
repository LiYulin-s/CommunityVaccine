#include "qtypedelegate.h"

QTypeDelegate::QTypeDelegate(QStringList typeList, QObject *parent)
{
    types = typeList;
}

QWidget *QTypeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox * editor = new QComboBox(parent);
    editor->addItems(types);
    return editor;
}

void QTypeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox * combox = static_cast<QComboBox *>(editor);
    combox->setCurrentIndex(index.model()->data(index,Qt::EditRole).toInt());
}

void QTypeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox * combox = static_cast<QComboBox *>(editor);
    int value = combox->currentIndex();
    model->setData(index,value);
}

void QTypeDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
