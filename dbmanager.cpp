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
                    "inf TEXT, "
                    "isOccured BOOLEAN"
                    ");";
        QSqlQuery query;
        if(!query.exec(str))
            qDebug() << "Invalid sql query: " << query.lastError().text();
        query.finish();
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
    query.finish();
    checkForWrongRecords();
    findComingEvents();
}

void DBManager::setSettings(structSettings *s)
{
    if(m_DataBase.isOpen())
        m_DataBase.close();
    m_DBName = s->DBPath;
    m_TableName = s->TableName;
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
    QString str = QString("INSERT INTO %1 (id, cabinet, date, time, inf, isOccured) "
                          "VALUES ('%2', '%3', '%4', '%5', '%6', '%7');")
            .arg(m_TableName)
            .arg(m_id)
            .arg(cabinet)
            .arg(date)
            .arg(time)
            .arg(inf)
            .arg(0); //set event as NOT occured
    if(!query.exec(str))
        qDebug() << "Invalid sql query: " << query.lastError().text();
    query.finish();
    sendModel();
    findComingEvents();
}

void DBManager::editEvent(quint8 id, QString cabinet, QString date, QString time, QString inf)
{
    quint8 isOccured = 1;
    if(QDateTime::currentDateTime() <
            QDateTime(QDate::fromString(date, "dd.MM.yyyy"), QTime::fromString(time)))
        isOccured = 0;
    QString str = QString("UPDATE %1 "
                          "SET cabinet = '%2', date = '%3', time = '%4', "
                          "inf = '%5', isOccured = '%7' "
                          "WHERE id = '%6';")
            .arg(m_TableName)
            .arg(cabinet)
            .arg(date)
            .arg(time)
            .arg(inf)
            .arg(id)
            .arg(isOccured);
    QSqlQuery query;
    if(!query.exec(str))
        qDebug() << "Invalid sql query: " << query.lastError().text();
    query.finish();
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
    query.finish();
    sendModel();
    findComingEvents();
}

void DBManager::markAsOccured(quint8 id)
{
    QSqlQuery query;
    QString str = QString("UPDATE %1 SET isOccured = '1' WHERE id = '%2';")
            .arg(m_TableName)
            .arg(id);
    if(!query.exec(str))
        qDebug() << "Invalid sql query: " << query.lastError().text();
    query.finish();
    sendModel();
    findComingEvents();
}

void DBManager::findComingEvents()
{
    QSqlQuery query;
    if(!query.exec("DROP TABLE IF EXISTS temp;"))
        qDebug() << "1Invalid sql query: " << query.lastError().text();
    query.finish();
    QString str = QString("CREATE TABLE temp AS SELECT * FROM '%1' WHERE isOccured = 0;")
            .arg(m_TableName);
    if(!query.exec(str))
        qDebug() << "2Invalid sql query: " << query.lastError().text();
    query.finish();
    str = QString("SELECT * FROM temp WHERE "
                                      "time = (SELECT MIN(time) FROM temp WHERE "
                                            "date = (SELECT MIN(date) FROM temp)) and "
                                            "date = (SELECT MIN(date) FROM temp);");

    if(!query.exec(str))
        qDebug() << "3Invalid sql query: " << query.lastError().text();
    QList<events> *List = new QList<events>;
    events tempEvent;
    QSqlRecord rec;
    while(query.next())
    {
        rec = query.record();
        tempEvent.id = rec.value(0).toUInt();
        tempEvent.cabinet = rec.value(1).toString();
        //to.string return wrong type date and we need to convert it manually
        tempEvent.date = QDate::fromString(rec.value(2).toString(), "dd.MM.yyyy");
        tempEvent.time = rec.value(3).toTime();
        tempEvent.inf = rec.value(4).toString();
        List->append(tempEvent);
    }
    if(!query.exec("DROP TABLE IF EXISTS temp;"))
        qDebug() << "4Invalid sql query: " << query.lastError().text();
    query.finish();
    if(!List->empty())
        emit comingEvents(List);
    sendModel();
}

quint32 DBManager::checkForWrongRecords()
{
    //check for future events but marked as occured and versa versa
    QString currentDate = QDate::currentDate().toString("dd.MM.yyyy");
    QString currentTime = QTime::currentTime().toString("hh:mm:ss");
    QString str = QString("UPDATE %1 SET isOccured = '1' "
                          "WHERE date < '%2' OR (date = '%2' and time < '%3');")
            .arg(m_TableName)
            .arg(currentDate)
            .arg(currentTime);
    QSqlQuery query;
    if(!query.exec(str))
        qDebug() << "Invalid sql query: " << query.lastError().text();
    query.finish();
    str = QString("UPDATE %1 SET isOccured = '0' "
                  "WHERE date > '%2' OR (date = '%2' and time > '%3');")
            .arg(m_TableName)
            .arg(currentDate)
            .arg(currentTime);
    if(!query.exec(str))
        qDebug() << "Invalid sql query: " << query.lastError().text();
    query.finish();
    sendModel();
}
