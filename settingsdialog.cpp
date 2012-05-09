#include "settingsdialog.h"
#include "ui_settingsdialog.h"

settingsDialog::settingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingsDialog)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("icon.png"));
    setWindowTitle(QString::fromLocal8Bit("Настройки"));
    m_currentSettings = new structSettings;
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
    delete m_currentSettings;
    delete ui;
}

void settingsDialog::setSettings(structSettings *s)
{
    m_currentSettings = s;
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
    m_currentSettings->TableName = ui->dbTableLineEdit->text();
    m_currentSettings->DBPath = ui->dbPathLineEdit->text();
    m_currentSettings->SoundPath = ui->soundPathEdit->text();
    m_currentSettings->msgPattern = ui->patternLineEdit->text();
    m_currentSettings->playSound = ui->soundCheckBox->isChecked();
    m_currentSettings->showDialog = ui->messageCheckBox->isChecked();
    settings.setValue("Database/Tablename", m_currentSettings->TableName);
    settings.setValue("Database/Path", m_currentSettings->DBPath);
    settings.setValue("Informer/SoundPath", m_currentSettings->SoundPath);
    settings.setValue("Informer/MsgPattern", m_currentSettings->msgPattern);
    if(m_currentSettings->playSound)
        settings.setValue("Informer/Sound", "true");
    else
        settings.setValue("Informer/Sound", "false");
    if(m_currentSettings->showDialog)
        settings.setValue("Informer/Dialog", "true");
    else
        settings.setValue("Informer/Dialog", "false");
    emit settingsSignal(m_currentSettings);
}

void settingsDialog::on_buttonBox_rejected()
{
    ui->dbTableLineEdit->setText(m_currentSettings->TableName);
    ui->dbPathLineEdit->setText(m_currentSettings->DBPath);
    ui->soundPathEdit->setText(m_currentSettings->SoundPath);
    ui->patternLineEdit->setText(m_currentSettings->msgPattern);
    ui->soundCheckBox->setChecked(m_currentSettings->playSound);
    ui->messageCheckBox->setChecked(m_currentSettings->showDialog);
}
