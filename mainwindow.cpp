#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_currentChanged(const QModelIndex &currrent, const QModelIndex &previous)
{
    ui->actCommit->setEnabled(tabModel->isDirty());
    ui->actGiveUp->setEnabled(tabModel->isDirty());
}

void MainWindow::on_currentRowChanged(const QModelIndex &currrent, const QModelIndex &previous)
{
    dataMapper->setCurrentIndex(currrent.row());
    QSqlRecord curRec = tabModel->record(currrent.row());
    if(curRec.value("isVaccined").toInt() == 2)
        ui->actGetVac->setEnabled(0);
    else
        ui->actGetVac->setEnabled(1);
    statusLab.setText(curRec.value("name").toString() + QString(" %1").arg(curRec.value("years").toInt())+ " " + vacList[curRec.value("isVaccined").toInt()]);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);
    QStringList strList;
    strList << "male" << "female";
    sexDel = new QComboBoxDelegate(strList,this);
    ageDel = new QIntSpinBoxDelegate(this);
    vacDel = new QisVaccineDeldgate(this);
    ui->statusBar->addWidget(&statusLab);
    on_actRefresh_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete sexDel;
    delete ageDel;
}

void MainWindow::on_actRefresh_triggered()
{
    /*QString aFile = QFileDialog::getOpenFileName(this,"选择数据库文件","","*.db");
    if(aFile.isEmpty())return;
    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName("aFile");*/
    DB = QSqlDatabase::addDatabase("QMYSQL");
    DB.setHostName("sql.wsfdb.cn");
    DB.setUserName("2278Community");
    DB.setDatabaseName("2278community");
    DB.setPassword("5817518");
    DB.setPort(3306);
    if(!DB.open())
    {
        QMessageBox::warning(this,"失败","打开数据库失败");
        return;
    }
    DB.exec("SET character_set_server = utf8;");
    openTable();

}

void MainWindow::openTable()
{
    tabModel = new QSqlTableModel(this,DB);
    tabModel->setTable("person");
    tabModel->setSort(tabModel->fieldIndex("ID"),Qt::AscendingOrder);
    tabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    if(!(tabModel->select()))
    {
        QMessageBox::critical(this,"错误",tabModel->lastError().text());
        return;
    }
    tabModel->setHeaderData(tabModel->fieldIndex("ID"),Qt::Horizontal,"身份证号码");
    tabModel->setHeaderData(tabModel->fieldIndex("name"),Qt::Horizontal,"姓名");
    tabModel->setHeaderData(tabModel->fieldIndex("years"),Qt::Horizontal,"年龄");
    tabModel->setHeaderData(tabModel->fieldIndex("sex"),Qt::Horizontal,"性别");
    tabModel->setHeaderData(tabModel->fieldIndex("tel"),Qt::Horizontal,"电话");
    tabModel->setHeaderData(tabModel->fieldIndex("addr"),Qt::Horizontal,"地址");
    tabModel->setHeaderData(tabModel->fieldIndex("time"),Qt::Horizontal,"接种时间");
    theSel = new QItemSelectionModel(tabModel);
    connect(theSel,SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(on_currentChanged(QModelIndex,QModelIndex)));
    connect(theSel,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(on_currentRowChanged(QModelIndex,QModelIndex)));
    ui->tableView->setModel(tabModel);
    ui->tableView->setSelectionModel(theSel);
    ui->tableView->setItemDelegateForColumn(tabModel->fieldIndex("years"),ageDel);
    ui->tableView->setItemDelegateForColumn(tabModel->fieldIndex("sex"),sexDel);
   // ui->tableView->setItemDelegateForColumn(tabModel->fieldIndex("isVaccined"),vacDel);
    dataMapper = new QDataWidgetMapper();
    dataMapper->setModel(tabModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    //dataMapper->addMapping(ui->IDEdit,tabModel->fieldIndex("ID"));
    //dataMapper->addMapping(ui->addrEdit,tabModel->fieldIndex("addr"));
    //dataMapper->addMapping(ui->telEdit,tabModel->fieldIndex("tel"));
    //dataMapper->addMapping(ui->sexCom,tabModel->fieldIndex("sex"));
    //dataMapper->toFirst();
    ui->tableView->setColumnHidden(tabModel->fieldIndex("isVaccined"),1);
    getFileName();
}

void MainWindow::getFileName()
{
    QSqlRecord emptyRec = tabModel->record();
    for (int i = 0;i <emptyRec.count() ;i++ )
    {
        ui->sortCom->addItem(emptyRec.fieldName(i));
    }
}


void MainWindow::on_actGetVac_triggered()
{
    QSqlRecord curRec = tabModel->record(ui->tableView->currentIndex().row());
    int isVac = curRec.value("isVaccined").toInt();
    isVac++;
    curRec.setValue("isVaccined",isVac);
    tabModel->setRecord(ui->tableView->currentIndex().row(),curRec);
    on_currentRowChanged(ui->tableView->currentIndex(),ui->tableView->rootIndex());
}


void MainWindow::on_actAdd_triggered()
{
    tabModel->insertRow(tabModel->rowCount(),QModelIndex());
    QModelIndex curIndex = tabModel->index(tabModel->rowCount() - 1,1);
    theSel->clearSelection();
    theSel->setCurrentIndex(curIndex,QItemSelectionModel::Select);
}


void MainWindow::on_actCommit_triggered()
{
    bool res = tabModel->submitAll();
    if(!res)
    {
        QMessageBox::critical(this,"提交错误",tabModel->lastError().text());
    }
    else
    {
        ui->actCommit->setEnabled(false);
        ui->actGiveUp->setEnabled(false);
    }
}


void MainWindow::on_actGiveUp_triggered()
{
    tabModel->revertAll();
    ui->actCommit->setEnabled(false);
    ui->actGiveUp->setEnabled(false);
}


void MainWindow::on_actRemove_triggered()
{
    tabModel->removeRow(theSel->currentIndex().row());
}

void MainWindow::on_sortCom_currentIndexChanged(int index)
{
    if(ui->ascRadio->isChecked())
        tabModel->setSort(index,Qt::AscendingOrder);
    else
        tabModel->setSort(index,Qt::DescendingOrder);
    tabModel->select();
}


void MainWindow::on_ascRadio_clicked()
{
    on_sortCom_currentIndexChanged(ui->sortCom->currentIndex());
}


void MainWindow::on_decRadio_clicked()
{
    on_sortCom_currentIndexChanged(ui->sortCom->currentIndex());
}

