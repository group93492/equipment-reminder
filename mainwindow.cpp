#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList List;
    List << "cabinet" << "date" << "time";
    ui->comboBox->addItems(List);
    ui->comboBox->setCurrentIndex(1);
    m_addeventDialog = new addeventDialog(this, true);
    m_editeventDialog = new addeventDialog(this, false);
    m_DataBase.connectToBase();
    connect(&m_DataBase, SIGNAL(tableModel(QSqlTableModel*)), this, SLOT(lookTable(QSqlTableModel*)));
    connect(m_addeventDialog, SIGNAL(addEventSignal(QString,QString,QString,QString)), &m_DataBase, SLOT(addEvent(QString,QString,QString,QString)));
    connect(m_editeventDialog, SIGNAL(editEventSignal(quint8,QString,QString,QString,QString)), &m_DataBase, SLOT(editEvent(quint8,QString,QString,QString,QString)));
    connect(this, SIGNAL(deleteEventSignal(quint8)), &m_DataBase, SLOT(deleteEvent(quint8)));
    m_DataBase.sendModel();
    ui->tableView->hideColumn(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::lookTable(QSqlTableModel *model)
{
    model->sort(ui->comboBox->currentIndex()+1, Qt::AscendingOrder);
    ui->tableView->setModel(model);
}

void MainWindow::on_addeventButton_clicked()
{
    m_addeventDialog->show();
    ui->editeventButton->setEnabled(false);
    ui->deleteeventButton->setEnabled(false);
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    ui->editeventButton->setEnabled(true);
    ui->deleteeventButton->setEnabled(true);
    m_currentCell = index;
    m_editeventDialog->setElements(m_currentCell.sibling(m_currentCell.row(), 0).data().toUInt(),
                                  m_currentCell.sibling(m_currentCell.row(), 1).data().toString(),
                                  m_currentCell.sibling(m_currentCell.row(), 2).data().toString(),
                                  m_currentCell.sibling(m_currentCell.row(), 3).data().toString(),
                                  m_currentCell.sibling(m_currentCell.row(), 4).data().toString());
}

void MainWindow::on_editeventButton_clicked()
{
    m_editeventDialog->show();
    ui->editeventButton->setEnabled(false);
    ui->deleteeventButton->setEnabled(false);
}

void MainWindow::on_deleteeventButton_clicked()
{
    ui->editeventButton->setEnabled(false);
    ui->deleteeventButton->setEnabled(false);
    emit deleteEventSignal(m_currentCell.sibling(m_currentCell.row(), 0).data().toUInt());
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->tableView->sortByColumn(index+1, Qt::AscendingOrder);
}
