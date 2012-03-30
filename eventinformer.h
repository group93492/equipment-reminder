#ifndef EVENTINFORMER_H
#define EVENTINFORMER_H

#include <QObject>
#include "dbmanager.h"

class eventInformer : public QObject
{
    Q_OBJECT
private:
    bool m_showDialog;
    bool m_playSound;
    QString m_SoundPath;
public:
    explicit eventInformer(QObject *parent = 0);
    
signals:
    void deleteEventSignal(quint8 id);
    void editEventSignal(quint8 id, QString cabinet, QString date, QString time, QString inf);
public slots:
    void showEvent(QList<events> List);
};

#endif // EVENTINFORMER_H
