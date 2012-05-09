#ifndef ADDEVENTDIALOG_H
#define ADDEVENTDIALOG_H

#include <QDialog>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QListWidgetItem>

namespace Ui {
class addeventDialog;
}

class addeventDialog : public QDialog
{
    Q_OBJECT
public:
    explicit addeventDialog(QWidget *parent = 0, bool p = true);
    ~addeventDialog();
    
protected:
    virtual void showEvent(QShowEvent * event);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::addeventDialog *ui;
    quint8 m_id;
    bool m_addDialog;

signals:
    void addEventSignal(QString cabinet, QString date, QString time, QString inf);
    void editEventSignal(quint8 id, QString cabinet, QString date, QString time, QString inf);

public slots:
    void setElements(quint8 id, QString cabinet, QString date, QString time, QString inf);
    void changeTime(QListWidgetItem *item);

};

#endif // ADDEVENTDIALOG_H
