#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    DataBase.connectToBase();
    connect(&DataBase, SIGNAL(tableModel(QSqlTableModel*)), this, SLOT(lookTable(QSqlTableModel*)));
    DataBase.sendModel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::lookTable(QSqlTableModel *model)
{
    ui->tableView->setModel(model);
}
