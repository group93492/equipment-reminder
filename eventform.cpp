#include "eventform.h"
#include "ui_eventform.h"

eventForm::eventForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::eventForm)
{
    ui->setupUi(this);
}

void eventForm::append(QString str)
{
    ui->textBrowser->append(str);
}

void eventForm::clear()
{
    ui->textBrowser->clear();
}

eventForm::~eventForm()
{
    delete ui;
}

void eventForm::on_pushButton_clicked()
{
    close();
}
