#pragma once
#include <qobject.h>

#include "Data.h"
#include "sqliteManage.h"

class essentialLogic : public QObject
{
	Q_OBJECT
public:
	essentialLogic(QObject* parent = nullptr);
	~essentialLogic();

	void InsertPatientInfoData(baseInformation& baseInfo);
	void QueryPatientInfoData(baseInformation* info);
	void UpdataBaseInfo(baseInformation& baseInfo);

	void SetBaseID(int baseID);
	int GetBaseID();

private:
	int m_baseID;
};

