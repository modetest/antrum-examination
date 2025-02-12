#include "ClinicalInformation.h"

ClinicalInformation::ClinicalInformation(QWidget *parent)
	: QWidget(parent)
{
	m_pLogic = new clinicalLogic();

	QPalette pal(this->palette());
	QColor color("#283141");
	pal.setColor(QPalette::Window, color);
	this->setAutoFillBackground(true);
	this->setPalette(pal);

	mainlay = new QVBoxLayout(this);
	mainlay->setMargin(0);

	title = new QLabel();
	title->setText(u8" ÁÙ´²ÐÅÏ¢");
	//title->setFixedSize(1863, 48);
	title->setFixedHeight(48);
	title->setStyleSheet("QLabel{background-color: #323E55;color:white;font-size:30px;}");

	itemData = new QGridLayout();
	itemData->setContentsMargins(54, 0, 54, 85);
	itemData->setVerticalSpacing(0);
	itemData->setHorizontalSpacing(164);


	QIntValidator* intValidator = new QIntValidator(1, 999, this);
	QDoubleValidator* doubleValidator = new QDoubleValidator(1, 99999, 2, this);

	heartRate = new InformationItem(intValidator,this);
	heartRate->InitInformationItem(u8"ÐÄÂÊ£º", u8"´Î/·Ö");
	rhythmHeart = new InformationItem(nullptr, this);
	rhythmHeart->InitInformationItem(u8"ÐÄÂÉ£º", "");
	rr = new InformationItem(intValidator, this);
	rr->InitInformationItem(u8"RR£º", u8"´Î/·Ö");
	systolicPressure = new InformationItem(intValidator, this);
	systolicPressure->InitInformationItem(u8"ÊÕËõÑ¹£º", "mmHg");
	diastolicPressure = new InformationItem(intValidator, this);
	diastolicPressure->InitInformationItem(u8"ÊæÕÅÑ¹£º", "mmHg");

	connect(systolicPressure->GetValueObject(), &QLineEdit::textChanged, this, &ClinicalInformation::OnAutoCompute);
	connect(diastolicPressure->GetValueObject(), &QLineEdit::textChanged, this, &ClinicalInformation::OnAutoCompute);


	map = new InformationItem(intValidator, this);
	map->InitInformationItem(u8"MAP£º", "mmHg");
	map->ReadOnly(true);
	pi = new InformationItem(doubleValidator, this);
	pi->InitInformationItem(u8"PI£º", "");
	spo = new InformationItem(intValidator, this);
	spo->InitInformationItem(u8"SPO2£º", "%");
	ultrasonicNumber = new InformationItem(nullptr, this);
	ultrasonicNumber->InitInformationItem(u8"³¬Éù±àºÅ£º", "");

	QHBoxLayout* HStartTimelayout = new  QHBoxLayout();
	QLabel* StartTimeLabel = new QLabel(u8"¿ªÊ¼Ê±¼ä£º");
	StartTimeLabel->setStyleSheet("width:89px;height:42px;color:#ffffff;font-size:28px;");
	StartTime = new QDateTimeEdit();
	StartTime->setDateTime(QDateTime::currentDateTime());
	StartTime->setFixedSize(305, 42);
	StartTime->setStyleSheet("color:#ffffff;font-size:23px;background-color:#354052;border-style:outset;alternate-background-color:rgb(204, 229, 255);");
	HStartTimelayout->addWidget(StartTimeLabel);
	HStartTimelayout->addWidget(StartTime);


	itemData->addWidget(heartRate, 0, 0);
	itemData->addWidget(rhythmHeart, 0, 1);
	itemData->addWidget(rr, 0, 2);
	itemData->addWidget(systolicPressure, 1, 0);
	itemData->addWidget(diastolicPressure, 1, 1);
	itemData->addWidget(map, 1, 2);
	itemData->addWidget(pi, 2, 0);
	itemData->addWidget(spo, 2, 1);
	itemData->addWidget(ultrasonicNumber, 2, 2);
	itemData->addLayout(HStartTimelayout, 3, 0);

	mainlay->addWidget(title);
	mainlay->addLayout(itemData);

	//ºôÎü·½Ê½
	QHBoxLayout* ventilatorTitleLaoyout = new QHBoxLayout();
	ventilatorTitleLaoyout->setContentsMargins(10, 0, 0, 0);	
	ventilatorTitleLaoyout->setSpacing(60);
	QLabel* ventilatorQLabel = new QLabel(u8"ºôÎüÖ§³Ö·½Ê½£º");
	ventilatorQLabel->setStyleSheet("width:89px;height:42px;color:#ffffff;font-size:28px;");
	yesVentilator = new QRadioButton(u8"ÊÇ");
	yesVentilator->setStyleSheet("color:#ffffff;font-size:28px;");
	noVentilator = new QRadioButton(u8"·ñ");
	noVentilator->setChecked(true);
	noVentilator->setStyleSheet("color:#ffffff;font-size:28px;");
	ventilatorTitleLaoyout->addWidget(ventilatorQLabel);
	ventilatorTitleLaoyout->addWidget(yesVentilator);
	ventilatorTitleLaoyout->addWidget(noVentilator);
	ventilatorTitleLaoyout->addStretch();
	connect(yesVentilator,&QRadioButton::clicked,this,&ClinicalInformation::OnVentilatorYesOrNo);
	connect(noVentilator, &QRadioButton::clicked, this, &ClinicalInformation::OnVentilatorYesOrNo);

	mainlay->addLayout(ventilatorTitleLaoyout);

	ventilatorWidget = new QWidget(this);
	ventilatorWidget->hide();
	QVBoxLayout* ventilatorWidgetLayout = new QVBoxLayout(ventilatorWidget);
	ventilatorWidgetLayout->setContentsMargins(50, 0, 0, 0);
	ventilatorWidget->setAutoFillBackground(true);
	ventilatorWidget->setPalette(pal);
	QHBoxLayout* hLaout = new QHBoxLayout();


	ventilatorComboBox = new QComboBox(ventilatorWidget);
	ventilatorComboBox->addItem(u8"±Çµ¼¹Ü");
	ventilatorComboBox->addItem(u8"ÃæÕÖÎüÑõ");
	ventilatorComboBox->addItem(u8"¸ßÁ÷Á¿ÎüÑõ");
	ventilatorComboBox->addItem(u8"ÎÞ´´ºôÎü»ú¸¨ÖúºôÎü");
	ventilatorComboBox->addItem(u8"ÓÐ´´ºôÎü»ú¸¨ÖúºôÎü");
	ventilatorComboBox->setStyleSheet("width:210px;height:42px;color:#ffffff;font-size:23px;background-color:#354052;border-style:outset");

	connect(ventilatorComboBox, &QComboBox::currentTextChanged, this, &ClinicalInformation::OnVentilatorChange);
	
	lmin = new InformationItem(doubleValidator, ventilatorWidget);
	lmin->SetWidth(70,200);   
	lmin->InitInformationItem(u8"", "l/min");

	FiO2 = new InformationItem(doubleValidator, ventilatorWidget);
	FiO2->SetWidth(70, 200);
	FiO2->InitInformationItem(u8"FiO2:", "%");

	IPAP = new InformationItem(doubleValidator, ventilatorWidget);
	IPAP->SetWidth(70, 200);
	IPAP->InitInformationItem(u8"IPAP:", "cmH2O");

	EPAP = new InformationItem(doubleValidator, ventilatorWidget);
	EPAP->SetWidth(70, 200);
	EPAP->InitInformationItem(u8"EPAP:", "cmH2O");

	PEEP = new InformationItem(doubleValidator, ventilatorWidget);
	PEEP->SetWidth(70, 200);
	PEEP->InitInformationItem(u8"PEEP:", "cmH2O");

	ventilatorMode = new InformationItem(nullptr, ventilatorWidget);
	ventilatorMode->SetWidth(200, 500);
	ventilatorMode->InitInformationItem(u8"ºôÎü»úÄ£Ê½:", "");

	ventilatorBtn = new QPushButton(ventilatorWidget);
	ventilatorBtn->setText(u8"È·¶¨");
	ventilatorBtn->setFixedSize(62, 26);
	ventilatorBtn->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/time_ok_2.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333;}");

	connect(ventilatorBtn, &QPushButton::clicked, this, &ClinicalInformation::OnVentilatorBtn);
	
	FiO2->setVisible(false);
	IPAP->setVisible(false);
	EPAP->setVisible(false);
	PEEP->setVisible(false);
	ventilatorMode->setVisible(false);
		
	hLaout->addWidget(ventilatorComboBox);
	hLaout->addWidget(lmin);
	hLaout->addWidget(FiO2);
	hLaout->addWidget(IPAP);
	hLaout->addWidget(EPAP);
	hLaout->addWidget(PEEP);	
	hLaout->addWidget(ventilatorMode);
	hLaout->addStretch();
	hLaout->addWidget(ventilatorBtn);
	hLaout->addStretch();

	ventilatorWidgetLayout->addLayout(hLaout);
	mainlay->addWidget(ventilatorWidget);

	//Ò©Îï
	QHBoxLayout* medicantTitleLaoyout = new QHBoxLayout();
	medicantTitleLaoyout->setContentsMargins(10, 0, 0, 0);
	medicantTitleLaoyout->setSpacing(20);
	QLabel* medicantQLabel = new QLabel(u8"ÊÇ·ñÊ¹ÓÃÒ©Îï£º");
	medicantQLabel->setStyleSheet("width:89px;height:42px;color:#ffffff;font-size:28px;");
	VasoactivityCheckBox = new QCheckBox(u8"Ñª¹Ü»îÐÔÒ©Îï");
	AnalgeiaCheckBox = new QCheckBox(u8"ÕòÍ´Ò©Îï");
	CalmCheckBox = new QCheckBox(u8"Õò¾²Ò©Îï");
	MuscleRelaxationCheckBox = new QCheckBox(u8"¼¡ËÉÒ©Îï");

	VasoactivityCheckBox->setStyleSheet("QCheckBox{color:#ffffff;font-size:28px;}QCheckBox::indicator{width:18px; height:18px;} QCheckBox::indicator:unchecked {image:url(:/antrum/resouce/radiobtn_unchecked.png);} QCheckBox::indicator:checked{image: url(:/antrum/resouce/qradiobtn_checked.png);}");
	
	AnalgeiaCheckBox->setStyleSheet("QCheckBox{color:#ffffff;font-size:28px;}QCheckBox::indicator{width:18px; height:18px;} QCheckBox::indicator:unchecked {image:url(:/antrum/resouce/radiobtn_unchecked.png);} QCheckBox::indicator:checked{image: url(:/antrum/resouce/qradiobtn_checked.png);}");

	CalmCheckBox->setStyleSheet("QCheckBox{color:#ffffff;font-size:28px;}QCheckBox::indicator{width:18px; height:18px;} QCheckBox::indicator:unchecked {image:url(:/antrum/resouce/radiobtn_unchecked.png);} QCheckBox::indicator:checked{image: url(:/antrum/resouce/qradiobtn_checked.png);}");

	MuscleRelaxationCheckBox->setStyleSheet("QCheckBox{color:#ffffff;font-size:28px;}QCheckBox::indicator{width:18px; height:18px;} QCheckBox::indicator:unchecked {image:url(:/antrum/resouce/radiobtn_unchecked.png);} QCheckBox::indicator:checked{image: url(:/antrum/resouce/qradiobtn_checked.png);}");

	connect(VasoactivityCheckBox,&QCheckBox::toggled,this,&ClinicalInformation::OnVasoactivityCheckBox);
	connect(AnalgeiaCheckBox, &QCheckBox::toggled, this, &ClinicalInformation::OnAnalgeiaCheckBox);
	connect(CalmCheckBox, &QCheckBox::toggled, this, &ClinicalInformation::OnCalmCheckBox);
	connect(MuscleRelaxationCheckBox, &QCheckBox::toggled, this, &ClinicalInformation::OnMuscleRelaxationCheckBox);

	medicantTitleLaoyout->addWidget(medicantQLabel);
	medicantTitleLaoyout->addWidget(VasoactivityCheckBox);
	medicantTitleLaoyout->addWidget(AnalgeiaCheckBox);
	medicantTitleLaoyout->addWidget(CalmCheckBox);
	medicantTitleLaoyout->addWidget(MuscleRelaxationCheckBox);
	medicantTitleLaoyout->addStretch();
	mainlay->addLayout(medicantTitleLaoyout);

	
	VasoactivityWidget = new QWidget(this);
	AnalgeiaWidget = new QWidget(this);
	CalmWidget = new QWidget(this);
	MuscleRelaxationWidget = new QWidget(this);

	QVBoxLayout* VasoactivityLayout = new QVBoxLayout(VasoactivityWidget);
	QVBoxLayout* AnalgeiaLayout = new QVBoxLayout(AnalgeiaWidget);
	QVBoxLayout* CalmLayout = new QVBoxLayout(CalmWidget);
	QVBoxLayout* MuscleRelaxationLayout = new QVBoxLayout(MuscleRelaxationWidget);

	//Ñª¹Ü»îÐÔ
	QGridLayout* VasoactivityGridLayout = new QGridLayout();

	QLabel* VasoactivityLabel = new QLabel(u8"Ñª¹Ü»îÐÔÒ©Îï£º");
	VasoactivityLabel->setStyleSheet("width:89px;height:42px;color:#ffffff;font-size:28px;");
	VasoactivityCombobox = new QComboBox(VasoactivityWidget);
	QStringList list;
	list << u8"È¥¼×ÉöÉÏÏÙËØ" << u8"¼äôÇ°·" << u8"´¹Ìå"<<u8"ÑÎËá¶à°Í°·×¢ÉäÒº" << u8"ÑÎËáÉöÉÏÏÙËØ×¢ÉäÒº" << u8"Ã×Á¦Å©×¢ÉäÒº" << u8"×óÎ÷ÃÏµ©" << u8"ÑÎËá¶à°Í·Ó¶¡°·×¢ÉäÒº" << u8"È¥ÒÒõ£Ã«»¨ÜÕ×¢ÉäÒº" << u8"µØ¸ßÐÁ×¢ÉäÒº" << u8"ÏõËá¸ÊÓÍ×¢ÉäÒº" << u8"×¢ÉäÓÃÏõÆÕÄÆ" << u8"ÑÎËáÄá¿¨µØÆ½×¢ÉäÒº" << u8"ÑÎËáÎÚÀ­µØ¶û×¢ÉäÒº" << u8"ÑÎËá°·µâÍª×¢ÉäÒº" << u8"ÑÎËáÀû¶à¿¨Òò×¢ÉäÒº" << u8"ÑÎËá°¬Ë¾Âå¶û×¢ÉäÒº" << u8"¾ÆÊ¯ËáÃÀÍÐÂå¶û×¢ÉäÒº";
	VasoactivityCombobox->addItems(list);
	//VasoactivityCombobox->setStyleSheet("width:210px;height:42px;color:#ffffff;font-size:23px;background-color:#354052;border-style:outset");
	VasoactivityCombobox->setFixedSize(300, 48);
	VasoactivityCombobox->setStyleSheet("color:#ffffff;font-size:23px;background-color:#354052;border-style:outset");
	VasoactivityTotalVolume = new InformationItem(doubleValidator, VasoactivityWidget);
	VasoactivityTotalVolume->SetWidth(165, 200);
	VasoactivityTotalVolume->InitInformationItem(u8"Ò©Îï×Ü¼ÁÁ¿:", "mg");
	VasoactivityMediumVolume = new InformationItem(doubleValidator, VasoactivityWidget);
	VasoactivityMediumVolume->SetWidth(130, 200);
	VasoactivityMediumVolume->InitInformationItem(u8"Ã½½éÈÝ»ý:", "ml");
	VasoactivityPumpInSpeed = new InformationItem(doubleValidator, VasoactivityWidget);
	VasoactivityPumpInSpeed->SetWidth(130, 200);
	VasoactivityPumpInSpeed->InitInformationItem(u8"±ÃÈëËÙ¶È:", "ml/h");
	VasoactivityPumpInVolume = new InformationItem(doubleValidator, VasoactivityWidget);
	VasoactivityPumpInVolume->SetWidth(130, 200);
	VasoactivityPumpInVolume->InitInformationItem(u8"Ã½½é¼ÁÁ¿:", "ug/kg/min  ");
	VasoactivityOKBrn = new QPushButton(VasoactivityWidget);
	VasoactivityOKBrn->setText(u8"È·¶¨");
	VasoactivityOKBrn->setFixedSize(62, 26);
	VasoactivityOKBrn->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/time_ok_2.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333;}");

	connect(VasoactivityOKBrn, &QPushButton::clicked, this, &ClinicalInformation::OnVasoactivityBtn);
		
	VasoactivityGridLayout->addWidget(VasoactivityLabel, 0, 0);
	VasoactivityGridLayout->addWidget(VasoactivityCombobox, 0, 1,Qt::AlignLeft);
	VasoactivityGridLayout->addWidget(VasoactivityTotalVolume, 0, 2,Qt::AlignRight);
	VasoactivityGridLayout->addWidget(VasoactivityMediumVolume, 0, 3, Qt::AlignRight);
	VasoactivityGridLayout->addWidget(VasoactivityPumpInSpeed, 0, 4, Qt::AlignRight);
	VasoactivityGridLayout->addWidget(VasoactivityPumpInVolume, 1, 2, Qt::AlignRight);
	VasoactivityGridLayout->addWidget(VasoactivityOKBrn, 1, 4, Qt::AlignRight);

	VasoactivityLayout->addLayout(VasoactivityGridLayout);
	VasoactivityWidget->hide();
	mainlay->addWidget(VasoactivityWidget);


	//ÕòÍ´
	QGridLayout* AnalgeiaGridLayout = new QGridLayout();
	QLabel* AnalgeiaLabel = new QLabel(u8"ÕòÍ´Ò©Îï£º");
	AnalgeiaLabel->setStyleSheet("width:89px;height:42px;color:#ffffff;font-size:28px;");
	AnalgeiaCombobox = new QComboBox(AnalgeiaWidget);
	QStringList Analgeialist;
	Analgeialist << u8"ÑÎËáÓÒÃÀÍÐßä¶¨×¢ÉäÒº" << u8"ÑÎËá°¬Ë¾ÂÈ°·Íª×¢ÉäÒº" << u8"×¢ÉäÒºÑÎËáÈð·ÒÌ«Äá" << u8"ÑÎËá°¢·ÒÌ«Äá×¢ÉäÒº" << u8"èÛéÚËá·ÒÌ«Äá×¢ÉäÒº" << u8"èÛéÚËáÊæ·ÒÌ«Äá×¢ÉäÒº" << u8"ÑÎËáÂð·È×¢ÉäÒº" << u8"ÑÎËáÄÉ²¼·È×¢ÉäÒº" << u8"¾ÆÊ¯Ëá²¼ÍÐ·ÈÅµ×¢ÉäÒº" << u8"µØ×ôÐÁ×¢ÉäÒº" << u8"×¢ÉäÒºÅÁÈðÎô²¼ÄÆ" << u8"Íª¿©Ëá°±¶¡Èý´¼×¢ÉäÒº" << u8"ÑÎËáÇúÂí¶à×¢ÉäÒº";
	AnalgeiaCombobox->addItems(Analgeialist);
	//VasoactivityCombobox->setStyleSheet("width:210px;height:42px;color:#ffffff;font-size:23px;background-color:#354052;border-style:outset");
	AnalgeiaCombobox->setFixedSize(300, 48);
	AnalgeiaCombobox->setStyleSheet("color:#ffffff;font-size:23px;background-color:#354052;border-style:outset");
	AnalgeiaTotalVolume = new InformationItem(doubleValidator, AnalgeiaWidget);
	AnalgeiaTotalVolume->SetWidth(165, 200);
	AnalgeiaTotalVolume->InitInformationItem(u8"Ò©Îï×Ü¼ÁÁ¿:", "mg");
	AnalgeiaMediumVolume = new InformationItem(doubleValidator, AnalgeiaWidget);
	AnalgeiaMediumVolume->SetWidth(130, 200);
	AnalgeiaMediumVolume->InitInformationItem(u8"Ã½½éÈÝ»ý:", "ml");
	AnalgeiaPumpInSpeed = new InformationItem(doubleValidator, AnalgeiaWidget);
	AnalgeiaPumpInSpeed->SetWidth(130, 200);
	AnalgeiaPumpInSpeed->InitInformationItem(u8"±ÃÈëËÙ¶È:", "ml/h");
	AnalgeiaPumpInVolume = new InformationItem(doubleValidator, AnalgeiaWidget);
	AnalgeiaPumpInVolume->SetWidth(130, 200);
	AnalgeiaPumpInVolume->InitInformationItem(u8"Ã½½é¼ÁÁ¿:", "ug/kg/min  ");

	AnalgeiaCPOTScore = new InformationItem(intValidator, AnalgeiaWidget);
	AnalgeiaCPOTScore->SetWidth(130, 200);
	AnalgeiaCPOTScore->InitInformationItem(u8"CPOTÆÀ·Ö:", "  ");

	AnalgeiaOKBrn = new QPushButton(AnalgeiaWidget);
	AnalgeiaOKBrn->setText(u8"È·¶¨");
	AnalgeiaOKBrn->setFixedSize(62, 26);
	AnalgeiaOKBrn->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/time_ok_2.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333;}");

	connect(AnalgeiaOKBrn, &QPushButton::clicked, this, &ClinicalInformation::OnAnalgeiaBtn);

	AnalgeiaGridLayout->addWidget(AnalgeiaLabel, 0, 0);
	AnalgeiaGridLayout->addWidget(AnalgeiaCombobox, 0, 1, Qt::AlignLeft);
	AnalgeiaGridLayout->addWidget(AnalgeiaTotalVolume, 0, 2, Qt::AlignRight);
	AnalgeiaGridLayout->addWidget(AnalgeiaMediumVolume, 0, 3, Qt::AlignRight);
	AnalgeiaGridLayout->addWidget(AnalgeiaPumpInSpeed, 0, 4, Qt::AlignRight);
	AnalgeiaGridLayout->addWidget(AnalgeiaPumpInVolume, 1, 2, Qt::AlignRight);
	AnalgeiaGridLayout->addWidget(AnalgeiaCPOTScore, 1, 3, Qt::AlignRight);
	AnalgeiaGridLayout->addWidget(AnalgeiaOKBrn, 1, 4, Qt::AlignRight);
								  
	AnalgeiaLayout->addLayout(AnalgeiaGridLayout);
	AnalgeiaWidget->hide();
	mainlay->addWidget(AnalgeiaWidget);

	//Õò¾²
	QGridLayout* CalmGridLayout = new QGridLayout();
	QLabel* CalmLabel = new QLabel(u8"Õò¾²Ò©Îï£º");
	CalmLabel->setStyleSheet("width:89px;height:42px;color:#ffffff;font-size:28px;");
	CalmCombobox = new QComboBox(CalmWidget);
	QStringList Calmlist;
	Calmlist << u8"»·²´·Ó×¢ÉäÒº" << u8"±û²´·ÓÈé×´×¢ÉäÒº" << u8"	±û²´·Ó / ÖÐ³¤Á´Ö¬·¾Èé×¢ÉäÒº" << u8"ÒÀÍÐßäõ¥×¢ÉäÒº" << u8"ßä´ïßòÂØ×¢ÉäÒº" << u8"×¢Éä±½»ÇËáÈðÂíßòÂØ" ;
	CalmCombobox->addItems(Calmlist);
	//VasoactivityCombobox->setStyleSheet("width:210px;height:42px;color:#ffffff;font-size:23px;background-color:#354052;border-style:outset");
	CalmCombobox->setFixedSize(300, 48);
	CalmCombobox->setStyleSheet("color:#ffffff;font-size:23px;background-color:#354052;border-style:outset");
	CalmTotalVolume = new InformationItem(doubleValidator, CalmWidget);
	CalmTotalVolume->SetWidth(165, 200);
	CalmTotalVolume->InitInformationItem(u8"Ò©Îï×Ü¼ÁÁ¿:", "mg");
	CalmMediumVolume = new InformationItem(doubleValidator, CalmWidget);
	CalmMediumVolume->SetWidth(130, 200);
	CalmMediumVolume->InitInformationItem(u8"Ã½½éÈÝ»ý:", "ml");
	CalmPumpInSpeed = new InformationItem(doubleValidator, CalmWidget);
	CalmPumpInSpeed->SetWidth(130, 200);
	CalmPumpInSpeed->InitInformationItem(u8"±ÃÈëËÙ¶È:", "ml/h");
	CalmPumpInVolume = new InformationItem(doubleValidator, CalmWidget);
	CalmPumpInVolume->SetWidth(130, 200);
	CalmPumpInVolume->InitInformationItem(u8"Ã½½é¼ÁÁ¿:", "ug/kg/min  ");

	CalmRASSScore = new InformationItem(intValidator, CalmWidget);
	CalmRASSScore->SetWidth(130, 200);
	CalmRASSScore->InitInformationItem(u8"RASSÆÀ·Ö:", "  ");

	CalmOKBrn = new QPushButton(CalmWidget);
	CalmOKBrn->setText(u8"È·¶¨");
	CalmOKBrn->setFixedSize(62, 26);
	CalmOKBrn->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/time_ok_2.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333;}");

	connect(CalmOKBrn, &QPushButton::clicked, this, &ClinicalInformation::OnCalmBtn);

	CalmGridLayout->addWidget(CalmLabel, 0, 0);
	CalmGridLayout->addWidget(CalmCombobox, 0, 1, Qt::AlignLeft);
	CalmGridLayout->addWidget(CalmTotalVolume, 0, 2, Qt::AlignRight);
	CalmGridLayout->addWidget(CalmMediumVolume, 0, 3, Qt::AlignRight);
	CalmGridLayout->addWidget(CalmPumpInSpeed, 0, 4, Qt::AlignRight);
	CalmGridLayout->addWidget(CalmPumpInVolume, 1, 2, Qt::AlignRight);
	CalmGridLayout->addWidget(CalmRASSScore, 1, 3, Qt::AlignRight);
	CalmGridLayout->addWidget(CalmOKBrn, 1, 4, Qt::AlignRight);

	CalmLayout->addLayout(CalmGridLayout);
	CalmWidget->hide();
	mainlay->addWidget(CalmWidget);

	//¼¡ËÉ
	QGridLayout* MuscleRelaxationGridLayout = new QGridLayout();
	QLabel* MuscleRelaxationLabel = new QLabel(u8"¼¡ËÉÒ©Îï£º");
	MuscleRelaxationLabel->setStyleSheet("width:89px;height:42px;color:#ffffff;font-size:28px;");
	MuscleRelaxationCombobox = new QComboBox(MuscleRelaxationWidget);
	QStringList MuscleRelaxationlist;
	MuscleRelaxationlist << u8"×¢ÉäÓÃÎ¬¿âäåï§" << u8"±½»ÇË³°¢Çú¿âï§" ;
	MuscleRelaxationCombobox->addItems(MuscleRelaxationlist);
	//VasoactivityCombobox->setStyleSheet("width:210px;height:42px;color:#ffffff;font-size:23px;background-color:#354052;border-style:outset");
	MuscleRelaxationCombobox->setFixedSize(300, 48);
	MuscleRelaxationCombobox->setStyleSheet("color:#ffffff;font-size:23px;background-color:#354052;border-style:outset");
	MuscleRelaxationTotalVolume = new InformationItem(doubleValidator, MuscleRelaxationWidget);
	MuscleRelaxationTotalVolume->SetWidth(165, 200);
	MuscleRelaxationTotalVolume->InitInformationItem(u8"Ò©Îï×Ü¼ÁÁ¿:", "mg");
	MuscleRelaxationMediumVolume = new InformationItem(doubleValidator, MuscleRelaxationWidget);
	MuscleRelaxationMediumVolume->SetWidth(130, 200);
	MuscleRelaxationMediumVolume->InitInformationItem(u8"Ã½½éÈÝ»ý:", "ml");
	MuscleRelaxationPumpInSpeed = new InformationItem(doubleValidator, MuscleRelaxationWidget);
	MuscleRelaxationPumpInSpeed->SetWidth(130, 200);
	MuscleRelaxationPumpInSpeed->InitInformationItem(u8"±ÃÈëËÙ¶È:", "ml/h");
	MuscleRelaxationPumpInVolume = new InformationItem(doubleValidator, MuscleRelaxationWidget);
	MuscleRelaxationPumpInVolume->SetWidth(130, 200);
	MuscleRelaxationPumpInVolume->InitInformationItem(u8"Ã½½é¼ÁÁ¿:", "ug/kg/min  ");

	MuscleRelaxationBISScore = new InformationItem(intValidator, MuscleRelaxationWidget);
	MuscleRelaxationBISScore->SetWidth(130, 200);
	MuscleRelaxationBISScore->InitInformationItem(u8"BISÆÀ·Ö:", "  ");

	MuscleRelaxationOKBrn = new QPushButton(MuscleRelaxationWidget);
	MuscleRelaxationOKBrn->setText(u8"È·¶¨");
	MuscleRelaxationOKBrn->setFixedSize(62, 26);
	MuscleRelaxationOKBrn->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/time_ok_2.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333;}");

	connect(MuscleRelaxationOKBrn, &QPushButton::clicked, this, &ClinicalInformation::OnMuscleRelaxationBtn);

	MuscleRelaxationGridLayout->addWidget(MuscleRelaxationLabel, 0, 0);
	MuscleRelaxationGridLayout->addWidget(MuscleRelaxationCombobox, 0, 1, Qt::AlignLeft);
	MuscleRelaxationGridLayout->addWidget(MuscleRelaxationTotalVolume, 0, 2, Qt::AlignRight);
	MuscleRelaxationGridLayout->addWidget(MuscleRelaxationMediumVolume, 0, 3, Qt::AlignRight);
	MuscleRelaxationGridLayout->addWidget(MuscleRelaxationPumpInSpeed, 0, 4, Qt::AlignRight);
	MuscleRelaxationGridLayout->addWidget(MuscleRelaxationPumpInVolume, 1, 2, Qt::AlignRight);
	MuscleRelaxationGridLayout->addWidget(MuscleRelaxationBISScore, 1, 3, Qt::AlignRight);
	MuscleRelaxationGridLayout->addWidget(MuscleRelaxationOKBrn, 1, 4, Qt::AlignRight);

	MuscleRelaxationLayout->addLayout(MuscleRelaxationGridLayout);
	MuscleRelaxationWidget->hide();
	mainlay->addWidget(MuscleRelaxationWidget);
}

