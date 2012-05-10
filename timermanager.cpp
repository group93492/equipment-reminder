#include "timermanager.h"

TimerManager::TimerManager(QObject *parent) :
    QObject(parent)
{
    m_currentTimer.setSingleShot(true);
    connect(&m_currentTimer, SIGNAL(timeout()), this, SLOT(sendEventOccuredSignal()));
}

void TimerManager::updateTimer(QList<events> *List)
{
    m_currentEventList = List;
    //it is not important which event is taken because they all have the same time of occured
    qint64 timerTime = QDateTime::currentDateTime().msecsTo(QDateTime(List->at(0).date, List->at(0).time));
    if(timerTime < 0)
    {
        if((-1 * timerTime) < AntiLagTimeMsec)
        {
            sendEventOccuredSignal();
            qDebug() << tr("\"lag\" event occured");
            return;
        }
        else
        {
            qDebug() << tr("timer time cannot be negative");
            return;
        }
    }
    m_currentTimer.start(timerTime);
    qDebug() << tr("timer started in ") << timerTime << tr(" msecs");
}

void TimerManager::sendEventOccuredSignal()
{
    emit eventOccured(m_currentEventList);
    qDebug() << tr("event occured");
}
