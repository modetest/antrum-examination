#include "reportWidget.h"

reportWidget::reportWidget(QString& baseID, QString& clID, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	QPalette pal = this->palette();
	pal.setBrush(QPalette::Window, QBrush(QPixmap(":/autrum/resouce/mainImage.png")));
	this->setPalette(pal);
	this->setFixedSize(1920,1080);
	mainlay = new QVBoxLayout(this);
	QPushButton* returnPushButton = new QPushButton(u8"返回", this);
	returnPushButton->setFixedSize(110, 40);
	returnPushButton->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/button-back.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333; }");
	connect(returnPushButton, &QPushButton::clicked, this, [&](){
		this->close();
		});
	mainlay->addWidget(returnPushButton);


	QScrollArea* verticalScrollArea = new QScrollArea(this);
	verticalScrollArea->setWidgetResizable(true);
	verticalScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	verticalScrollArea->setStyleSheet("QScrollArea{border:none; background-color:transparent; } QScrollBar{background-color:#323E55;;border:none } QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background:#171f2b;}"
	);
	scrollWidget = new QWidget();
	scrollWidget->setObjectName("scrollWidget");
	scrollWidget->setStyleSheet("#scrollWidget{background-color:#171f2b;border:none}");
	verticalScrollArea->setWidget(scrollWidget);

	reportLogic* m_pLogic = new reportLogic(this);

	strBaseID = baseID;
	strClID = clID;

	m_pLogic->queryData(strBaseID.toInt(), strClID.toInt(), baseInfo, clinicalInfo, nuInfo, collectInfo);

	
	auto rootPath = QApplication::applicationDirPath();
	scrollLayout = new QVBoxLayout(scrollWidget);

	InitBaseInfo();

	InitMediantsInfo();

	InitCollectionInfo(rootPath);

	scrollLayout->addWidget(baseWidget);//基本信息
	scrollLayout->addWidget(clinicalWidget);//临床信息
	scrollLayout->addWidget(collectionWidget);//营养信息

	//保存报告
	QPushButton* saveReportButton = new QPushButton(u8"保存报告", this);
	saveReportButton->setFixedSize(110, 40);
	saveReportButton->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/button-back.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333; }");
	connect(saveReportButton, &QPushButton::clicked, this,&reportWidget::OnSaveReportBtn);
	QHBoxLayout* btnLay = new QHBoxLayout();
	btnLay->addWidget(saveReportButton);
	scrollLayout->addLayout(btnLay);

	mainlay->addWidget(verticalScrollArea);
}

reportWidget::~reportWidget()
{}