ClinicalInformation::~ClinicalInformation()
{}
void ClinicalInformation::GetData(clinicalInformation& clin)
{
	heartRate->GetValue(clin.hreat_rate);
	rhythmHeart->GetValue(clin.heart_Rhythm);
	rr->GetValue(clin.RR);
	systolicPressure->GetValue(clin.SBP);
	diastolicPressure->GetValue(clin.DBP);
	map->GetValue(clin.MAP);
	pi->GetValue(clin.PI);
	spo->GetValue(clin.SPO2);
	ultrasonicNumber->GetValue(clin.USCode);
	clin.AcTime = StartTime->text();

	if (clin.hreat_rate == "")
		clin.hreat_rate = "0";
	if (clin.heart_Rhythm == "")
		clin.heart_Rhythm = "none";
	if (clin.RR == "")
		clin.RR = "0";
	if (clin.SBP == "")
		clin.SBP = "0";
	if (clin.DBP == "")
		clin.DBP = "0";
	if (clin.MAP == "")
		clin.MAP = "0";
	if (clin.PI == "")
		clin.PI = "0";
	if (clin.SPO2 == "")
		clin.SPO2 = "0";
	if (clin.USCode == "")
		clin.USCode = "none";

	m_pLogic->GetHreatheData(clin.vecBreathe);
	m_pLogic->GetVasoactivityData(clin.vecActivity);
	m_pLogic->GetAnalgeiaData(clin.vecAnalgesia);
	m_pLogic->GetCalmData(clin.vecCalm);
	m_pLogic->GetMuscleRelaxationData(clin.vecMuscleRelaxation);
}
void ClinicalInformation::SetData(clinicalInformation& clin)
{
	this->heartRate->SetValueData(clin.hreat_rate);
	this->rhythmHeart->SetValueData(clin.heart_Rhythm);
	this->rr->SetValueData(clin.RR);
	this->systolicPressure->SetValueData(clin.SBP);
	this->diastolicPressure->SetValueData(clin.DBP);
	this->map->SetValueData(clin.MAP);
	this->pi->SetValueData(clin.PI);
	this->spo->SetValueData(clin.SPO2);
	this->ultrasonicNumber->SetValueData(clin.USCode);
}

