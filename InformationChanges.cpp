#include "InformationChanges.h"
#include <qdebug.h>


InformationChanges::InformationChanges(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

	qDebug() << QApplication::applicationDirPath();

	mainlay = new QVBoxLayout();
	returnPushButton = new QPushButton(u8"返回",this);
	returnPushButton->setFixedSize(110,40);
	returnPushButton->setStyleSheet("QPushButton{ background-image: url(:/new/prefix1/UI/Rectangle 24.png);border: none;color:white;font-size: 21px; }"
		"QPushButton:pressed{ background-image: url(:/new/prefix1/UI/Rectangle 24.png);border: none;color:white;font-size: 21px; }"
		"QPushButton:hover { background-image: url(:/new/prefix1/UI/Rectangle 24.png);border: none;color:white;font-size: 21px; }");

	essentialInformation = new EssentialInformation(this);
	clinicalInformation = new ClinicalInformation(this);
	nutritionalInformation = new NutritionalInformation(this);
	antrumAreaCurvature = new AntrumAreaCurvature(this);
	pickUPInformation = new PickUPInformation(this);

	buttons = new QHBoxLayout(this);
	derive = new QPushButton(u8"导出为PDF",this);
	derive->setFixedSize(180, 50);
	derive->setStyleSheet("QPushButton{ background-image: url(:/new/prefix1/UI/Rectangle 25.png);border: none;color:white;font-size: 21px; }"
		"QPushButton:pressed{ background-image: url(:/new/prefix1/UI/Rectangle 25.png);border: none;color:white;font-size: 21px; }"
		"QPushButton:hover { background-image: url(:/new/prefix1/UI/Rectangle 25.png);border: none;color:white;font-size: 21px; }");
	returnPushButton2 = new QPushButton(u8"返回");
	returnPushButton2->setFixedSize(180, 50);
	returnPushButton2->setStyleSheet("QPushButton{ background-image: url(:/new/prefix1/UI/Rectangle 25.png);border: none;color:white;font-size: 21px; }"
		"QPushButton:pressed{ background-image: url(:/new/prefix1/UI/Rectangle 25.png);border: none;color:white;font-size: 21px; }"
		"QPushButton:hover { background-image: url(:/new/prefix1/UI/Rectangle 25.png);border: none;color:white;font-size: 21px; }");
	buttons->addWidget(derive);
	buttons->addWidget(returnPushButton2);


	verticalScrollArea = new QScrollArea(this);
	verticalScrollArea->setFixedSize(1920, 1080);
	//scrollAreaVBox = new QVBoxLayout(verticalScrollArea);

	mainlay->addWidget(returnPushButton);
	mainlay->addWidget(essentialInformation);//基本信息
	mainlay->addWidget(clinicalInformation);//临床信息
	mainlay->addWidget(nutritionalInformation);//营养信息
	mainlay->addWidget(antrumAreaCurvature);//胃窦面积变化率
	mainlay->addWidget(pickUPInformation);//采集信息
	mainlay->addLayout(buttons);

	widget = new QWidget(this);
	widget->setLayout(mainlay);
	verticalScrollArea->setWidget(widget);
	

	/*mainlay->addWidget(verticalScrollArea);

	scrollAreaVBox->addWidget(returnPushButton);
	scrollAreaVBox->addWidget(essentialInformation);

	widget = new QWidget;
	widget->setLayout(scrollAreaVBox);
	verticalScrollArea->setWidget(widget);*/
}

InformationChanges::~InformationChanges()
{
	

}
