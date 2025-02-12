#pragma once

#include <QWidget>
#include <qlabel.h>
#include <qboxlayout.h>
#include <QCheckBox>
#include <QPushButton>
#include <QDateTimeEdit>

#include "nutritionalLogic.h"
#include "InformationItem.h"
#include "selectDisplay.h"

class NutritionalInformation : public QWidget
{
	Q_OBJECT

public:
	NutritionalInformation(QWidget *parent = nullptr);
	~NutritionalInformation();

	void GetData(QVector<nutritionalInformation>& vec);

public slots:
	void OnENCheckBox(bool b);
	void OnPNCheckBox(bool b);
	void OnIMCheckBox(bool b);

	void OnENBtn();
	void OnPNBtn();
	void OnIMBtn();

	void OnDeleteItem(short t, QString& key);

private:
	QVBoxLayout* mainlay;
	QLabel* title;

	QWidget* ENWidget;
	QWidget* PNWidget;
	QWidget* IMWidget;

	QCheckBox* ENCheckBox;
	QCheckBox* PNCheckBox;
	QCheckBox* IMCheckBox;

	InformationItem* ENFormula;
	InformationItem* ENVolume;
	InformationItem* ENCalories;
	InformationItem* ENProtein;
	QDateTimeEdit* ENStartTime;


	InformationItem* PNFormula;
	InformationItem* PNVolume;
	InformationItem* PNCalories;
	InformationItem* PNProtein;
	QDateTimeEdit* PNStartTime;

	InformationItem* IMFormula;
	InformationItem* IMVolume;
	InformationItem* IMCalories; 
	InformationItem* IMProtein;
	QDateTimeEdit* IMStartTime;

	nutritionalLogic* m_pLogic;
};