void reportWidget::OnSaveReportBtn()
{
	auto rootPath = QApplication::applicationDirPath();
	QString xmlPath = rootPath + "/template";
	QString ImgPath = rootPath + "/resultSet/" + strBaseID + "/" +strClID + "/curve.png";
	QString savePath = rootPath + "/resultSet/" + strBaseID + "/" + strClID + "/export.docx";

	QString modifiedPath = "./template/modified.xml";

	insertImageToTemplate(ImgPath);

	QFile file(modifiedPath);
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "open xml file fail. " << file.errorString();
		return;
	}
	QByteArray baContent = file.readAll();
	file.close();
	QString strAllContent = QString::fromUtf8(baContent);
	strAllContent.replace("$USCode", clinicalInfo.USCode);
	QString curTime = QDateTime::currentDateTime().toString();
	strAllContent.replace("$DataTime", curTime);
	strAllContent.replace("$PatientID", strBaseID);

	strAllContent.replace("$code", "CODE");
	strAllContent.replace("$name", baseInfo.name);
	strAllContent.replace("$age", baseInfo.age);
	strAllContent.replace("$sex", baseInfo.sex);
	strAllContent.replace("$height", baseInfo.height);
	strAllContent.replace("$weight", baseInfo.actual_weight);
	strAllContent.replace("$ht", baseInfo.ideal_weight);
	strAllContent.replace("$BMI", baseInfo.BMI);
	strAllContent.replace("$APACHE2", baseInfo.APACHE2);

	strAllContent.replace("$SOFA", baseInfo.SOFA);
	strAllContent.replace("$admission_number", baseInfo.admission_number);
	strAllContent.replace("$hospital", baseInfo.hospital);
	strAllContent.replace("$departments", baseInfo.administrative_office);
	strAllContent.replace("$ward", baseInfo.inpatient_ward);
	strAllContent.replace("$bed_number", baseInfo.bed_number);
	strAllContent.replace("$medical_team", baseInfo.medical_group);
	strAllContent.replace("$medical_leader", baseInfo.treat_group_leader);
	strAllContent.replace("$ysician", baseInfo.attending_doctor);

	strAllContent.replace("$doctor", baseInfo.bed_doctor);
	strAllContent.replace("$pher", baseInfo.acquisition_doctor);
	strAllContent.replace("$admission_time", baseInfo.admission_date);
	strAllContent.replace("$ICU_time", baseInfo.icu_date);

	strAllContent.replace("$diagnosis", baseInfo.principal_diagnosis);


	strAllContent.replace("$heart_rate", clinicalInfo.hreat_rate);
	strAllContent.replace("$rhythm", clinicalInfo.heart_Rhythm);
	strAllContent.replace("$sure ", clinicalInfo.SBP);
	strAllContent.replace("$diastolic_", clinicalInfo.DBP);
	strAllContent.replace("$MAP", clinicalInfo.MAP);
	strAllContent.replace("$SPO", clinicalInfo.SPO2);
	strAllContent.replace("$RR", clinicalInfo.RR);
	strAllContent.replace("$PI", clinicalInfo.PI);

	//组合字符
	QString medicants;
	if (clinicalInfo.vecActivity.size() > 0)
	{
		medicants = clinicalInfo.vecActivity[0].name + ": " + clinicalInfo.vecActivity[0].TotalVolume + "mg, " + clinicalInfo.vecActivity[0].MediaVolume + "ml, " + clinicalInfo.vecActivity[0].PumpingSpeed + "ml/h, " + clinicalInfo.vecActivity[0].PumpingVolume + "ug/kg/min, " + u8"评分" + clinicalInfo.vecActivity[0].Score;
	}

	for (size_t i = 1; i < clinicalInfo.vecActivity.size(); i++)
	{
		medicants = medicants + "; " + clinicalInfo.vecActivity[i].name + ": " + clinicalInfo.vecActivity[i].TotalVolume + "mg, " + clinicalInfo.vecActivity[i].MediaVolume + "ml, " + clinicalInfo.vecActivity[i].PumpingSpeed + "ml/h, " + clinicalInfo.vecActivity[i].PumpingVolume + "ug/kg/min, " + u8"评分" + clinicalInfo.vecActivity[i].Score;
	}
	QString nuStr;
	if (nuInfo.size() > 0)
	{
		nuStr = nuInfo[0].Formula + ", " + nuInfo[0].Volume + "ml, " + nuInfo[0].Kcal + "kcal, " + nuInfo[0].Protein + u8"g, 开始时间：" + nuInfo[0].StartTime;
	}

	for (size_t i = 1; i < nuInfo.size(); i++)
	{
		nuStr = nuInfo[i].Formula + ", " + nuInfo[0].Volume + "ml, " + nuInfo[0].Kcal + "kcal, " + nuInfo[0].Protein + u8"g, 开始时间：" + nuInfo[0].StartTime;
	}

	if (clinicalInfo.vecBreathe.size() > 0)
	{
		strAllContent.replace("$respiratory_s", clinicalInfo.vecBreathe[0].pattern);
	}
	else
	{
		strAllContent.replace("$respiratory_s", u8"无");
	}
	strAllContent.replace("$medicine", medicants);
	strAllContent.replace("$nutrition", nuStr);

	strAllContent.replace("$collection_time", collectInfo.acTime);
	strAllContent.replace("$duration", collectInfo.cycleTime);
	strAllContent.replace("$area", collectInfo.diastoleArea);//舒张
	strAllContent.replace("$a", collectInfo.contractArea);//收缩
	strAllContent.replace("$total_count", collectInfo.count);

	//计算
	//1/3(平均最大面积 - 平均最小面积) | 平均最大面积
	auto diastole = collectInfo.diastoleArea.toFloat();
	auto contract = collectInfo.contractArea.toFloat();
	auto count = collectInfo.count.toInt();

	auto aca = (diastole - contract) / diastole;
	float acf = (float)count / 3.0f;
	auto MI = aca * acf;
	strAllContent.replace("$MI", QString::number(MI, 'f', 2));
	//Volume = 27.0 + 14.6 * CSA − 1.28 * age
	float volume = 27.0 + 14.6 * diastole - 1.28 * baseInfo.age.toInt();
	strAllContent.replace("$GRV", QString::number(volume, 'f', 2));

	//输入
	 
	strAllContent.replace("$curve_rhythm", rhythm->GetValueObject()->text());
	strAllContent.replace("$check", checkName->GetValueObject()->text());
	strAllContent.replace("$record", recordName->GetValueObject()->text());
	strAllContent.replace("$uditing", reviewName->GetValueObject()->text());

	QFile newFile(savePath);
	if (!newFile.open(QIODevice::WriteOnly))
	{
		qDebug() << "file open fail." << newFile.errorString();
		return;
	}

	newFile.write(strAllContent.toUtf8());
	newFile.close();

	this->setResult(10);
	this->close();
}

