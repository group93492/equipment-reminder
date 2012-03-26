#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QFile>
#include <QtSql>
#include <QDebug>

class DBManager : public QObject
{
    Q_OBJECT
private:
    QString m_TableName;
    QString m_DBName;
    QSqlDatabase m_DataBase;
public:
    explicit DBManager(QObject *parent = 0);
    void connectToBase();
signals:
    void tableModel(QSqlTableModel *model);
public slots:
    void sendModel();
    void addEvent(QString cabinet, QString date, QString time, QString inf);
};

#endif // DBMANAGER_H