void ClinicalInformation::OnVentilatorChange(const QString& index)
{
	auto indexnumber = ventilatorComboBox->currentIndex();
	switch (indexnumber)
	{
	case 0:
		lmin->setVisible(true);
		FiO2->setVisible(false);
		IPAP->setVisible(false);
		EPAP->setVisible(false);
		PEEP->setVisible(false);
		ventilatorMode->setVisible(false);
		break; 
	case 1:
		lmin->setVisible(true);
		FiO2->setVisible(false);
		IPAP->setVisible(false);
		EPAP->setVisible(false);
		PEEP->setVisible(false);
		ventilatorMode->setVisible(false);
		break;
	case 2:
		lmin->setVisible(true);
		FiO2->setVisible(true);
		IPAP->setVisible(false);
		EPAP->setVisible(false);
		PEEP->setVisible(false);
		ventilatorMode->setVisible(false);
		break;
	case 3:
		lmin->setVisible(false);
		FiO2->setVisible(true);
		IPAP->setVisible(true);
		EPAP->setVisible(true);
		PEEP->setVisible(false);
		ventilatorMode->setVisible(false);
		break;
	case 4:
		lmin->setVisible(false);
		FiO2->setVisible(true);
		IPAP->setVisible(false);
		EPAP->setVisible(false);
		PEEP->setVisible(true);
		ventilatorMode->setVisible(true);
		break;

	default:
		break;
	}
}

