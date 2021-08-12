#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QDataWidgetMapper>
#include <QFileDialog>
#include <QMessageBox>

#include "qcomboboxdelegate.h"
#include "qintspinboxdelegate.h"
#include "qisvaccinedeldgate.h"
#include "qgetvaccineddelegate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    //void on_currentChanged(const QModelIndex &currrent,const QModelIndex &previous);
    void on_currentRowChanged(const QModelIndex &currrent,const QModelIndex &previous);
    void getVaccined(const QModelIndex &index);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actOpen_triggered();

private:
    Ui::MainWindow *ui;
    QSqlDatabase DB;
    QSqlTableModel * tabModel;
    QItemSelectionModel * theSel;
    QDataWidgetMapper * dataMapper;
    void openTable();
    void getFileName();
    QComboBoxDelegate * sexDel;
    QIntSpinBoxDelegate * ageDel;
    QisVaccineDeldgate * vacDel;
    QgetVaccinedDelegate * gVacDel;
};
#endif // MAINWINDOW_H
