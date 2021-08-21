#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QDataWidgetMapper>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QInputDialog>
#include <QTime>
#include <QTableView>
#include <QDateTime>

#include "qcomboboxdelegate.h"
#include "qintspinboxdelegate.h"
#include "qisvaccinedeldgate.h"
#include "qtypedelegate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void on_currentChanged(const QModelIndex &currrent,const QModelIndex &previous);
    void on_currentRowChanged(const QModelIndex &currrent,const QModelIndex &previous);
    void on_vac_currentChanged(const QModelIndex &currrent,const QModelIndex &previous);

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

    void on_searchButton_clicked();

    void on_searchEdit_textChanged(const QString &arg1);

    void on_actCheckV_triggered(bool is);

private:
    Ui::MainWindow *ui;
    QSqlDatabase DB;
    QSqlTableModel * tabModel;
    QSqlTableModel * vacModel;
    QItemSelectionModel * theSel;
    QItemSelectionModel * vacSel;
    QDataWidgetMapper * dataMapper;
    void openTable();
    void getFileName();
    QComboBoxDelegate * sexDel;
    QIntSpinBoxDelegate * ageDel;
    QisVaccineDeldgate * vacDel;
    QTypeDelegate * typeDel;
    QItemDelegate  * norDel;
    QLabel statusLab;
    QStringList vacList = {"未接种","已接种1针","已接种2针","已接种3针"};
    QStringList vacType = {"灭活","重组RNA","腺病毒"};
    QTableView * view;
    int vacTimes[4] = {2,3,1};
    QStringList vacnameList;

};
#endif // MAINWINDOW_H
