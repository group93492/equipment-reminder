#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H
#include <QObject>
#include <QTimer>
#include "dbmanager.h"

class TimerManager : public QObject
{
    Q_OBJECT
private:
    enum {
        AntiLagTimeMsec = 200, //if there is some lags in system, we can get a
//delay while sending a signal with list of feature events and when we will try
//to process this event, the time will take place during the lag
//to avoid this situation, we alsa process the events which occured time was during
//last AntiLagTimeMsec mseconds.
    };
    QList<events> *m_currentEventList;
    QTimer m_currentTimer;

public:
    explicit TimerManager(QObject *parent = 0);
    
signals:
    void eventOccured(QList<events> *List);
    
private slots:
    void sendEventOccuredSignal();

public slots:
    void updateTimer(QList<events> *List);

};

#endif // TIMERMANAGER_H
