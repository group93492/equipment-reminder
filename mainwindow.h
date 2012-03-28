#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <dbmanager.h>
#include <addeventdialog.h>

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
    DBManager m_DataBase;
    addeventDialog *m_addeventDialog;
    addeventDialog *m_editeventDialog;
    QModelIndex m_currentCell;
private slots:
    void lookTable(QSqlTableModel *model);
    void on_addeventButton_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    void on_editeventButton_clicked();
    void on_deleteeventButton_clicked();
signals:
    void deleteEventSignal(quint8 id);
};

#endif // MAINWINDOW_H