void ClinicalInformation::OnVentilatorBtn()
{
	QString displayStr;
	breathe data;

	int indexnumber = ventilatorComboBox->currentIndex();
	QString title = ventilatorComboBox->currentText();

	if (m_pLogic->isHave(title, BREATHE))
		return;

	lmin->GetValue(data.LMin);
	FiO2->GetValue(data.FiO2);
	EPAP->GetValue(data.EPAP);
	IPAP->GetValue(data.IPAP);
	PEEP->GetValue(data.PEEP);
	ventilatorMode->GetValue(data.hreathModel);
	m_pLogic->checkBreathZero(data);

	switch (indexnumber)
	{
	case 0:
		//title = u8"±Çµ¼¹Ü";
		//lmin->GetValue(data.LMin);
		displayStr = title + ": " + data.LMin + "l/min";
		break;
	case 1:
		//title = u8"ÃæÕÖÎüÑõ";
		//lmin->GetValue(data.LMin);
		displayStr = title + ": " + data.LMin + "l/min";
		break;
	case 2:
		//title = u8"¸ßÁ÷Á¿ÎüÑõ";
		//lmin->GetValue(data.LMin);
		//FiO2->GetValue(data.FiO2);
		displayStr = title + ": " + data.LMin+"l/min, FiO2 "+data.FiO2 + "%";
		break;
	case 3:
		//title = u8"ÎÞ´´ºôÎü»ú¸¨ÖúºôÎü";
		//EPAP->GetValue(data.EPAP);
		//FiO2->GetValue(data.FiO2);
		//IPAP->GetValue(data.IPAP);
		displayStr = title + ": " + "FiO2 " + data.FiO2 + "%, "+"IPAP "+ data.IPAP + "cmH2O, " + "EPAP "+ data.EPAP+"cmH2O";
		break;
	case 4:
		//title = u8"ÓÐ´´ºôÎü»ú¸¨ÖúºôÎü";
		//FiO2->GetValue(data.FiO2);
		//PEEP->GetValue(data.PEEP);
		//ventilatorMode->GetValue(data.hreathModel);
		displayStr = title + ": " + "FiO2 " + data.FiO2 + "%, " + "PEEP " + data.PEEP + "cmH2O, " + u8"ºôÎü»úÄ£Ê½ " + data.hreathModel;
		break;

	default:
		break;
	}
	
	m_pLogic->insert(title, data);
	selectDisplay* dis = new selectDisplay(BREATHE, title, displayStr, ventilatorWidget);
	ventilatorWidget->layout()->addWidget(dis);
	connect(dis, &selectDisplay::removeItme, this, &ClinicalInformation::OnDeleteItem);
}

