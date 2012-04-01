#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QFile>
#include <QtSql>
#include <QDebug>
#include "settingsdialog.h"

struct events{
    quint8 id;
    QString cabinet;
    QDate date;
    QTime time;
    QString inf;
};

class DBManager : public QObject
{
    Q_OBJECT
private:
    QString m_TableName;
    QString m_DBName;
    QSqlDatabase m_DataBase;
    quint8 m_id;
public:
    explicit DBManager(QObject *parent = 0);
    void connectToBase();
    void setSettings(structSettings s);
signals:
    void tableModel(QSqlTableModel *model);
public slots:
    void sendModel();
    void addEvent(QString cabinet, QString date, QString time, QString inf);
    void editEvent(quint8 id, QString cabinet, QString date, QString time, QString inf);
    void deleteEvent(quint8 id);
    void findComingEvents();
signals:
    void comingEvents(QList<events> List);
};

#endif // DBMANAGER_H
