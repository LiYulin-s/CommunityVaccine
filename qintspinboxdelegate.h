#ifndef QINTSPINBOXDELEGATE_H
#define QINTSPINBOXDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>
#include <QWidget>
#include <QSpinBox>

class QIntSpinBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit QIntSpinBoxDelegate(QObject *parent = nullptr);
    QIntSpinBoxDelegate(int max,int min,QObject * parent = nullptr);
    QWidget *createEditor(QWidget * parent,const QStyleOptionViewItem &option,const QModelIndex &index) const override;
    void setEditorData(QWidget * editor,const QModelIndex &index) const override;
    void setModelData(QWidget * editor,QAbstractItemModel * model,const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget * editor,const QStyleOptionViewItem &option,const QModelIndex &index) const override;

private:
    int max = 100;
    int min = 0;
};

#endif // QINTSPINBOXDELEGATE_H
