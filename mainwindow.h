#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QDataWidgetMapper>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>

#include "qcomboboxdelegate.h"
#include "qintspinboxdelegate.h"
#include "qisvaccinedeldgate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void on_currentChanged(const QModelIndex &currrent,const QModelIndex &previous);
    void on_currentRowChanged(const QModelIndex &currrent,const QModelIndex &previous);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actRefresh_triggered();

    void on_actGetVac_triggered();

    void on_actAdd_triggered();

    void on_actCommit_triggered();

    void on_actGiveUp_triggered();

    void on_actRemove_triggered();

    void on_sortCom_currentIndexChanged(int index);

    void on_ascRadio_clicked();

    void on_decRadio_clicked();

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
    QLabel statusLab;
    QStringList vacList = {"未接种","已接种1针","已接种2针"};
};
#endif // MAINWINDOW_H
