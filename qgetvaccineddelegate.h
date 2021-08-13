#ifndef QGETVACCINEDDELEGATE_H
#define QGETVACCINEDDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QtSql>

class QgetVaccinedDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public slots:
    void sendSignal();

public:
    explicit QgetVaccinedDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget * parent,const QStyleOptioViewItem &option,const QModelIndex &index) const override;
    void setEditorData(QWidget * editor,const QModelIndex &index) const override;
    void setModelData(QWidget * editor,QAbstractItemModel * model,const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget * editor,const QStyleOptionViewItem &option,const QModelIndex &index) const override;
signals:
    void clicked(const QModelIndex index);

private:
    QModelIndex * index;
};

#endif // QGETVACCINEDDELEGATE_H
