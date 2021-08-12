#include "qgetvaccineddelegate.h"

void QgetVaccinedDelegate::sendSignal()
{
    emit clicked(*index);
}

QgetVaccinedDelegate::QgetVaccinedDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

QWidget *QgetVaccinedDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPushButton * editor = new QPushButton(parent);
    return editor;
}

void QgetVaccinedDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QPushButton * button = static_cast<QPushButton *>(editor);
    if(index.model()->data(index.model()->index(index.row(),6)).toInt() == 2)
    {
        button->setEnabled(false);
    }
    button->setText("注射疫苗");
    connect(button,SIGNAL(clicked()),this,SLOT(sendSignal()));
}

void QgetVaccinedDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

}

void QgetVaccinedDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
