#include "nutritionalLogic.h"

nutritionalLogic::nutritionalLogic()
{
}

nutritionalLogic::~nutritionalLogic()
{
}

void nutritionalLogic::GetENPageData(QVector<nutritionalInformation>& vec)
{
	QMap<QString, nutritionalInformation>::iterator itor;
	for (itor = m_ENMap.begin(); itor != m_ENMap.end(); ++itor)
	{
		itor.value().name = u8"EN";
		vec.append(itor.value());
	}
}

void nutritionalLogic::GetPNPageData(QVector<nutritionalInformation>& vec)
{
	QMap<QString, nutritionalInformation>::iterator itor;
	for (itor = m_PNMap.begin(); itor != m_PNMap.end(); ++itor)
	{
		itor.value().name = u8"PN";
		vec.append(itor.value());
	}
}
void nutritionalLogic::GetOFPageData(QVector<nutritionalInformation>& vec)
{
	QMap<QString, nutritionalInformation>::iterator itor;
	for (itor = m_OFMap.begin(); itor != m_OFMap.end(); ++itor)
	{
		itor.value().name = u8"经口进食";
		vec.append(itor.value());
	}
}

void nutritionalLogic::insert(const QString key, const nutritionalInformation& data, const nutritionalEnum type)
{
	switch (type)
	{
	case EN:
		m_ENMap.insert(key, data);
		break;
	case PN:
		m_PNMap.insert(key, data);
		break;
	case IM:
		m_OFMap.insert(key, data);
		break;
	default:
		break;
	}
}

void nutritionalLogic::remove(const QString& title, const nutritionalEnum type)
{
	switch (type)
	{
	case EN:
		m_ENMap.remove(title);
		break;
	case PN:
		m_PNMap.remove(title);
		break;
	case IM:
		m_OFMap.remove(title);
		break;
	default:
		break;
	}
}

bool nutritionalLogic::isHave(const QString& title, const nutritionalEnum type)
{
	bool isHad = false;

	switch (type)
	{
	case EN:
		isHad = m_ENMap.contains(title);
		break;
	case PN:
		isHad = m_PNMap.contains(title);
		break;
	case IM:
		isHad = m_OFMap.contains(title);
		break;
	default:
		break;
	}
	return isHad;
}

int nutritionalLogic::size(const nutritionalEnum type)
{
	int len = -1;
	switch (type)
	{
	case EN:
		len = m_ENMap.size();
		break;
	case PN:
		len = m_PNMap.size();
		break;
	case IM:
		len = m_OFMap.size();
		break;
	default:
		break;
	}
	return len;
}

void nutritionalLogic::CheckZero(nutritionalInformation& data)
{
	if (data.Formula == "")
		data.Formula = "none";
	if (data.Volume == "")
		data.Volume = "0";
	if (data.Kcal == "")
		data.Kcal = "0";
	if (data.Protein == "")
		data.Protein = "0";
}
