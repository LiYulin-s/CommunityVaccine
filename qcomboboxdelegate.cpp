#include "qcomboboxdelegate.h"

QComboBoxDelegate::QComboBoxDelegate(QStringList list,QObject * parent)
{
    Q_UNUSED(parent);
    strList = list;
}

QWidget *QComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox * editor = new QComboBox(parent);
    editor->setFrame(false);
    editor->addItems(strList);
    return editor;
}

void QComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index,Qt::EditRole).toString();
    QComboBox * combobox = static_cast<QComboBox *>(editor);
    combobox->setCurrentText(value);
}

void QComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox * combobox = static_cast<QComboBox *>(editor);
    QString value = combobox->currentText();
    model->setData(index,value,Qt::EditRole);
}

void QComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
