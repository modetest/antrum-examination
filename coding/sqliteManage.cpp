#include "sqliteManage.h"

sqliteManage* sqliteManage::instance = nullptr;

sqliteManage::sqliteManage()
{
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        m_db = QSqlDatabase::database("qt_sql_default_connection");
        qDebug() << "qt_sql_default_connection";
    }
    else {
        m_db = QSqlDatabase::addDatabase("QSQLITE");

        m_db.setDatabaseName("./db/ultrasound.db");     // 设置数据库名称 
    }
    if (!m_db.open()) {
        qDebug() << "database open error：" << m_db.lastError();
    }
    else {
        qDebug() << "database ok!";
    }
   // qDebug() << "this DB hasFeature:Transaction:" << m_db.driver()->hasFeature(QSqlDriver::Transactions);
    
}

sqliteManage::~sqliteManage()
{
    qDebug() << "db close!";
    m_db.close();
}

sqliteManage* sqliteManage::getInstance()
{
    if (instance == nullptr) {
        instance = new sqliteManage();
    }
    return instance;
}

QSqlDatabase* sqliteManage::GetDb()
{
    return &m_db;
}




