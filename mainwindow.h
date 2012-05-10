#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include "dbmanager.h"
#include "addeventdialog.h"
#include "eventinformer.h"
#include "settingsdialog.h"
#include "timermanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    addeventDialog *m_addeventDialog;
    addeventDialog *m_editeventDialog;
    settingsDialog *m_settingsDialog;
    QAction *m_editAction;
    QAction *m_deleteAction;
    QSystemTrayIcon *m_tray;
    QModelIndex m_currentCell;
    eventInformer m_eventInformer;
    TimerManager m_timerManager;
    DBManager m_DataBase;

protected:
    virtual void closeEvent(QCloseEvent *event);

private slots:
    void lookTable(QSqlTableModel *model);
    void on_addeventButton_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    void on_editeventButton_clicked();
    void on_deleteeventButton_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void trayActivate(QSystemTrayIcon::ActivationReason reason);
    void hideTray();
    void on_languageChangeRussianAction_clicked();
    void on_languageChangeEnglishAction_clicked();

signals:
    void deleteEventSignal(quint8 id);

};

#endif // MAINWINDOW_H