void ClinicalInformation::OnDeleteItem(short t, QString& key)
{
	m_pLogic->remove(key, (BreatheOrMedicType)t);
}

void ClinicalInformation::OnVasoactivityCheckBox(bool b)
{
	if (b)
	{
		VasoactivityWidget->show();
	}
	else
	{
		VasoactivityWidget->hide();
	}
}

void ClinicalInformation::OnAnalgeiaCheckBox(bool b)
{
	if (b)
	{
		AnalgeiaWidget->show();
	}
	else
	{
		AnalgeiaWidget->hide();
	}
}

void ClinicalInformation::OnCalmCheckBox(bool b)
{
	if (b)
	{
		CalmWidget->show();
	}
	else
	{
		CalmWidget->hide();
	}
}

void ClinicalInformation::OnMuscleRelaxationCheckBox(bool b)
{
	if (b)
	{
		MuscleRelaxationWidget->show();
	}
	else
	{
		MuscleRelaxationWidget->hide();
	}
}

void ClinicalInformation::OnVasoactivityBtn()
{
	medicine data;
	QString title = VasoactivityCombobox->currentText();
	data.name = title;
	VasoactivityTotalVolume->GetValue(data.TotalVolume);
	VasoactivityMediumVolume->GetValue(data.MediaVolume);
	VasoactivityPumpInSpeed->GetValue(data.PumpingSpeed);
	VasoactivityPumpInVolume->GetValue(data.PumpingVolume);

	m_pLogic->checkMediantZero(data);
	if (m_pLogic->isHave(title, VASOACTIVITY))
		return;
	QString displayStr = title+": "+data.TotalVolume+"mg, "+data.MediaVolume+"ml, "+data.PumpingSpeed+"ml/h, "+data.PumpingVolume+"ug/kg/min";

	selectDisplay* dis = new selectDisplay(VASOACTIVITY, title, displayStr, ventilatorWidget);
	VasoactivityWidget->layout()->addWidget(dis);
	connect(dis, &selectDisplay::removeItme, this, &ClinicalInformation::OnDeleteItem);
	m_pLogic->insert(title, data, VASOACTIVITY);
}

