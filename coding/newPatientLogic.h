#pragma once
#include <qobject.h>

#include "Data.h"
#include "sqliteManage.h"

class newPatientLogic : public QObject
{
public:
	newPatientLogic();
	~newPatientLogic();

	bool newPatientRecord(int& baseID,int& clid, const baseInformation& baseInfo, const clinicalInformation& clInfo, QVector<nutritionalInformation> nuInfo);

private:
	sqliteManage* m_sql;

};