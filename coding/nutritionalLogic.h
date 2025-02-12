#pragma once
#include <qobject.h>
#include <qmap.h>

#include "Data.h"

enum nutritionalEnum
{
	EN = 10,
	PN,
	IM
};

class nutritionalLogic : public QObject 
{
	Q_OBJECT
public:
	nutritionalLogic();
	~nutritionalLogic();

	void GetENPageData(QVector<nutritionalInformation>& vec);
	void GetPNPageData(QVector<nutritionalInformation>& vec);
	void GetOFPageData(QVector<nutritionalInformation>& vec);


	void insert(const QString key, const nutritionalInformation& data, const nutritionalEnum type);
	void remove(const QString& title, const nutritionalEnum type);
	bool isHave(const QString& title, const nutritionalEnum type);
	int size(const nutritionalEnum type);

	void CheckZero(nutritionalInformation& data);
private:
	QMap<QString, nutritionalInformation> m_ENMap;
	QMap<QString, nutritionalInformation> m_PNMap;
	QMap<QString, nutritionalInformation> m_OFMap;
};

