#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_eventDialog = new addeventDialog;
    m_DataBase.connectToBase();
    connect(&m_DataBase, SIGNAL(tableModel(QSqlTableModel*)), this, SLOT(lookTable(QSqlTableModel*)));
    connect(m_eventDialog, SIGNAL(event(QString,QString,QString,QString)), &m_DataBase, SLOT(addEvent(QString,QString,QString,QString)));
    m_DataBase.sendModel();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::lookTable(QSqlTableModel *model)
{
    ui->tableView->setModel(model);
}

void MainWindow::on_pushButton_clicked()
{
    m_eventDialog->show();
}
