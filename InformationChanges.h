#pragma once

#include "EssentialInformation.h"
#include "ClinicalInformation.h"
#include "NutritionalInformation.h"
#include "AntrumAreaCurvature.h"
#include "PickUPInformation.h"
#include <QWidget>
#include <QPushButton>
#include <QScrollArea>
#include <qboxlayout.h>

#include "ui_InformationChanges.h"

class InformationChanges : public QWidget
{
	Q_OBJECT

public:
	InformationChanges(QWidget *parent = nullptr);
	~InformationChanges();

private:
	Ui::InformationChangesClass ui;


private:
	QVBoxLayout* mainlay;
	QScrollArea* verticalScrollArea;
	QWidget* widget;
	//QVBoxLayout* scrollAreaVBox;
	QPushButton* returnPushButton;
	EssentialInformation* essentialInformation;
	ClinicalInformation* clinicalInformation;
	NutritionalInformation* nutritionalInformation;
	AntrumAreaCurvature* antrumAreaCurvature;
	PickUPInformation* pickUPInformation;
	QHBoxLayout* buttons;
	QPushButton* derive;
	QPushButton* returnPushButton2;
};