void reportWidget::InitBaseInfo()
{
	baseWidget = new QWidget(this);
	QVBoxLayout* baseLayout = new QVBoxLayout(baseWidget);
	baseLayout->setContentsMargins(0, 0, 0, 0);

	QLabel* title = new QLabel(baseWidget);
	title->setText(u8" 基本信息");
	title->setFixedHeight(48);
	title->setStyleSheet("QLabel{background-color: #323E55;color:white;font-size:30px;}");

	QGridLayout* itemData = new QGridLayout();
	itemData->setContentsMargins(54, 0, 54, 85);
	itemData->setVerticalSpacing(0);
	itemData->setHorizontalSpacing(164);

	InformationItem* serialNumber = new InformationItem(nullptr, this);
	serialNumber->InitInformationItem(u8"编号：", "");
	serialNumber->ReadOnly(true);

	InformationItem* name = new InformationItem(nullptr, this);
	name->InitInformationItem(u8"姓名：", "");
	name->ReadOnly(true);
	name->SetValueData(baseInfo.name);


	InformationItem* age = new InformationItem(nullptr, this);
	age->InitInformationItem(u8"年龄：", "");
	age->ReadOnly(true);
	age->SetValueData(baseInfo.age);

	InformationItem* gender = new InformationItem(nullptr, this);
	gender->InitInformationItem(u8"性别：", "");
	gender->ReadOnly(true);
	gender->SetValueData(baseInfo.sex);

	InformationItem* stature = new InformationItem(nullptr, this);
	stature->InitInformationItem(u8"身高：", "cm");
	stature->ReadOnly(true);
	stature->SetValueData(baseInfo.height);

	InformationItem* weight = new InformationItem(nullptr, this);
	weight->InitInformationItem(u8"体重：", "kg");
	weight->ReadOnly(true);
	weight->SetValueData(baseInfo.actual_weight);


	InformationItem* idealBodyWeight = new InformationItem(nullptr, this);
	idealBodyWeight->InitInformationItem(u8"理想体重：", "kg");
	idealBodyWeight->ReadOnly(true);
	idealBodyWeight->SetValueData(baseInfo.ideal_weight);


	InformationItem* estimationBodySurfaceArea = new InformationItem(nullptr, this);
	char16_t square = 0xB2;
	estimationBodySurfaceArea->InitInformationItem(u8"体表面积估值（BSI）：", "m" + QString::fromUtf16(&square, 1));
	estimationBodySurfaceArea->ReadOnly(true);
	estimationBodySurfaceArea->SetValueData(baseInfo.BSI);

	InformationItem* bmi = new InformationItem(nullptr, this);
	bmi->InitInformationItem(u8"BMI：", "");
	bmi->ReadOnly(true);
	bmi->SetValueData(baseInfo.BMI);

	InformationItem* apache2Score = new InformationItem(nullptr, this);
	apache2Score->InitInformationItem(u8"APACHE2评分：", u8"分");
	apache2Score->ReadOnly(true);
	apache2Score->SetValueData(baseInfo.APACHE2);

	InformationItem* sofaScore = new InformationItem(nullptr, this);
	sofaScore->InitInformationItem(u8"SOFA评分：", u8"分");
	sofaScore->ReadOnly(true);
	sofaScore->SetValueData(baseInfo.SOFA);

	InformationItem* admissionNumber = new InformationItem(nullptr, this);
	admissionNumber->InitInformationItem(u8"住院号：", "");
	admissionNumber->ReadOnly(true);
	admissionNumber->SetValueData(baseInfo.admission_number);

	InformationItem* hospital = new InformationItem(nullptr, this);
	hospital->InitInformationItem(u8"医院：", "");
	hospital->SetValueWidth(934);
	hospital->GetValueObject()->setMaxLength(50);
	hospital->ReadOnly(true);
	hospital->SetValueData(baseInfo.hospital);

	InformationItem* administrativeOffice = new InformationItem(nullptr, this);
	administrativeOffice->InitInformationItem(u8"科室：", "");
	administrativeOffice->ReadOnly(true);
	administrativeOffice->SetValueData(baseInfo.administrative_office);


	InformationItem* inpatientWard = new InformationItem(nullptr, this);
	inpatientWard->InitInformationItem(u8"病区：", "");
	inpatientWard->SetValueWidth(934);
	inpatientWard->ReadOnly(true);
	inpatientWard->SetValueData(baseInfo.inpatient_ward);

	InformationItem* bedNumber = new InformationItem(nullptr, this);
	bedNumber->InitInformationItem(u8"床号：", "");
	bedNumber->ReadOnly(true);
	bedNumber->SetValueData(baseInfo.bed_number);

	InformationItem* medicalGroup = new InformationItem(nullptr, this);
	medicalGroup->InitInformationItem(u8"医疗组：", "");
	medicalGroup->SetValueWidth(934);
	medicalGroup->ReadOnly(true);
	medicalGroup->SetValueData(baseInfo.medical_group);

	InformationItem* medicalTeamLeader = new InformationItem(nullptr, this);
	medicalTeamLeader->InitInformationItem(u8"医疗组长：", "");
	medicalTeamLeader->ReadOnly(true);
	medicalTeamLeader->SetValueData(baseInfo.treat_group_leader);

	InformationItem* attendingDoctor = new InformationItem(nullptr, this);
	attendingDoctor->InitInformationItem(u8"主治医生：", "");
	attendingDoctor->ReadOnly(true);
	attendingDoctor->SetValueData(baseInfo.attending_doctor);

	InformationItem* bedTubeDoctor = new InformationItem(nullptr, this);
	bedTubeDoctor->InitInformationItem(u8"管床医生：", "");
	bedTubeDoctor->ReadOnly(true);
	bedTubeDoctor->SetValueData(baseInfo.bed_doctor);

	InformationItem* doctorTookPicture = new InformationItem(nullptr, this);
	doctorTookPicture->InitInformationItem(u8"采图医生：", "");
	doctorTookPicture->ReadOnly(true);
	doctorTookPicture->SetValueData(baseInfo.acquisition_doctor);

	QHBoxLayout* sourcePatientsBox = new QHBoxLayout();
	sourcePatientsBox->setSpacing(24);
	sourcePatientsBox->setContentsMargins(2, 0, 2, 0);
	QLabel* sourcePatientsTitle = new QLabel(u8"病人来源：");
	sourcePatientsTitle->setStyleSheet("width:89px;height:42px;color:#ffffff;font-size:28px;");
	QRadioButton* emergencyAdmisionRadio = new QRadioButton(u8"急诊入院");
	emergencyAdmisionRadio->setStyleSheet("color:#ffffff;font-size:23px;");
	QRadioButton* generalWardRadio = new QRadioButton(u8"普通病房");
	generalWardRadio->setStyleSheet("color:#ffffff;font-size:23px;");
	QRadioButton* operationRadio = new QRadioButton(u8"手术");
	operationRadio->setStyleSheet("color:#ffffff;font-size:23px;");
	QLineEdit* sourcePatientsValue = new QLineEdit;
	sourcePatientsValue->setStyleSheet("height:42px;color:#ffffff;font-size:23px;background-color:#354052;border-style:outset");
	sourcePatientsValue->setFixedWidth(528);
	sourcePatientsValue->setReadOnly(true);
	sourcePatientsValue->setText(baseInfo.patients_source);

	sourcePatientsBox->addWidget(sourcePatientsTitle);
	sourcePatientsBox->addWidget(emergencyAdmisionRadio);
	sourcePatientsBox->addWidget(generalWardRadio);
	sourcePatientsBox->addWidget(operationRadio);
	sourcePatientsBox->addWidget(sourcePatientsValue);

	InformationItem* admissionTime = new InformationItem(nullptr, this);
	admissionTime->GetValueObject()->setMaxLength(50);
	admissionTime->InitInformationItem(u8"入院时间：", "");
	admissionTime->ReadOnly(true);
	admissionTime->SetValueData(baseInfo.admission_date);

	InformationItem* icuAdmission = new InformationItem(nullptr, this);
	icuAdmission->GetValueObject()->setMaxLength(50);
	icuAdmission->InitInformationItem(u8"ICU时间：", "");
	icuAdmission->ReadOnly(true);
	icuAdmission->SetValueData(baseInfo.icu_date);


	QHBoxLayout* diagnosisLayout = new QHBoxLayout();
	diagnosisLayout->setSpacing(24);
	diagnosisLayout->setContentsMargins(2, 0, 2, 0);
	QLabel* diagnosisTitle = new QLabel(u8"主要诊断：");
	diagnosisTitle->setFixedSize(147, 80);
	diagnosisTitle->setStyleSheet("color:#ffffff;font-size:28px;");
	QTextEdit* diagnosisValue = new QTextEdit;
	diagnosisValue->setStyleSheet("color:#ffffff;font-size:23px;background-color:#354052;border-style:outset");
	diagnosisValue->setReadOnly(true);
	diagnosisValue->setPlainText(baseInfo.principal_diagnosis);

	diagnosisLayout->addWidget(diagnosisTitle);
	diagnosisLayout->addWidget(diagnosisValue);

	itemData->addWidget(serialNumber, 0, 0);
	itemData->addWidget(name, 0, 1);
	itemData->addWidget(age, 0, 2);

	itemData->addWidget(gender, 1, 0);
	itemData->addWidget(stature, 1, 1);
	itemData->addWidget(weight, 1, 2);
	itemData->addWidget(idealBodyWeight, 2, 0);
	itemData->addWidget(estimationBodySurfaceArea, 2, 1);
	itemData->addWidget(bmi, 2, 2);
	itemData->addWidget(apache2Score, 3, 0);
	itemData->addWidget(sofaScore, 3, 1);
	itemData->addWidget(admissionNumber, 3, 2);
	itemData->addWidget(hospital, 4, 0, 1, 2);
	itemData->addWidget(administrativeOffice, 4, 2);
	itemData->addWidget(inpatientWard, 5, 0, 1, 2);
	itemData->addWidget(bedNumber, 5, 2);
	itemData->addWidget(medicalGroup, 6, 0, 1, 2);
	itemData->addWidget(medicalTeamLeader, 6, 2);
	itemData->addWidget(attendingDoctor, 7, 0);
	itemData->addWidget(bedTubeDoctor, 7, 1);
	itemData->addWidget(doctorTookPicture, 7, 2);
	itemData->addLayout(sourcePatientsBox, 8, 0, 1, 2);
	itemData->addWidget(admissionTime, 9, 0);
	itemData->addWidget(icuAdmission, 10, 0);
	itemData->addLayout(diagnosisLayout, 11, 0, 2, 3);

	emergencyAdmisionRadio->setChecked(true);

	baseLayout->addWidget(title);
	baseLayout->addLayout(itemData);
}

