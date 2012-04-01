#ifndef EVENTINFORMER_H
#define EVENTINFORMER_H

#include <QObject>
#include <QApplication>
#include <QtGui>
#include <QString>
#include "dbmanager.h"
#include "eventform.h"
#include "settingsdialog.h"

class eventInformer : public QObject
{
    Q_OBJECT
private:
    bool m_showDialog;
    bool m_playSound;
    QString m_msgPattern;
    QString m_SoundPath;
    QSound *m_sound;
    eventForm m_form;
public:
    explicit eventInformer(QObject *parent = 0);
signals:
    void deleteEventSignal(quint8 id);
public slots:
    void showEvent(QList<events> List);
    void closeEvent();
    void setSettings(structSettings s);
};

#endif // EVENTINFORMER_H
