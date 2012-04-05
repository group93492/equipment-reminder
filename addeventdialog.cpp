#include "addeventdialog.h"
#include "ui_addeventdialog.h"

addeventDialog::addeventDialog(QWidget *parent, bool p) :
    QDialog(parent),
    ui(new Ui::addeventDialog)
{
    ui->setupUi(this);
    QStringList List;
    List << "08:05 - 09:40" //maybe read from file?
         << "09:50 - 11:25"
         << "11:45 - 13:20"
         << "13:30 - 15:05"
         << "15:15 - 16:50"
         << "17:00 - 18:35"
         << "18:45 - 20:20";
    ui->listWidget->addItems(List);
    ui->dateEdit->setDate(QDate::currentDate());
    m_addDialog = p;
    if(p)
        this->setWindowTitle(QString::fromLocal8Bit("Добавить событие"));
    else
        this->setWindowTitle(QString::fromLocal8Bit("Редактировать событие"));
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(changeTime(QListWidgetItem*)));
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

void addeventDialog::changeTime(QListWidgetItem *item)
{
    ui->timeEdit->setTime(QTime::fromString(item->text().left(5), "hh:mm"));
}
