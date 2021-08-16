#ifndef QTYPEDELEGATE_H
#define QTYPEDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>
#include <QWidget>
#include <QComboBox>

class QTypeDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit QTypeDelegate(QStringList typeList,QObject *parent = nullptr);
    QWidget *createEditor(QWidget * parent,const QStyleOptionViewItem &option,const QModelIndex &index) const override;
    void setEditorData(QWidget * editor,const QModelIndex &index) const override;
    void setModelData(QWidget * editor,QAbstractItemModel * model,const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget * editor,const QStyleOptionViewItem &option,const QModelIndex &index) const override;

private:
    QStringList types;

};

#endif // QTYPEDELEGATE_H
