#include "dbmanager.h"

DBManager::DBManager(QObject *parent) :
    QObject(parent)
{
    m_DBName = "ReminderDB";
    m_TableName = "Events";
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
        QSqlQuery query;
        QString str = "CREATE TABLE " + m_TableName + " ( "
                    "cabinet TEXT, "
                    "date TEXT, "
                    "time TEXT, "
                    "inf TEXT "
                    ");";
        if(!query.exec(str))
            qDebug() << "Invalid sql query";
    }
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
    QSqlQuery query;
    QString str = QString("INSERT INTO %1 (cabinet, date, time, inf)"
                          "VALUES ('%2', '%3', '%4', '%5');")
            .arg(m_TableName)
            .arg(cabinet)
            .arg(date)
            .arg(time)
            .arg(inf);
    if(!query.exec(str))
        qDebug() << "Invalid sql query";
    sendModel();
}


