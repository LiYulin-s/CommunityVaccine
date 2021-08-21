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
    int vacName = curRec.value("vacType").toInt();
    if(vacName == -1)
    {
        ui->actGetVac->setEnabled(1);
    }
    else if(vacTimes[vacModel->record(vacName).value("type").toInt()] == curRec.value("isVaccined").toInt())
        ui->actGetVac->setEnabled(0);
    else
        ui->actGetVac->setEnabled(1);

    QString type;
    if (curRec.value("vacType").toInt() == -1)
        type = " ";
    else
        type = vacnameList[curRec.value("vacType").toInt()];
    statusLab.setText(QString("%1 %2 %3 %4 %5").arg(curRec.value("name").toString()).arg(curRec.value("years").toInt()).arg(vacList[curRec.value("isVaccined").toInt()]).arg((curRec.value("time").toString())).arg(type));
}

void MainWindow::on_vac_currentChanged(const QModelIndex &currrent, const QModelIndex &previous)
{
    ui->actCommit->setEnabled(vacModel->isDirty());
    ui->actGiveUp->setEnabled(vacModel->isDirty());
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
    typeDel = new QTypeDelegate(vacType,this);
    norDel = new QItemDelegate(this);
    ui->statusBar->addWidget(&statusLab);
    on_actRefresh_triggered();
    this->setWindowTitle("Community Vaccines Manager");
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
    QString passwd = QInputDialog::getText(this,"输入密码","密码",QLineEdit::NoEcho);
    DB.setHostName("sql.wsfdb.cn");
    DB.setUserName("2278Community");
    DB.setDatabaseName("2278community");
    DB.setPassword(passwd);
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
    vacModel = new QSqlTableModel(this,DB);
    tabModel->setTable("person");
    vacModel->setTable("vaccines");
    vacModel->setSort(tabModel->fieldIndex("ID"),Qt::AscendingOrder);
    vacModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    vacModel->select();
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
    vacModel->setHeaderData(vacModel->fieldIndex("name"),Qt::Horizontal,"名称");
    vacModel->setHeaderData(vacModel->fieldIndex("type"),Qt::Horizontal,"类型");
    vacModel->setHeaderData(vacModel->fieldIndex("total"),Qt::Horizontal,"数量");
    theSel = new QItemSelectionModel(tabModel);
    vacSel = new QItemSelectionModel(vacModel);
    connect(theSel,SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(on_currentChanged(QModelIndex,QModelIndex)));
    connect(vacSel,SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(on_vac_currentChanged(QModelIndex,QModelIndex)));
    connect(theSel,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(on_currentRowChanged(QModelIndex,QModelIndex)));
    ui->tableView->setModel(tabModel);
    ui->tableView->setSelectionModel(theSel);
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
    ui->tableView->setColumnHidden(tabModel->fieldIndex("time"),1);
    ui->tableView->setColumnHidden(tabModel->fieldIndex("vacType"),1);
    ui->tableView->setItemDelegateForColumn(tabModel->fieldIndex("years"),ageDel);
    ui->tableView->setItemDelegateForColumn(tabModel->fieldIndex("sex"),sexDel);
    ui->tableView->setItemDelegateForColumn(1,norDel);
    getFileName();
}

void MainWindow::getFileName()
{
    ui->sortCom->clear();
    ui->searchCom->clear();
    QSqlTableModel * model = static_cast<QSqlTableModel *>(ui->tableView->model());
    QSqlRecord emptyRec = model->record();
    vacnameList.clear();
    for (int i = 0;i <emptyRec.count() ;i++ )
    {
        ui->sortCom->addItem(emptyRec.fieldName(i));
        ui->searchCom->addItem(emptyRec.fieldName(i));
    }
    for(int i = 0; i < vacModel->rowCount();i++)
    {
         QSqlRecord vacRec = vacModel->record(i);
         vacnameList.append(vacRec.value("name").toString() + ':' + vacType[vacRec.value("type").toInt()]);
    }
}


void MainWindow::on_actGetVac_triggered()
{
    QDateTime date;
    QSqlRecord curRec = tabModel->record(ui->tableView->currentIndex().row());
    int isVac = curRec.value("isVaccined").toInt();
    if (!isVac)
    {
        QString choice = QInputDialog::getItem(this,"选择疫苗","注射的疫苗：",vacnameList);
        QMessageBox::StandardButton butt = QMessageBox::information(this,"确认",QString("确认为%1注射%2疫苗？").arg(curRec.value("name").toString()).arg(choice),QMessageBox::Yes|QMessageBox::No);
        if(butt == QMessageBox::No)
            return;
        for(int i = 0; i < vacModel->rowCount();i++)
        {
            if(vacnameList[i] == choice)
            {
                auto rec = vacModel->record(i);
                rec.setValue("total",vacModel->record(i).value("total").toInt() - 1);
                vacModel->setRecord(i,rec);
                curRec.setValue("vacType",i);
                curRec.setValue("time",curRec.value("time").toString() +date.currentDateTime().toString("yyyy年m月d日") + QTime::currentTime().toString("hh:mm:ssAP") + ' ');
                break;
            }
        }
    }
    else {
        QMessageBox::StandardButton butt = QMessageBox::information(this,"确认",QString("确认为%1注射%2疫苗？").arg(curRec.value("name").toString()).arg(vacnameList[curRec.value("vacType").toInt()]),QMessageBox::Yes|QMessageBox::No);
        if(butt == QMessageBox::No)
            return;
        curRec.setValue("time",curRec.value("time").toString() +date.currentDateTime().toString("yyyy年m月d日") + QTime::currentTime().toString("hh:mm:ssAP") + ' ');
        auto rec = vacModel->record(curRec.value("vacType").toInt());
        rec.setValue("total",vacModel->record(curRec.value("vacType").toInt()).value("total").toInt() - 1);
        vacModel->setRecord(curRec.value("vacType").toInt(),rec);
    }
    isVac++;
    curRec.setValue("isVaccined",isVac);
    tabModel->setRecord(ui->tableView->currentIndex().row(),curRec);
    on_currentRowChanged(ui->tableView->currentIndex(),ui->tableView->rootIndex());
}


