#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QIcon icon("icon.png");
    setWindowIcon(icon);
    setWindowTitle("Equipment Reminder");
    m_tray = new QSystemTrayIcon(icon);
    structSettings *settings;
    m_addeventDialog = new addeventDialog(this, true);
    m_editeventDialog = new addeventDialog(this, false);
    m_settingsDialog = new settingsDialog(this);
    settings = new structSettings;
    //create tray menu
    QMenu *trayMenu = new QMenu;
    trayMenu->addAction(QString::fromLocal8Bit("Показать окно"), this, SLOT(hideTray()));
    trayMenu->addSeparator();
    trayMenu->addAction(QString::fromLocal8Bit("Добавить событие"), this, SLOT(on_addeventButton_clicked()));
    trayMenu->addSeparator();
    trayMenu->addAction(QString::fromLocal8Bit("Выход"), qApp, SLOT(quit()));
    m_tray->setContextMenu(trayMenu);
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
    //connect section
    connect(&m_DataBase, SIGNAL(tableModel(QAbstractTableModel*)), this, SLOT(lookTable(QAbstractTableModel*)));
    connect(m_addeventDialog, SIGNAL(addEventSignal(QString,QString,QString,QString)), &m_DataBase, SLOT(addEvent(QString,QString,QString,QString)));
    connect(m_editeventDialog, SIGNAL(editEventSignal(quint8,QString,QString,QString,QString)), &m_DataBase, SLOT(editEvent(quint8,QString,QString,QString,QString)));
    connect(this, SIGNAL(deleteEventSignal(quint8)), &m_DataBase, SLOT(deleteEvent(quint8)));
    connect(m_settingsDialog, SIGNAL(settingsSignal(structSettings*)), &m_DataBase, SLOT(setSettings(structSettings*)));
    connect(m_settingsDialog, SIGNAL(settingsSignal(structSettings*)), &m_eventInformer, SLOT(setSettings(structSettings*)));
    connect(ui->quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(m_tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayActivate(QSystemTrayIcon::ActivationReason)));
    connect(&m_eventInformer, SIGNAL(eventOccured(quint8)), &m_DataBase, SLOT(markAsOccured(quint8)));
    //connect change showed events (all/not occured/occured)
    connect(this, SIGNAL(showAllEvents()), &m_DataBase, SLOT(showAllEvents()));
    connect(this, SIGNAL(showNotOccuredEvents()), &m_DataBase, SLOT(showNotOccuredEvents()));
    connect(this, SIGNAL(showOnlyOccuredEvents()), &m_DataBase, SLOT(showOnlyOccuredEvents()));
    //timer manager
    connect(&m_DataBase, SIGNAL(comingEvents(QList<events>*)), &m_timerManager, SLOT(updateTimer(QList<events>*)));
    connect(&m_timerManager, SIGNAL(eventOccured(QList<events>*)), &m_eventInformer, SLOT(showEvent(QList<events>*)));
    //settings initialization
    QSettings set("settings", QSettings::IniFormat);
    settings->TableName = set.value("Database/Tablename", "events").toString();
    settings->DBPath = set.value("Database/Path", "ReminderDB").toString();
    settings->SoundPath = set.value("Informer/SoundPath", "").toString();
    settings->msgPattern = set.value("Informer/MsgPattern", QString::fromLocal8Bit("%1 в %2 надо быть в кабинете %3, заметки: %4")).toString();
    settings->playSound = set.value("Informer/Sound", false).toBool();
    settings->showDialog = set.value("Informer/Dialog", true).toBool();
    m_settingsDialog->setSettings(settings);
    m_eventInformer.setSettings(settings);
    m_DataBase.setSettings(settings);
    m_DataBase.connectToBase();
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

void MainWindow::lookTable(QAbstractTableModel *model)
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

void MainWindow::on_showOccuredCheckBox_clicked()
{
    static Qt::CheckState previousCheckState = Qt::Checked;
    if(ui->showOccuredCheckBox->checkState() != previousCheckState)
        switch(ui->showOccuredCheckBox->checkState())
        {
        case Qt::Checked://show all events
            emit showAllEvents();
            previousCheckState = Qt::Checked;
            break;
        case Qt::PartiallyChecked://show only future events
            emit showNotOccuredEvents();
            previousCheckState = Qt::PartiallyChecked;
            break;
        case Qt::Unchecked://show only occured events
            emit showOnlyOccuredEvents();
            previousCheckState = Qt::Unchecked;
            break;
        }
}