void reportWidget::InitMediantsInfo()
{
	clinicalWidget = new QWidget(this);
	QVBoxLayout* clinicallay = new QVBoxLayout(clinicalWidget);
	clinicallay->setMargin(0);

	QLabel* title = new QLabel(clinicalWidget);
	title->setText(u8" 临床信息");
	title->setFixedHeight(48);
	title->setStyleSheet("QLabel{background-color: #323E55;color:white;font-size:30px;}");

	QGridLayout* itemData = new QGridLayout();
	itemData->setContentsMargins(54, 0, 54, 85);
	itemData->setVerticalSpacing(0);
	itemData->setHorizontalSpacing(164);

	InformationItem* heartRate = new InformationItem(nullptr, this);
	heartRate->InitInformationItem(u8"心率：", u8"次/分");
	heartRate->ReadOnly(true);
	heartRate->SetValueData(clinicalInfo.hreat_rate);

	InformationItem* rhythmHeart = new InformationItem(nullptr, this);
	rhythmHeart->InitInformationItem(u8"心律：", "");
	rhythmHeart->ReadOnly(true);
	rhythmHeart->SetValueData(clinicalInfo.heart_Rhythm);

	InformationItem* rr = new InformationItem(nullptr, this);
	rr->InitInformationItem(u8"RR：", u8"次/分");
	rr->ReadOnly(true);
	rr->SetValueData(clinicalInfo.RR);

	InformationItem* systolicPressure = new InformationItem(nullptr, this);
	systolicPressure->InitInformationItem(u8"收缩压：", "mmHg");
	systolicPressure->ReadOnly(true);
	systolicPressure->SetValueData(clinicalInfo.SBP);

	InformationItem* diastolicPressure = new InformationItem(nullptr, this);
	diastolicPressure->InitInformationItem(u8"舒张压：", "mmHg");
	diastolicPressure->ReadOnly(true);
	diastolicPressure->SetValueData(clinicalInfo.DBP);
	
	InformationItem* map = new InformationItem(nullptr, this);
	map->InitInformationItem(u8"MAP：", "mmHg");
	map->ReadOnly(true);
	map->SetValueData(clinicalInfo.MAP);

	InformationItem* pi = new InformationItem(nullptr, this);
	pi->InitInformationItem(u8"PI：", "");
	pi->ReadOnly(true);
	pi->SetValueData(clinicalInfo.MAP);

	InformationItem* spo = new InformationItem(nullptr, this);
	spo->InitInformationItem(u8"SPO2：", "%");
	spo->ReadOnly(true);
	spo->SetValueData(clinicalInfo.SPO2);

	InformationItem* ultrasonicNumber = new InformationItem(nullptr, this);
	ultrasonicNumber->InitInformationItem(u8"超声编号：", "");
	ultrasonicNumber->ReadOnly(true);
	ultrasonicNumber->SetValueData(clinicalInfo.USCode);


	/*QHBoxLayout* HStartTimelayout = new  QHBoxLayout();
	QLabel* StartTimeLabel = new QLabel(u8"开始时间：");
	StartTimeLabel->setStyleSheet("width:89px;height:42px;color:#ffffff;font-size:28px;");
	QDateTimeEdit* StartTime = new QDateTimeEdit();
	StartTime->setDateTime(QDateTime::currentDateTime());
	StartTime->setFixedSize(305, 42);
	StartTime->setStyleSheet("color:#ffffff;font-size:23px;background-color:#354052;border-style:outset;alternate-background-color:rgb(204, 229, 255);");
	HStartTimelayout->addWidget(StartTimeLabel);
	HStartTimelayout->addWidget(StartTime);*/


	itemData->addWidget(heartRate, 0, 0);
	itemData->addWidget(rhythmHeart, 0, 1);
	itemData->addWidget(rr, 0, 2);
	itemData->addWidget(systolicPressure, 1, 0);
	itemData->addWidget(diastolicPressure, 1, 1);
	itemData->addWidget(map, 1, 2);
	itemData->addWidget(pi, 2, 0);
	itemData->addWidget(spo, 2, 1);
	itemData->addWidget(ultrasonicNumber, 2, 2);

	clinicallay->addWidget(title);
	clinicallay->addLayout(itemData);



	QString medicants;
	if (clinicalInfo.vecActivity.size() > 0)
	{
		medicants = clinicalInfo.vecActivity[0].name + ": " + clinicalInfo.vecActivity[0].TotalVolume + "mg, " + clinicalInfo.vecActivity[0].MediaVolume + "ml, " + clinicalInfo.vecActivity[0].PumpingSpeed + "ml/h, " + clinicalInfo.vecActivity[0].PumpingVolume + "ug/kg/min, " + u8"评分" + clinicalInfo.vecActivity[0].Score;
	}

	for (size_t i = 1; i < clinicalInfo.vecActivity.size(); i++)
	{
		medicants = medicants + "; " + clinicalInfo.vecActivity[i].name + ": " + clinicalInfo.vecActivity[i].TotalVolume + "mg, " + clinicalInfo.vecActivity[i].MediaVolume + "ml, " + clinicalInfo.vecActivity[i].PumpingSpeed + "ml/h, " + clinicalInfo.vecActivity[i].PumpingVolume + "ug/kg/min, " + u8"评分" + clinicalInfo.vecActivity[i].Score;
	}

	medicants = u8"药物:" + medicants;

	QLabel* medicatsLabel = new QLabel(this);
	medicatsLabel->setWordWrap(true);
	medicatsLabel->setText(medicants);
	medicatsLabel->setStyleSheet("QLabel{width:89px; height:42px; color:#ffffff; font-size:28px;}");
	clinicallay->addWidget(medicatsLabel);


	QString nuStr;
	if (nuInfo.size() > 0)
	{
		nuStr = nuInfo[0].name +": " + nuInfo[0].Formula + ", " + nuInfo[0].Volume + "ml, " + nuInfo[0].Kcal + "kcal, " + nuInfo[0].Protein + u8"g, 开始时间：" + nuInfo[0].StartTime;
	}

	for (size_t i = 1; i < nuInfo.size(); i++)
	{
		nuStr = nuStr+ "; " + nuInfo[i].name + ": " + nuInfo[i].Formula + ", " + nuInfo[i].Volume + "ml, " + nuInfo[i].Kcal + "kcal, " + nuInfo[i].Protein + u8"g, 开始时间：" + nuInfo[i].StartTime;
	}

	nuStr = u8"营养:" + nuStr;

	QLabel* nuLabel = new QLabel(this);
	nuLabel->setWordWrap(true);
	nuLabel->setText(nuStr);
	nuLabel->setStyleSheet("QLabel{width:89px; height:42px; color:#ffffff; font-size:28px;}");
	clinicallay->addWidget(nuLabel);
}