void ClinicalInformation::OnAnalgeiaBtn()
{
	medicine data;
	QString title = AnalgeiaCombobox->currentText();
	data.name = title;
	AnalgeiaTotalVolume->GetValue(data.TotalVolume);
	AnalgeiaMediumVolume->GetValue(data.MediaVolume);
	AnalgeiaPumpInSpeed->GetValue(data.PumpingSpeed);
	AnalgeiaPumpInVolume->GetValue(data.PumpingVolume);
	AnalgeiaCPOTScore->GetValue(data.Score);
	m_pLogic->checkMediantZero(data);
	if (m_pLogic->isHave(title, ANALGESIA))
		return;
	QString displayStr = title + ": " + data.TotalVolume + "mg, " + data.MediaVolume + "ml, " + data.PumpingSpeed + "ml/h, " + data.PumpingVolume + "ug/kg/min, "+u8"CPOTÆÀ·Ö"+data.Score;

	selectDisplay* dis = new selectDisplay(ANALGESIA, title, displayStr, AnalgeiaWidget);
	AnalgeiaWidget->layout()->addWidget(dis);
	connect(dis, &selectDisplay::removeItme, this, &ClinicalInformation::OnDeleteItem);
	m_pLogic->insert(title, data, ANALGESIA);
}

void ClinicalInformation::OnCalmBtn()
{
	medicine data;
	QString title = CalmCombobox->currentText();
	data.name = title;
	CalmTotalVolume->GetValue(data.TotalVolume);
	CalmMediumVolume->GetValue(data.MediaVolume);
	CalmPumpInSpeed->GetValue(data.PumpingSpeed);
	CalmPumpInVolume->GetValue(data.PumpingVolume);
	CalmRASSScore->GetValue(data.Score);
	m_pLogic->checkMediantZero(data);
	if (m_pLogic->isHave(title, CALM))
		return;
	QString displayStr = title + ": " + data.TotalVolume + "mg, " + data.MediaVolume + "ml, " + data.PumpingSpeed + "ml/h, " + data.PumpingVolume + "ug/kg/min, " + u8"RASSÆÀ·Ö" + data.Score;

	selectDisplay* dis = new selectDisplay(CALM, title, displayStr, CalmWidget);
	CalmWidget->layout()->addWidget(dis);
	connect(dis, &selectDisplay::removeItme, this, &ClinicalInformation::OnDeleteItem);
	m_pLogic->insert(title, data, CALM);
}

