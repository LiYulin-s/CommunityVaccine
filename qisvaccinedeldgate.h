#ifndef QISVACCINEDELDGATE_H
#define QISVACCINEDELDGATE_H

#include <QStyledItemDelegate>
#include <QObject>
#include <QWidget>
#include <QTextEdit>

class QisVaccineDeldgate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit QisVaccineDeldgate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget * parent,const QStyleOptionViewItem &option,const QModelIndex &index) const override;
    void setEditorData(QWidget * editor,const QModelIndex &index) const override;
    void setModelData(QWidget * editor,QAbstractItemModel * model,const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget * editor,const QStyleOptionViewItem &option,const QModelIndex &index) const override;
};

#endif // QISVACCINEDELDGATE_H
