#pragma once
#include <qobject.h>
#include <QDir>

#include "Data.h"
#include "sqliteManage.h"
//#include "essentialinformation.h"
//#include "clinicalinformation.h"
//#include "nutritionalinformation.h"

class BaseInfoLogic : public QObject
{
	Q_OBJECT
public:
	BaseInfoLogic(QObject*  parent = nullptr);
	~BaseInfoLogic();

	bool newClinicalRecord(int& baseID, int& clid, const clinicalInformation& clInfo, QVector<nutritionalInformation> nuInfo);

	void QueryClinicalData(int& baseID, QVector<collectionData>& data);
	void DeletePatientData(QString& rootPath, int baseID,int clinicalID);
private:
	sqliteManage* m_sql;
};

