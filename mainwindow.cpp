#include "mainwindow.h"
#include "ui_mainwindow.h"

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
}

MainWindow::~MainWindow()
{
    delete ui;
    delete sexDel;
    delete ageDel;
}

void MainWindow::on_actOpen_triggered()
{
    QString aFile = QFileDialog::getOpenFileName(this,"选择数据库文件","","*.db");
    if(aFile.isEmpty())return;
    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName(aFile);
    if(!DB.open())
    {
        QMessageBox::warning(this,"失败","打开数据库失败");
        return;
    }
    openTable();

}

void MainWindow::openTable()
{
    tabModel = new QSqlTableModel(this,DB);
    tabModel->setTable("person");
    tabModel->setSort(tabModel->fieldIndex("ID"),Qt::AscendingOrder);
    tabModel->setEditStrategy(QSqlTableModel::OnRowChange);
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
    //tabModel->setHeaderData(tabModel->fieldIndex("isVaccined"),Qt::Horizontal,"是否接种");
    tabModel->setHeaderData(tabModel->fieldIndex("time"),Qt::Horizontal,"接种时间");
    theSel = new QItemSelectionModel(tabModel);
    //connect(theSel,SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(on_currentChanged(QModelIndex,QModelIndex)));
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

