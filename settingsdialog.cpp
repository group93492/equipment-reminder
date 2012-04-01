#include "settingsdialog.h"
#include "ui_settingsdialog.h"

settingsDialog::settingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingsDialog)
{
    ui->setupUi(this);
    if(!QSound::isAvailable())
    {
        ui->soundPathEdit->setEnabled(false);
        ui->soundPathToolButton->setEnabled(false);
        ui->soundCheckBox->setChecked(false);
        ui->soundCheckBox->setEnabled(false);
        ui->soundErrorLabel->setText(QString::fromLocal8Bit("Проблемы со звуковой системой. Возможно не установлен NAS"));
    }
}

settingsDialog::~settingsDialog()
{
    delete ui;
}

void settingsDialog::setSettings(structSettings *s)
{
    ui->soundPathEdit->setText(s->SoundPath);
    ui->soundCheckBox->setChecked(s->playSound);
    ui->messageCheckBox->setChecked(s->showDialog);
    ui->patternLineEdit->setText(s->msgPattern);
    ui->dbPathLineEdit->setText(s->DBPath);
    ui->dbTableLineEdit->setText(s->TableName);
}

void settingsDialog::on_soundPathToolButton_clicked()
{
    if(QSound::isAvailable())
    {
        QString path;
        path = QFileDialog::getOpenFileName(this, "Open sound", QDir::currentPath(), "Sound (*.wav)");
        if(!path.isEmpty())
            ui->soundPathEdit->setText(path);
    }
}

void settingsDialog::on_dbToolButton_clicked()
{
    QString path;
    path = QFileDialog::getOpenFileName(this, "Open DB", QDir::currentPath());
    if(!path.isEmpty())
        ui->dbPathLineEdit->setText(path);
}

void settingsDialog::on_buttonBox_accepted()
{
    QSettings settings("settings", QSettings::IniFormat);
    structSettings *s;
    s = new structSettings;
    s->TableName = ui->dbTableLineEdit->text();
    s->DBPath = ui->dbPathLineEdit->text();
    s->SoundPath = ui->soundPathEdit->text();
    s->msgPattern = ui->patternLineEdit->text();
    s->playSound = ui->soundCheckBox->isChecked();
    s->showDialog = ui->messageCheckBox->isChecked();
    settings.setValue("Database/Tablename", s->TableName);
    settings.setValue("Database/Path", s->DBPath);
    settings.setValue("Informer/SoundPath", s->SoundPath);
    settings.setValue("Informer/MsgPattern", s->msgPattern);
    if(s->playSound)
        settings.setValue("Informer/Sound", "true");
    else
        settings.setValue("Informer/Sound", "false");
    if(s->showDialog)
        settings.setValue("Informer/Dialog", "true");
    else
        settings.setValue("Informer/Dialog", "false");
    emit settingsSignal(s);
}
