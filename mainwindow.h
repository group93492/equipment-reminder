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
    addeventDialog *m_eventDialog;
private slots:
    void lookTable(QSqlTableModel *model);
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
