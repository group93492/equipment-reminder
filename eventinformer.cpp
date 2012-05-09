#include "eventinformer.h"


eventInformer::eventInformer(QObject *parent) :
    QObject(parent)
{
    m_form.setWindowFlags(Qt::ToolTip); //Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint
    m_form.move((QApplication::desktop()->width() - m_form.width()) / 2,
                (QApplication::desktop()->height() - m_form.height()) / 2);
    connect(&m_form, SIGNAL(acceptSignal()), this, SLOT(closeEvent()));
    m_sound = new QSound(m_SoundPath);
}

eventInformer::~eventInformer()
{
    delete m_sound;
}

void eventInformer::setSettings(structSettings *s)
{
    m_showDialog = s->showDialog;
    m_playSound = s->playSound;
    m_msgPattern = s->msgPattern;
    if(m_SoundPath != s->SoundPath)
    {
        m_SoundPath = s->SoundPath;
        delete m_sound;
        m_sound = new QSound(m_SoundPath);
    }
}

void eventInformer::showEvent(QList<events> *List)
{
    if(m_showDialog)
    {
        QString msgString;
        for(int i = 0; i < List->size(); i++)
        {
            msgString = m_msgPattern
                    .arg(List->value(i).date.toString("dd.MM.yyyy"))
                    .arg(List->value(i).time.toString("hh:mm:ss"))
                    .arg(List->value(i).cabinet)
                    .arg(List->value(i).inf);
            m_form.append(msgString);
            emit deleteEventSignal(List->value(i).id);
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