void ClinicalInformation::OnMuscleRelaxationBtn()
{
	medicine data;
	QString title = MuscleRelaxationCombobox->currentText();
	data.name = title;
	MuscleRelaxationTotalVolume->GetValue(data.TotalVolume);
	MuscleRelaxationMediumVolume->GetValue(data.MediaVolume);
	MuscleRelaxationPumpInSpeed->GetValue(data.PumpingSpeed);
	MuscleRelaxationPumpInVolume->GetValue(data.PumpingVolume);
	MuscleRelaxationBISScore->GetValue(data.Score);
	m_pLogic->checkMediantZero(data);
	if (m_pLogic->isHave(title, MUSCLERELAXATION))
		return;
	QString displayStr = title + ": " + data.TotalVolume + "mg, " + data.MediaVolume + "ml, " + data.PumpingSpeed + "ml/h, " + data.PumpingVolume + "ug/kg/min, " + u8"BISÆÀ·Ö" + data.Score;

	selectDisplay* dis = new selectDisplay(MUSCLERELAXATION, title, displayStr, MuscleRelaxationWidget);
	MuscleRelaxationWidget->layout()->addWidget(dis);
	connect(dis, &selectDisplay::removeItme, this, &ClinicalInformation::OnDeleteItem);
	m_pLogic->insert(title, data, MUSCLERELAXATION);
}

void ClinicalInformation::OnAutoCompute()
{
	//MAP = 1 / 3£¨ÊÕËõÑ¹ - ÊæÕÅÑ¹£© + ÊæÕÅÑ¹
	QString strSystolicPressure;
	QString strDiastolicPressure;
	
	systolicPressure->GetValue(strSystolicPressure);
	diastolicPressure->GetValue(strDiastolicPressure);

	int iSystolicPressure = strSystolicPressure.toInt();
	int iDiastolicPressure = strDiastolicPressure.toInt();
	int iMap = ((iSystolicPressure - iDiastolicPressure) + iSystolicPressure) / 3;

	map->GetValueObject()->setText(QString::number(iMap, 'f', 2));
}

void ClinicalInformation::OnVentilatorYesOrNo()
{
	QRadioButton* p = qobject_cast<QRadioButton*>(sender());

	auto str = p->text();

	if (str == u8"ÊÇ")
	{
		ventilatorWidget->show();
		//emit scrollDown();
	}
	else
	{
		ventilatorWidget->hide();
	}
}

