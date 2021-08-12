#ifndef QCOMBOBOXDELEGATE_H
#define QCOMBOBOXDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QList>

class QComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    QComboBoxDelegate(QStringList list,QObject * parent = nullptr);
    QWidget *createEditor(QWidget * parent,const QStyleOptionViewItem &option,const QModelIndex &index) const override;
    void setEditorData(QWidget * editor,const QModelIndex &index) const override;
    void setModelData(QWidget * editor,QAbstractItemModel * model,const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget * editor,const QStyleOptionViewItem &option,const QModelIndex &index) const override;
private:
    QStringList strList;
};

#endif // QCOMBOBOXDELEGATE_H
