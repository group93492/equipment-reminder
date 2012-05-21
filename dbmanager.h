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
    void checkForWrongRecords();
    enum sendModelType {
        enumShowAllEvents = 0,
        enumShowOnlyOccuredEvents = 1,
        enumShowNotOccuredEvents = 2
    } m_sendModelType;
    void sendModel(const QSqlQuery &query);

public:
    explicit DBManager(QObject *parent = 0);
    void connectToBase();
    void sendModel();

signals:
    void tableModel(QAbstractTableModel *model);
    void comingEvents(QList<events> *List);

public slots:
    void setSettings(structSettings *s);
    void addEvent(QString cabinet, QString date, QString time, QString inf);
    void editEvent(quint8 id, QString cabinet, QString date, QString time, QString inf);
    void deleteEvent(quint8 id);
    void markAsOccured(quint8 id);
    void showAllEvents();
    void showOnlyOccuredEvents();
    void showNotOccuredEvents();
    void findComingEvents();

};

#endif // DBMANAGER_H
