#include "addeventdialog.h"
#include "ui_addeventdialog.h"

addeventDialog::addeventDialog(QWidget *parent, bool p) :
    QDialog(parent),
    ui(new Ui::addeventDialog)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
    m_addDialog = p;
    if(p)
        this->setWindowTitle(QString::fromLocal8Bit("Добавить событие"));
    else
        this->setWindowTitle(QString::fromLocal8Bit("Редактировать событие"));
}

addeventDialog::~addeventDialog()
{
    delete ui;
}

void addeventDialog::on_buttonBox_accepted()
{
    QString cabinet = ui->cabinetEdit->text();
    QString date = ui->dateEdit->date().toString("dd.MM.yyyy");
    QString time = ui->timeEdit->time().toString("hh:mm:ss");
    QString inf = ui->notesEdit->document()->toPlainText();
    if(m_addDialog)
        emit addEventSignal(cabinet, date, time, inf);
    else
        emit editEventSignal(m_id, cabinet, date, time, inf);
}

void addeventDialog::setElements(quint8 id, QString cabinet, QString date, QString time, QString inf)
{
    m_id = id;
    ui->cabinetEdit->setText(cabinet);
    ui->dateEdit->setDate(QDate::fromString(date, "dd.MM.yyyy"));
    ui->timeEdit->setTime(QTime::fromString(time, "hh:mm:ss"));
    ui->notesEdit->setText(inf);
}
