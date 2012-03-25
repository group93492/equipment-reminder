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
                    "number INTEGER, "
                    "cabinet INTEGER, "
                    "date INTEGER, "
                    "time INTEGER, "
                    "inf TEXT "
                    ");";
        query.exec(str);
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


