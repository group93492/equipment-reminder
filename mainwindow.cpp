#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QIcon icon("icon.png");
    m_tray = new QSystemTrayIcon(icon);
    structSettings *settings;
    m_addeventDialog = new addeventDialog(this, true);
    m_editeventDialog = new addeventDialog(this, false);
    m_settingsDialog = new settingsDialog(this);
    settings = new structSettings;
    //create tray menu
    QMenu *trayMenu = new QMenu;
    trayMenu->addAction(tr("Show window"), this, SLOT(hideTray()));
    trayMenu->addSeparator();
    trayMenu->addAction(tr("Add event"), this, SLOT(on_addeventButton_clicked()));
    trayMenu->addSeparator();
    trayMenu->addAction(tr("Exit"), qApp, SLOT(quit()));
    m_tray->setContextMenu(trayMenu);
    //create menu
    QMenu *menuGeneral;
    QMenu *menuSettings;
    QMenu *menuAbout;
    menuGeneral = ui->menuBar->addMenu(tr("Common"));
    menuSettings = ui->menuBar->addMenu(tr("Settings"));
    menuAbout = ui->menuBar->addMenu(tr("About"));
    //general menu
    menuGeneral->addAction(tr("Add event..."), this, SLOT(on_addeventButton_clicked()));
    m_editAction = menuGeneral->addAction(tr("Edit event..."), this, SLOT(on_editeventButton_clicked()));
    m_editAction->setEnabled(false);
    m_deleteAction = menuGeneral->addAction(tr("Delete event"), this, SLOT(on_deleteeventButton_clicked()));
    m_deleteAction->setEnabled(false);
    menuGeneral->addSeparator();
    menuGeneral->addAction(tr("Exit"), qApp, SLOT(quit()));
    //settings menu
    menuSettings->addAction(tr("Settings..."), m_settingsDialog, SLOT(show()));
    //about menu
    menuAbout->addAction(tr("About Qt"), qApp, SLOT(aboutQt()));
    //combobox
    QStringList List;
    List << tr("cabinet") << tr("date") << tr("time");
    ui->comboBox->addItems(List);
    ui->comboBox->setCurrentIndex(1);
    //connect section
    connect(&m_DataBase, SIGNAL(tableModel(QSqlTableModel*)), this, SLOT(lookTable(QSqlTableModel*)));
    connect(m_addeventDialog, SIGNAL(addEventSignal(QString,QString,QString,QString)), &m_DataBase, SLOT(addEvent(QString,QString,QString,QString)));
    connect(m_editeventDialog, SIGNAL(editEventSignal(quint8,QString,QString,QString,QString)), &m_DataBase, SLOT(editEvent(quint8,QString,QString,QString,QString)));
    connect(this, SIGNAL(deleteEventSignal(quint8)), &m_DataBase, SLOT(deleteEvent(quint8)));
    connect(m_settingsDialog, SIGNAL(settingsSignal(structSettings*)), &m_DataBase, SLOT(setSettings(structSettings*)));
    connect(m_settingsDialog, SIGNAL(settingsSignal(structSettings*)), &m_eventInformer, SLOT(setSettings(structSettings*)));
    connect(ui->quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(m_tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayActivate(QSystemTrayIcon::ActivationReason)));
    connect(&m_eventInformer, SIGNAL(deleteEventSignal(quint8)), &m_DataBase, SLOT(deleteEvent(quint8)));
    //timer manager
    connect(&m_DataBase, SIGNAL(comingEvents(QList<events>*)), &m_timerManager, SLOT(updateTimer(QList<events>*)));
    connect(&m_timerManager, SIGNAL(eventOccured(QList<events>*)), &m_eventInformer, SLOT(showEvent(QList<events>*)));
    //settings initialization
    QSettings set("settings", QSettings::IniFormat);
    settings->TableName = set.value("Database/Tablename", "events").toString();
    settings->DBPath = set.value("Database/Path", "ReminderDB").toString();
    settings->SoundPath = set.value("Informer/SoundPath", "").toString();
    settings->msgPattern = set.value("Informer/MsgPattern", tr("%1 in %2 have to be in cabinet %3, notes: %4")).toString();
    settings->playSound = set.value("Informer/Sound", false).toBool();
    settings->showDialog = set.value("Informer/Dialog", true).toBool();
    m_settingsDialog->setSettings(settings);
    m_eventInformer.setSettings(settings);
    m_DataBase.setSettings(settings);
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
    delete m_tray;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_tray->setVisible(true);
    this->hide();
    event->accept();
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

void MainWindow::trayActivate(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick)
    {
        this->show();
        m_tray->hide();
    }
}

void MainWindow::hideTray()
{
    m_tray->hide();
    this->show();
}
