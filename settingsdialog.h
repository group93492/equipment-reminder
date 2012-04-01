#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QSound>
#include <QSettings>

struct structSettings
{
    QString TableName;
    QString DBPath;
    QString msgPattern;
    QString SoundPath;
    bool showDialog;
    bool playSound;
};

namespace Ui {
class settingsDialog;
}

class settingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit settingsDialog(QWidget *parent = 0);
    ~settingsDialog();
    void setSettings(structSettings s);
    
private slots:
    void on_soundPathToolButton_clicked();

    void on_dbToolButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::settingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