void MainWindow::on_actAdd_triggered()
{
    QSqlTableModel * model = static_cast<QSqlTableModel *>(ui->tableView->model());
    model->insertRow(model->rowCount(),QModelIndex());
    QModelIndex curIndex = model->index(model->rowCount() - 1,1);
    if(model->fieldIndex("name") == 0)
    {
        vacSel->clearSelection();
        vacSel->setCurrentIndex(curIndex,QItemSelectionModel::Select);
    }
    else
    {
        theSel->clearSelection();
        theSel->setCurrentIndex(curIndex,QItemSelectionModel::Select);
    }
}


void MainWindow::on_actCommit_triggered()
{
    QSqlTableModel * model = static_cast<QSqlTableModel *>(ui->tableView->model());
    bool res = vacModel->submitAll();
    bool res_ = tabModel->submitAll();
    if(!(res and res_))
    {
        QMessageBox::critical(this,"提交错误",model->lastError().text());
    }
}


void MainWindow::on_actGiveUp_triggered()
{
    QSqlTableModel * model = static_cast<QSqlTableModel *>(ui->tableView->model());
    model->revertAll();
    ui->actCommit->setEnabled(false);
    ui->actGiveUp->setEnabled(false);
}


void MainWindow::on_actRemove_triggered()
{
    QSqlTableModel * model = static_cast<QSqlTableModel *>(ui->tableView->model());
    model->removeRow(ui->tableView->currentIndex().row());
}

void MainWindow::on_sortCom_currentIndexChanged(int index)
{
    QSqlTableModel * model = static_cast<QSqlTableModel *>(ui->tableView->model());
    if(ui->ascRadio->isChecked())
        model->setSort(index,Qt::AscendingOrder);
    else
        model->setSort(index,Qt::DescendingOrder);
    model->select();
}


void MainWindow::on_ascRadio_clicked()
{
    on_sortCom_currentIndexChanged(ui->sortCom->currentIndex());
}


void MainWindow::on_decRadio_clicked()
{
    on_sortCom_currentIndexChanged(ui->sortCom->currentIndex());
}


void MainWindow::on_searchButton_clicked()
{
    QSqlTableModel * model = static_cast<QSqlTableModel *>(ui->tableView->model());
    if(ui->searchEdit->text().isEmpty())
    {
        model->setFilter("");
        model->select();
    }
    else
        model->setFilter(ui->searchCom->currentText() + "=" + + '\'' + ui->searchEdit->text()  + '\'');
}


void MainWindow::on_searchEdit_textChanged(const QString &arg1)
{
    on_searchButton_clicked();
}


void MainWindow::on_actCheckV_triggered(bool is)
{
    /*view = new QTableView();
    view->setGeometry(this->geometry());
    view->setModel(vacModel);
    view->show();*/
    if(is)
    {
        ui->tableView->setModel(vacModel);
        ui->tableView->setSelectionModel(vacSel);
        ui->actGetVac->setEnabled(0);
        ui->actCheckV->setText("查看人员");
        QIcon icon(":/person.png");
        ui->actCheckV->setIcon(icon);
        statusLab.setHidden(1);
        getFileName();
        ui->tableView->setItemDelegate(norDel);
        ui->tableView->setItemDelegateForColumn(vacModel->fieldIndex("type"),typeDel);
    }
    else
    {
        ui->tableView->setModel(tabModel);
        ui->tableView->setColumnHidden(tabModel->fieldIndex("isVaccined"),1);
        ui->tableView->setColumnHidden(tabModel->fieldIndex("time"),1);
        ui->tableView->setColumnHidden(tabModel->fieldIndex("vacType"),1);
        ui->tableView->setItemDelegateForColumn(tabModel->fieldIndex("years"),ageDel);
        ui->tableView->setItemDelegateForColumn(tabModel->fieldIndex("sex"),sexDel);
        ui->tableView->setItemDelegateForColumn(1,norDel);
        ui->actCheckV->setText("查看疫苗");
        statusLab.setHidden(0);
        statusLab.setEnabled(1);
        QIcon icon(":/vaccine.png");
        ui->actCheckV->setIcon(icon);
        getFileName();
        on_currentRowChanged(tabModel->index(0,1),tabModel->index(0,1));
        ui->tableView->setModel(tabModel);
        ui->tableView->setSelectionModel(theSel);
    }
}