void reportWidget::InitCollectionInfo(QString& rootPath)
{
	collectionWidget = new QWidget(this);
	QVBoxLayout* collectionlay = new QVBoxLayout(collectionWidget);
	collectionlay->setMargin(0);

	InformationItem* diastoleArea = new InformationItem(nullptr, this);
	diastoleArea->InitInformationItem(u8"平均舒张面积：", u8"cm2");
	diastoleArea->ReadOnly(true);
	diastoleArea->SetValueData(collectInfo.diastoleArea);

	InformationItem* contractArea = new InformationItem(nullptr, this);
	contractArea->InitInformationItem(u8"平均收缩面积：", u8"cm2");
	contractArea->ReadOnly(true);
	contractArea->SetValueData(collectInfo.contractArea);

	InformationItem* count = new InformationItem(nullptr, this);
	count->InitInformationItem(u8"周期次数：", u8"次");
	count->ReadOnly(true);
	count->SetValueData(collectInfo.count);

	InformationItem* startTime = new InformationItem(nullptr, this);
	startTime->GetValueObject()->setMaxLength(50);
	startTime->InitInformationItem(u8"采集时间：", u8"");
	startTime->ReadOnly(true);
	startTime->SetValueData(collectInfo.acTime);

	InformationItem* cycleTime = new InformationItem(nullptr, this);
	cycleTime->InitInformationItem(u8"采集时常：", u8"s");
	cycleTime->ReadOnly(true);
	cycleTime->SetValueData(collectInfo.cycleTime);

	QGridLayout* collectLay = new QGridLayout();

	collectLay->addWidget(diastoleArea, 0, 0);
	collectLay->addWidget(contractArea, 0, 1);
	collectLay->addWidget(count, 0, 2);
	collectLay->addWidget(startTime, 1, 0);
	collectLay->addWidget(cycleTime, 1, 1);

	collectionlay->addLayout(collectLay);

	
	QLabel* imageLabel = new QLabel(this);
	imageLabel->setFixedSize(1900, 600);

	QString ImgPath = rootPath + "/resultSet/" + strBaseID + "/" + strClID + "/curve.png";
	QPixmap pixImage;
	pixImage.load(ImgPath);
	imageLabel->setPixmap(pixImage);
	collectionlay->addWidget(imageLabel);


	//计算
  //1/3(平均最大面积 - 平均最小面积) | 平均最大面积
	auto diastole = collectInfo.diastoleArea.toFloat();
	auto contract = collectInfo.contractArea.toFloat();
	auto toltalCount = collectInfo.count.toInt();

	auto aca = (diastole - contract) / diastole;
	float acf = (float)toltalCount / 3.0f;
	auto MIFloat = aca * acf;
	//Volume = 27.0 + 14.6 * CSA − 1.28 * age
	float volume = 27.0 + 14.6 * diastole - 1.28 * baseInfo.age.toInt();

	InformationItem* MI = new InformationItem(nullptr, this);
	MI->InitInformationItem(u8"MI：", u8"");
	MI->ReadOnly(true);
	MI->SetValueData(QString::number(MIFloat, 'f', 2));

	InformationItem* GRV = new InformationItem(nullptr, this);
	GRV->InitInformationItem(u8"估测GRV：", u8"ml");
	GRV->ReadOnly(true);
	GRV->SetValueData(QString::number(volume, 'f', 2));


	//输入
	rhythm = new InformationItem(nullptr, this);
	rhythm->InitInformationItem(u8"胃肠自主神经节律：", u8"");
	rhythm->GetValueObject()->setMaxLength(20);
	checkName = new InformationItem(nullptr, this);
	checkName->InitInformationItem(u8"检查：", u8"");
	checkName->GetValueObject()->setMaxLength(10);

	recordName = new InformationItem(nullptr, this);
	recordName->InitInformationItem(u8"记录：", u8"");
	recordName->GetValueObject()->setMaxLength(10);

	reviewName = new InformationItem(nullptr, this);
	reviewName->InitInformationItem(u8"审核：", u8"");
	recordName->GetValueObject()->setMaxLength(10);

	QGridLayout* boLay = new QGridLayout();
	boLay->addWidget(MI, 0, 0);
	boLay->addWidget(GRV, 0, 1);
	boLay->addWidget(rhythm, 0, 2);
	boLay->addWidget(checkName, 1, 0);
	boLay->addWidget(recordName, 1, 1);
	boLay->addWidget(reviewName, 1, 2);
	
	collectionlay->addLayout(boLay);
}

