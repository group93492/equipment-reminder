#include "eventinformer.h"


eventInformer::eventInformer(QObject *parent) :
    QObject(parent)
{
    m_form.setWindowFlags(Qt::ToolTip); //Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint
    m_form.move((QApplication::desktop()->width() - m_form.width()) / 2,
                (QApplication::desktop()->height() - m_form.height()) / 2);
    connect(&m_form, SIGNAL(acceptSignal()), this, SLOT(closeEvent()));
    m_msgPattern = QString::fromLocal8Bit("%1 в %2 надо быть в кабинете %3, заметки: %4");  //NEED FIX!
    m_sound = new QSound("ringin.wav");
    m_showDialog = true;
    m_playSound - true;
}

void eventInformer::showEvent(QList<events> List)
{
    if(m_showDialog)
    {
        QString msgString;
        for(int i = 0; i < List.size(); i++)
        {
            msgString = m_msgPattern
                    .arg(List.value(i).date.toString("dd.MM.yyyy"))
                    .arg(List.value(i).time.toString("hh:mm:ss"))
                    .arg(List.value(i).cabinet)
                    .arg(List.value(i).inf);
            m_form.append(msgString);
            emit deleteEventSignal(List.value(i).id);
        }
        m_form.show();
    }
    if(m_playSound)
        m_sound->play();
}

void eventInformer::closeEvent()
{
    m_sound->stop();
    m_form.clear();
}
