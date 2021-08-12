#include "qisvaccinedeldgate.h"

QisVaccineDeldgate::QisVaccineDeldgate(QObject *parent) : QStyledItemDelegate(parent)
{

}

QWidget *QisVaccineDeldgate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTextEdit * editor = new QTextEdit(parent);
    editor->setReadOnly(true);
    return editor;
}

void QisVaccineDeldgate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QTextEdit * edit = static_cast<QTextEdit *>(editor);
    int value = index.model()->data(index,Qt::EditRole).toInt();
    if(value == 0)
    {
        edit->setTextColor(QColor(255,0,0));
        edit->setText("未注射疫苗");
    }
    else
    {
        edit->setTextColor(QColor(0,255,0));
        edit->setText(QString("已注射疫苗:%1针").arg(value));
    }
}

void QisVaccineDeldgate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

}

void QisVaccineDeldgate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