void reportWidget::insertImageToTemplate(QString& imgPath)
{
	// 创建XML文档对象
	QString xmlPath = "./template/template.xml";
	QString modifiedPath = "./template/modified.xml";

	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(xmlPath.toStdString().c_str()) == tinyxml2::XML_SUCCESS)
	{
		qDebug() << u8"加载XML文件成功";

		// 获取根节点
		tinyxml2::XMLElement* root = doc.RootElement();

		print_child_nodes(root, imgPath);

		if (doc.SaveFile(modifiedPath.toStdString().c_str()) != tinyxml2::XML_SUCCESS)
		{
			qDebug() << u8"保存失败";
		}
	}
	else
	{
		qDebug() << u8"加载XML文件失败";
	}
}

QString reportWidget::ImagToBase64(QString& strImagePath)
{
	QImage image(strImagePath);
	QByteArray ba;
	QBuffer buf(&ba);
	image.save(&buf, "png");
	QString str = ba.toBase64();
	return str;
}

void reportWidget::print_child_nodes(tinyxml2::XMLNode* root, QString& imgPath)
{
	for (tinyxml2::XMLNode* child = root->FirstChild(); child != nullptr; child = child->NextSibling()) {
		// 根据需要处理不同的节点类型
		if (child->ToElement()) {
			// 元素节点
			tinyxml2::XMLElement* element = child->ToElement();
			const char* name = element->Name();
			std::string tempStr(name);
			if (tempStr == "pkg:binaryData")
			{
				element->SetText(ImagToBase64(imgPath).toStdString().c_str());
				return;
			}

			// 递归遍历子元素
			print_child_nodes(child, imgPath);
		}
	}
}

