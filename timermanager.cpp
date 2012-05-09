#include "timermanager.h"

TimerManager::TimerManager(QObject *parent) :
    QObject(parent),
    m_currentTimer(this)
{
    m_currentTimer.setSingleShot(true);
}

void TimerManager::updateTimer(QList<events> *List)
{
    m_currentEventList = List;
    connect(&m_currentTimer, SIGNAL(timeout()), this, SLOT(sendEventOccuredSignal()));
    //it is not important which event is taken because they all have the same time of occured
    qint64 timerTime = QDateTime::currentDateTime().msecsTo(QDateTime(List->at(0).date, List->at(0).time));
    if(timerTime < 0 && (-1 * timerTime) < AntiLagTimeMsec)
    {
        sendEventOccuredSignal();
        qDebug() << "\"lag\" event occured";
        return;
    }
    m_currentTimer.start(timerTime);
    qDebug() << "timer started in " << timerTime << " msecs";
}

void TimerManager::sendEventOccuredSignal()
{
    emit eventOccured(m_currentEventList);
    qDebug() << "event occured";
}
