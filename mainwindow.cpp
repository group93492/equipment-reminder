#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_addeventDialog = new addeventDialog(this, true);
    m_editeventDialog = new addeventDialog(this, false);
    m_settingsDialog = new settingsDialog(this);
    //create menu
    QMenu *menuGeneral;
    QMenu *menuSettings;
    QMenu *menuAbout;
    menuGeneral = ui->menuBar->addMenu(QString::fromLocal8Bit("Общее"));
    menuSettings = ui->menuBar->addMenu(QString::fromLocal8Bit("Настройки"));
    menuAbout = ui->menuBar->addMenu(QString::fromLocal8Bit("О программе"));
    //general menu
    menuGeneral->addAction(QString::fromLocal8Bit("Добавить событие..."), this, SLOT(on_addeventButton_clicked()));
    m_editAction = menuGeneral->addAction(QString::fromLocal8Bit("Редактировать событие..."), this, SLOT(on_editeventButton_clicked()));
    m_editAction->setEnabled(false);
    m_deleteAction = menuGeneral->addAction(QString::fromLocal8Bit("Удалить событие"), this, SLOT(on_deleteeventButton_clicked()));
    m_deleteAction->setEnabled(false);
    menuGeneral->addSeparator();
    menuGeneral->addAction(QString::fromLocal8Bit("Выход"), qApp, SLOT(quit()));
    //settings menu
    menuSettings->addAction(QString::fromLocal8Bit("Настройки..."), m_settingsDialog, SLOT(show()));
    //about menu
    menuAbout->addAction(QString::fromLocal8Bit("О Qt"), qApp, SLOT(aboutQt()));
    //combobox
    QStringList List;
    List << "cabinet" << "date" << "time";
    ui->comboBox->addItems(List);
    ui->comboBox->setCurrentIndex(1);

    connect(&m_DataBase, SIGNAL(tableModel(QSqlTableModel*)), this, SLOT(lookTable(QSqlTableModel*)));
    connect(m_addeventDialog, SIGNAL(addEventSignal(QString,QString,QString,QString)), &m_DataBase, SLOT(addEvent(QString,QString,QString,QString)));
    connect(m_editeventDialog, SIGNAL(editEventSignal(quint8,QString,QString,QString,QString)), &m_DataBase, SLOT(editEvent(quint8,QString,QString,QString,QString)));
    connect(this, SIGNAL(deleteEventSignal(quint8)), &m_DataBase, SLOT(deleteEvent(quint8)));

    //settings initialization
    QSettings set("settings", QSettings::IniFormat);
    m_settings.TableName = set.value("Database/Tablename", "events").toString();
    m_settings.DBPath = set.value("Database/Path", "ReminderDB").toString();
    m_settings.SoundPath = set.value("Informer/SoundPath", "").toString();
    m_settings.msgPattern = set.value("Informer/MsgPattern", QString::fromLocal8Bit("%1 в %2 надо быть в кабинете %3, заметки: %4")).toString();
    m_settings.playSound = set.value("Informer/Sound", false).toBool();
    m_settings.showDialog = set.value("Informer/Dialog", true).toBool();
    m_settingsDialog->setSettings(m_settings);
    m_informer.setSettings(m_settings);
    m_DataBase.setSettings(m_settings);

    m_DataBase.connectToBase();
    m_DataBase.sendModel();
    ui->tableView->hideColumn(0);
}

MainWindow::~MainWindow()
{
    delete m_addeventDialog;
    delete m_editeventDialog;
    delete m_editAction;
    delete m_deleteAction;
    delete m_settingsDialog;
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
    m_editAction->setEnabled(true);
    m_deleteAction->setEnabled(true);
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
    m_editAction->setEnabled(false);
    m_deleteAction->setEnabled(false);
}

void MainWindow::on_deleteeventButton_clicked()
{
    ui->editeventButton->setEnabled(false);
    ui->deleteeventButton->setEnabled(false);
    m_editAction->setEnabled(false);
    m_deleteAction->setEnabled(false);
    emit deleteEventSignal(m_currentCell.sibling(m_currentCell.row(), 0).data().toUInt());
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->tableView->sortByColumn(index+1, Qt::AscendingOrder);
}
