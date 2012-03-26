#ifndef ADDEVENTDIALOG_H
#define ADDEVENTDIALOG_H

#include <QDialog>
#include <QDate>
#include <QTime>
#include <QDebug>

namespace Ui {
class addeventDialog;
}

class addeventDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit addeventDialog(QWidget *parent = 0);
    ~addeventDialog();
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::addeventDialog *ui;
signals:
    void event(QString cabinet, QString date, QString time, QString inf);
};

#endif // ADDEVENTDIALOG_H
