#pragma once

#include <QObject>
#include "Data.h"
#include"sqliteManage.h"

class reportLogic  : public QObject
{
	Q_OBJECT

public:
	reportLogic(QObject *parent);
	~reportLogic();


	void queryData(int baseID, int clID, baseInformation& baseInfo, clinicalInformation& clinInfo, QVector<nutritionalInformation>& nuInfo, collectionData& collectInfo);
};
