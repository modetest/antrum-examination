#pragma once
#include <QObject>
#include <QCryptographicHash>// MD5 º”√‹
#include <QSettings>

#include "sqliteManage.h"



class loginLogic : public QObject
{
public:
	loginLogic();
	~loginLogic();

public:
	void readPwd(QString& account, QString& pwd);

	bool login(const bool save, const QString& account, const QString& pwd);

private:
	void savePwd(const QString& account, const QString& pwd);
	QString MD5_encryption(const QString& data);


private:
	sqliteManage* m_sql;
};

