#pragma once
#include <qobject.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QDebug>

class sqliteManage :public QObject
{
	Q_OBJECT
private:
	static sqliteManage* instance;
	sqliteManage();
	~sqliteManage();

public:
	static sqliteManage* getInstance();
	QSqlDatabase* GetDb();
private:
	QSqlDatabase m_db;
};

