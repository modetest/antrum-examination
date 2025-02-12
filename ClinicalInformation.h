#pragma once

#include <QWidget>
#include <qlabel.h>
#include <QPushButton>
#include <QRadioButton>
#include <qboxlayout.h>
#include <QComboBox>
#include <QCheckBox>
#include <QDateTimeEdit>


#include "InformationItem.h"
#include "clinicalLogic.h"
#include "selectDisplay.h"

class ClinicalInformation : public QWidget
{
	Q_OBJECT

public:
	ClinicalInformation(QWidget *parent = nullptr);
	~ClinicalInformation();
public:
	void SetData(clinicalInformation& clin);
	void GetData(clinicalInformation& clin);

public slots:
	void OnVentilatorYesOrNo();
	void OnVentilatorChange(const QString& index);
	void OnVentilatorBtn();
	void OnDeleteItem(short t, QString& key);


	void OnVasoactivityCheckBox(bool b);
	void OnAnalgeiaCheckBox(bool b);
	void OnCalmCheckBox(bool b);
	void OnMuscleRelaxationCheckBox(bool b);


	void OnVasoactivityBtn();
	void OnAnalgeiaBtn();
	void OnCalmBtn();
	void OnMuscleRelaxationBtn();

	void OnAutoCompute();


signals:
	void scrollDown();


private:
	clinicalLogic* m_pLogic;

	QVBoxLayout* mainlay;
	QLabel* title;
	QGridLayout* itemData;

	InformationItem* heartRate;//心率
	InformationItem* rhythmHeart;//心律
	InformationItem* rr;//RR
	InformationItem* systolicPressure;//收缩压
	InformationItem* diastolicPressure;//舒张压
	InformationItem* map;//MAP
	InformationItem* pi;//PI
	InformationItem* spo;//SPO
	InformationItem* ultrasonicNumber;//超声编号
	QDateTimeEdit* StartTime;

	//呼吸机
	QRadioButton* yesVentilator;
	QRadioButton* noVentilator;
	QWidget* ventilatorWidget;
	QComboBox* ventilatorComboBox;
	InformationItem* lmin;
	InformationItem* FiO2;
	InformationItem* IPAP;
	InformationItem* EPAP;
	InformationItem* PEEP;
	InformationItem* ventilatorMode;
	QPushButton* ventilatorBtn;

	//药物
	QCheckBox* VasoactivityCheckBox;
	QCheckBox* AnalgeiaCheckBox;
	QCheckBox* CalmCheckBox;
	QCheckBox* MuscleRelaxationCheckBox;

	QWidget* VasoactivityWidget;
	QWidget* AnalgeiaWidget;
	QWidget* CalmWidget;
	QWidget* MuscleRelaxationWidget;

	QComboBox* VasoactivityCombobox;
	QComboBox* AnalgeiaCombobox;
	QComboBox* CalmCombobox;
	QComboBox* MuscleRelaxationCombobox;

	InformationItem* VasoactivityTotalVolume;
	InformationItem* VasoactivityMediumVolume;
	InformationItem* VasoactivityPumpInSpeed;
	InformationItem* VasoactivityPumpInVolume;
	QPushButton* VasoactivityOKBrn;

	InformationItem* AnalgeiaTotalVolume;
	InformationItem* AnalgeiaMediumVolume;
	InformationItem* AnalgeiaPumpInSpeed;
	InformationItem* AnalgeiaPumpInVolume;
	InformationItem* AnalgeiaCPOTScore;
	QPushButton* AnalgeiaOKBrn;

	InformationItem* CalmTotalVolume;
	InformationItem* CalmMediumVolume;
	InformationItem* CalmPumpInSpeed;
	InformationItem* CalmPumpInVolume;
	InformationItem* CalmRASSScore;
	QPushButton* CalmOKBrn;

	InformationItem* MuscleRelaxationTotalVolume;
	InformationItem* MuscleRelaxationMediumVolume;
	InformationItem* MuscleRelaxationPumpInSpeed;
	InformationItem* MuscleRelaxationPumpInVolume;
	InformationItem* MuscleRelaxationBISScore;
	QPushButton* MuscleRelaxationOKBrn;

};
