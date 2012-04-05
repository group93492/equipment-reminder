#include "dbmanager.h"

DBManager::DBManager(QObject *parent) :
    QObject(parent)
{
    m_DataBase = QSqlDatabase::addDatabase("QSQLITE");
}

void DBManager::connectToBase()
{

    QFile file(m_DBName);
    if(file.exists())
    {
        m_DataBase.setDatabaseName(m_DBName);
        if(!m_DataBase.open())
            qDebug() << m_DataBase.lastError().text();
    }
    else
    {
        m_DataBase.setDatabaseName(m_DBName);
        if(!m_DataBase.open())
            qDebug() << m_DataBase.lastError().text();
        QString str = "CREATE TABLE " + m_TableName + " ( "
                    "id INTEGER, "
                    "cabinet TEXT, "
                    "date TEXT, "
                    "time TEXT, "
                    "inf TEXT "
                    ");";
        QSqlQuery query;
        if(!query.exec(str))
            qDebug() << "Invalid sql query: " << query.lastError().text();
    }
    QSqlQuery query;
    QString str = QString("SELECT MAX(id) FROM %1;")
            .arg(m_TableName);
    if(!query.exec(str))
        qDebug() << "Invalid sql query: " << query.lastError().text();
    QSqlRecord rec;
    query.next();
    rec = query.record();
    m_id = rec.value(0).toUInt() + 1;
    findComingEvents();
}

void DBManager::setSettings(structSettings *s)
{
    if(m_DataBase.isOpen())
        m_DataBase.close();
    m_DBName = s->DBPath;
    m_TableName = s->TableName;
    connectToBase();
    sendModel();
}

void DBManager::sendModel()
{
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable(m_TableName);
    model->select();
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    emit tableModel(model);
}

void DBManager::addEvent(QString cabinet, QString date, QString time, QString inf)
{
    m_id++;
    QSqlQuery query;
    QString str = QString("INSERT INTO %1 (id, cabinet, date, time, inf) "
                          "VALUES ('%2', '%3', '%4', '%5', '%6');")
            .arg(m_TableName)
            .arg(m_id)
            .arg(cabinet)
            .arg(date)
            .arg(time)
            .arg(inf);
    if(!query.exec(str))
        qDebug() << "Invalid sql query: " << query.lastError().text();
    sendModel();
    findComingEvents();
}

void DBManager::editEvent(quint8 id, QString cabinet, QString date, QString time, QString inf)
{
    QSqlQuery query;
    QString str = QString("UPDATE %1 "
                          "SET cabinet = '%2', date = '%3', time = '%4', inf = '%5' "
                          "WHERE id = %6;")
            .arg(m_TableName)
            .arg(cabinet)
            .arg(date)
            .arg(time)
            .arg(inf)
            .arg(id);
    if(!query.exec(str))
        qDebug() << "Invalid sql query: " << query.lastError().text();
    sendModel();
    findComingEvents();
}

void DBManager::deleteEvent(quint8 id)
{
    QSqlQuery query;
    QString str = QString("DELETE FROM %1 "
                          "WHERE id = %2;")
            .arg(m_TableName)
            .arg(id);
    if(!query.exec(str))
        qDebug() << "Invalid sql query: " << query.lastError().text();
    sendModel();
    findComingEvents();
}

void DBManager::findComingEvents()
{
    QSqlQuery query;
    QSqlRecord rec;
    QString currentDate = QDate::currentDate().toString("dd.MM.yyyy");
    QString currentTime = QTime::currentTime().toString("hh:mm:ss");
    //cleaning table of obsolete records
    QString str = QString("DELETE FROM %1 WHERE date < '%2' OR (date = '%2' and time < '%3');")
            .arg(m_TableName)
            .arg(currentDate)
            .arg(currentTime);
    if(!query.exec(str))
        qDebug() << "Invalid sql query: " << query.lastError().text();
    str = QString("SELECT * FROM %1 WHERE "
                  "time = (SELECT MIN(time) FROM %1 WHERE date = (SELECT MIN(date) FROM %1)) and "
                  "date = (SELECT MIN(date) FROM %1);")
            .arg(m_TableName);
    if(!query.exec(str))
        qDebug() << "Invalid sql query: " << query.lastError().text();
    QList<events> List;
    events tempEvent;
    while(query.next())
    {
        rec = query.record();
        tempEvent.id = rec.value(0).toUInt();
        tempEvent.cabinet = rec.value(1).toString();
        tempEvent.date = QDate::fromString(rec.value(2).toString(), "dd.MM.yyyy"); //DONT FIX!
        tempEvent.time = rec.value(3).toTime();
        tempEvent.inf = rec.value(4).toString();
        List.append(tempEvent);
    }
    emit comingEvents(List);
    sendModel();
}
