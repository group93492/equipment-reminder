#include "addeventdialog.h"
#include "ui_addeventdialog.h"

addeventDialog::addeventDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addeventDialog)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
    ui->timeEdit->setTime(QTime::currentTime());
}

addeventDialog::~addeventDialog()
{
    delete ui;
}

void addeventDialog::on_buttonBox_accepted()
{
    QString cabinet = ui->lineEdit->text();
    QString date = ui->dateEdit->date().toString("dd.MM.yyyy");
    QString time = ui->timeEdit->time().toString("hh:mm:ss");
    QString inf = ui->textEdit_2->document()->toPlainText();
    emit event(cabinet, date, time, inf);
}
