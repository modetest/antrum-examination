#include "clinicalLogic.h"

clinicalLogic::clinicalLogic(QObject* parent):QObject(parent)
{

}

clinicalLogic::~clinicalLogic()
{

}

void clinicalLogic::insert(const QString& title, breathe& data)
{
	m_MaphreathData.insert(title, data);
}

void clinicalLogic::insert(const QString& title, medicine& data, const BreatheOrMedicType type)
{


	switch (type)
	{
	case VASOACTIVITY:
		m_MapVasoactivityData.insert(title, data);
		break;
	case ANALGESIA:
		m_MapAnalgeiaData.insert(title, data);
		break;
	case CALM:
		m_MapCalmData.insert(title, data);
		break;
	case MUSCLERELAXATION:
		m_MapMuscleRelaxationData.insert(title, data);
		break;
	default:
		break;
	}
}

void clinicalLogic::remove(const QString& title,const BreatheOrMedicType type)
{
	switch (type)
	{
	case BREATHE:
		m_MaphreathData.remove(title);
		break;
	case VASOACTIVITY:
		m_MapVasoactivityData.remove(title);
		break;
	case ANALGESIA:
		m_MapAnalgeiaData.remove(title);
		break;
	case CALM:
		m_MapCalmData.remove(title);
		break;
	case MUSCLERELAXATION:
		m_MapMuscleRelaxationData.remove(title);
		break;
	default:
		break;
	}
}

bool clinicalLogic::isHave(const QString& title,const BreatheOrMedicType type)
{
	bool isHad = false;
	switch (type)
	{
	case BREATHE:
		isHad = m_MaphreathData.contains(title);
		break;
	case VASOACTIVITY:
		isHad = m_MapVasoactivityData.contains(title);
		break;
	case ANALGESIA:
		isHad = m_MapAnalgeiaData.contains(title);
		break;
	case CALM:
		isHad = m_MapCalmData.contains(title);
		break;
	case MUSCLERELAXATION:
		isHad = m_MapMuscleRelaxationData.contains(title);
		break;
	default:
		break;
	}
	return isHad;
}

int clinicalLogic::size(const BreatheOrMedicType type)
{
	int len = -1;
	switch (type)
	{
	case BREATHE:
		len = m_MaphreathData.size();
		break;
	case VASOACTIVITY:
		len = m_MapVasoactivityData.size();
		break;
	case ANALGESIA:
		len = m_MapAnalgeiaData.size();
		break;
	case CALM:
		len = m_MapCalmData.size();
		break;
	case MUSCLERELAXATION:
		len = m_MapMuscleRelaxationData.size();
		break;
	default:
		break;
	}
	
	return len;
}

void clinicalLogic::GetHreatheData(QVector<breathe>& vec)
{
	vec.clear();
	QMap<QString, breathe>::iterator itor;
	for (itor = m_MaphreathData.begin(); itor != m_MaphreathData.end(); ++itor)
	{
		itor.value().pattern = itor.key();
		vec.append(itor.value());
	}
}

void clinicalLogic::GetVasoactivityData(QVector<medicine>& vec)
{
	vec.clear();
	QMap<QString, medicine>::iterator itor;
	for (itor = m_MapVasoactivityData.begin(); itor != m_MapVasoactivityData.end(); ++itor)
	{
		vec.append(itor.value());
	}
}

void clinicalLogic::GetAnalgeiaData(QVector<medicine>& vec)
{
	vec.clear();
	QMap<QString, medicine>::iterator itor;
	for (itor = m_MapAnalgeiaData.begin(); itor != m_MapAnalgeiaData.end(); ++itor)
	{
		vec.append(itor.value());
	}
}

void clinicalLogic::GetCalmData(QVector<medicine>& vec)
{
	vec.clear();
	QMap<QString, medicine>::iterator itor;
	for (itor = m_MapCalmData.begin(); itor != m_MapCalmData.end(); ++itor)
	{
		vec.append(itor.value());
	}
}

void clinicalLogic::GetMuscleRelaxationData(QVector<medicine>& vec)
{
	vec.clear();
	QMap<QString, medicine>::iterator itor;
	for (itor = m_MapMuscleRelaxationData.begin(); itor != m_MapMuscleRelaxationData.end(); ++itor)
	{
		vec.append(itor.value());
	}
}

void clinicalLogic::checkBreathZero(breathe& data)
{
	if (data.LMin == "")
		data.LMin = "0";
	if (data.FiO2 == "")
		data.FiO2 = "0";
	if (data.IPAP == "")
		data.IPAP = "0";
	if (data.EPAP == "")
		data.EPAP = "0";
	if (data.PEEP == "")
		data.PEEP = "0";
	if (data.hreathModel == "")
		data.hreathModel = "none";
}

void clinicalLogic::checkMediantZero(medicine& data)
{
	if(data.TotalVolume == "")
		data.TotalVolume = "0";
	if (data.MediaVolume == "")
		data.MediaVolume = "0";
	if (data.PumpingSpeed == "")
		data.PumpingSpeed = "0";
	if (data.PumpingVolume == "")
		data.PumpingVolume = "0";
	if (data.Score == "")
		data.Score = "0";
}
