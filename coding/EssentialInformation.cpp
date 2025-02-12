#include "EssentialInformation.h"

EssentialInformation::EssentialInformation(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);
	QPalette pal(this->palette());
	QColor color("#283141");
	pal.setColor(QPalette::Window, color);
	this->setAutoFillBackground(true);
	this->setPalette(pal);

	mainlay = new QVBoxLayout(this);
	//mainlay->setMargin(0);
	mainlay->setContentsMargins(0, 0, 0, 9);
	/*QPalette qPalette;
	qPalette.setBrush(QPalette::Window,QColor(40,49,65,1));
	this->setPalette(qPalette);*/
	//this->setStyleSheet("QWidget{background-color:#283141;border-radius: 9px;}");
	

	title = new QLabel();
	title->setText(u8" 基本信息");
	title->setFixedHeight(48);
	title->setStyleSheet("QLabel{background-color: #323E55;color:white;font-size:30px;}");

	itemData = new QGridLayout();
	itemData->setContentsMargins(54, 0, 54, 85);
	itemData->setVerticalSpacing(0);
	itemData->setHorizontalSpacing(164);

	QIntValidator* intValidator = new QIntValidator(1, 999, this);
	QDoubleValidator* doubleValidator = new QDoubleValidator(1, 99999,2, this);

	serialNumber = new InformationItem(nullptr,this);
	serialNumber->InitInformationItem(u8"编号：","");
	serialNumber->ReadOnly(true);

	name = new InformationItem(nullptr, this);
	name->InitInformationItem(u8"姓名：", "");
	age = new InformationItem(intValidator, this);
	age->InitInformationItem(u8"年龄：", "");


	QHBoxLayout* genderLayout = new QHBoxLayout();
	QLabel* genderLabel = new QLabel(u8"性别：");
	genderLabel->setStyleSheet("width:89px;height:42px;color:#ffffff;font-size:28px;");
	gender = new QComboBox(this);
	gender->setStyleSheet("width:288px;height:42px;color:#ffffff;font-size:23px;background-color:#354052;border-style:outset");
	gender->addItem(u8"男",1);
	gender->addItem(u8"女", 2);
	genderLayout->addWidget(genderLabel);
	genderLayout->addWidget(gender);

	stature = new InformationItem(intValidator, this);
	stature->InitInformationItem(u8"身高：", "cm");
	weight = new InformationItem(intValidator, this);
	weight->InitInformationItem(u8"体重：", "kg");

	
	idealBodyWeight = new InformationItem(doubleValidator, this);
	idealBodyWeight->InitInformationItem(u8"理想体重：", "kg");
	idealBodyWeight->ReadOnly(true);

	estimationBodySurfaceArea = new InformationItem(doubleValidator, this);
	char16_t square = 0xB2;
	estimationBodySurfaceArea->InitInformationItem(u8"体表面积估值（BSI）：", "m" + QString::fromUtf16(&square, 1));
	estimationBodySurfaceArea->ReadOnly(true);

	bmi = new InformationItem(doubleValidator, this);
	bmi->InitInformationItem(u8"BMI：", "");
	bmi->ReadOnly(true);

	apache2Score = new InformationItem(intValidator, this);
	apache2Score->InitInformationItem(u8"APACHE2评分：", u8"分");
	sofaScore = new InformationItem(intValidator, this);
	sofaScore->InitInformationItem(u8"SOFA评分：", u8"分");
	admissionNumber = new InformationItem(nullptr, this);
	admissionNumber->InitInformationItem(u8"住院号：", "");
	hospital = new InformationItem(nullptr, this);
	hospital->InitInformationItem(u8"医院：", "");
	hospital->SetValueWidth(934);
	hospital->GetValueObject()->setMaxLength(50);

	administrativeOffice = new InformationItem(nullptr, this);
	administrativeOffice->InitInformationItem(u8"科室：", "");
	inpatientWard = new InformationItem(nullptr, this);
	inpatientWard->InitInformationItem(u8"病区：", "");
	inpatientWard->SetValueWidth(934);
	bedNumber = new InformationItem(nullptr, this);
	bedNumber->InitInformationItem(u8"床号：", "");
	medicalGroup = new InformationItem(nullptr, this);
	medicalGroup->InitInformationItem(u8"医疗组：", "");
	medicalGroup->SetValueWidth(934);
	medicalTeamLeader = new InformationItem(nullptr, this);
	medicalTeamLeader->InitInformationItem(u8"医疗组长：", "");
	attendingDoctor = new InformationItem(nullptr, this);
	attendingDoctor->InitInformationItem(u8"主治医生：", "");
	bedTubeDoctor = new InformationItem(nullptr, this);
	bedTubeDoctor->InitInformationItem(u8"管床医生：", "");
	doctorTookPicture = new InformationItem(nullptr, this);
	doctorTookPicture->InitInformationItem(u8"采图医生：", "");

	sourcePatientsBox = new QHBoxLayout;
	sourcePatientsBox->setSpacing(24);
	sourcePatientsBox->setContentsMargins(2,0,2,0);
	sourcePatientsTitle = new QLabel(u8"病人来源：");
	sourcePatientsTitle->setStyleSheet("width:89px;height:42px;color:#ffffff;font-size:28px;");
	emergencyAdmisionRadio = new QRadioButton(u8"急诊入院");
	emergencyAdmisionRadio->setStyleSheet("color:#ffffff;font-size:23px;");
	generalWardRadio = new QRadioButton(u8"普通病房");
	generalWardRadio->setStyleSheet("color:#ffffff;font-size:23px;");
	operationRadio = new QRadioButton(u8"手术");
	operationRadio->setStyleSheet("color:#ffffff;font-size:23px;");
	sourcePatientsValue = new QLineEdit;
	sourcePatientsValue->setStyleSheet("height:42px;color:#ffffff;font-size:23px;background-color:#354052;border-style:outset");
	sourcePatientsValue->setFixedWidth(528);
	sourcePatientsBox->addWidget(sourcePatientsTitle);
	sourcePatientsBox->addWidget(emergencyAdmisionRadio);
	sourcePatientsBox->addWidget(generalWardRadio);
	sourcePatientsBox->addWidget(operationRadio);
	sourcePatientsBox->addWidget(sourcePatientsValue);

	admissionTimeLayout = new QHBoxLayout();
	admissionTimeLayout->setSpacing(24);
	admissionTimeLayout->setContentsMargins(2, 20, 2, 10);
	admissionTimeTitle = new QLabel(u8"入院时间：");
	admissionTimeTitle->setFixedWidth(145);
	admissionTimeTitle->setStyleSheet("color:#ffffff;font-size:28px;");
	admissionTimeEdit = new QDateTimeEdit;
	admissionTimeEdit->setStyleSheet("color:#ffffff;font-size:23px;background-color:#354052;border-style:outset;alternate-background-color:rgb(204, 229, 255);");
	admissionTimeEdit->setDateTime(QDateTime::currentDateTime());
	admissionTimeEdit->setCalendarPopup(true);
	admissionTimeEdit->setFixedHeight(42);
	admissionTimeLayout->addWidget(admissionTimeTitle);
	admissionTimeLayout->addWidget(admissionTimeEdit);
	admissionTimeLayout->addStretch();

	icuAdmissionTimeLayout = new QHBoxLayout();
	icuAdmissionTimeLayout->setSpacing(24);
	icuAdmissionTimeLayout->setAlignment(Qt::AlignLeft);
	icuAdmissionTimeLayout->setContentsMargins(2, 10, 2, 20);
	icuAdmissionTimeTitle = new QLabel(u8"入ICU时间：");
	icuAdmissionTimeTitle->setFixedWidth(145);
	icuAdmissionTimeTitle->setStyleSheet("color:#ffffff;font-size:28px;");
	icuAdmissionTimeEdit = new QDateTimeEdit;
	icuAdmissionTimeEdit->setStyleSheet("color:#ffffff;font-size:23px;background-color:#354052;border-style:outset;alternate-background-color:rgb(204, 229, 255);");
	icuAdmissionTimeEdit->setDateTime(QDateTime::currentDateTime());
	icuAdmissionTimeEdit->setCalendarPopup(true);
	icuAdmissionTimeEdit->setFixedHeight(42);
	icuAdmissionTimeLayout->addWidget(icuAdmissionTimeTitle);
	icuAdmissionTimeLayout->addWidget(icuAdmissionTimeEdit);
	icuAdmissionTimeLayout->addStretch();

	diagnosisLayout = new QHBoxLayout();
	diagnosisLayout->setSpacing(24);
	diagnosisLayout->setContentsMargins(2, 0, 2, 0);
	diagnosisTitle = new QLabel(u8"主要诊断：");
	diagnosisTitle->setFixedSize(147,80);
	diagnosisTitle->setStyleSheet("color:#ffffff;font-size:28px;");
	diagnosisValue = new QTextEdit;
	diagnosisValue->setStyleSheet("color:#ffffff;font-size:23px;background-color:#354052;border-style:outset");
	diagnosisLayout->addWidget(diagnosisTitle);
	diagnosisLayout->addWidget(diagnosisValue);

	itemData->addWidget(serialNumber,0,0);
	itemData->addWidget(name,0,1);
	itemData->addWidget(age,0,2);

	itemData->addLayout(genderLayout, 1, 0);
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
	itemData->addLayout(sourcePatientsBox, 8, 0,1,2);
	itemData->addLayout(admissionTimeLayout, 9, 0);
	itemData->addLayout(icuAdmissionTimeLayout, 10, 0);
	itemData->addLayout(diagnosisLayout, 11, 0, 2, 3);

	emergencyAdmisionRadio->setChecked(true);

	//save button
	saveButton = new QPushButton();
	saveButton->setText(u8"保存");
	saveButton->setFixedSize(QSize(110, 40));
	saveButton->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/button-back.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333; }");

	mainlay->addWidget(title);
	mainlay->addLayout(itemData);
	mainlay->addWidget(saveButton,1, Qt::AlignCenter);

	connect(saveButton, &QPushButton::clicked, this, &EssentialInformation::OnSaveBtnOn);

	connect(weight->GetValueObject(), &QLineEdit::textChanged, this, &EssentialInformation::OnAutoCompute);
	connect(stature->GetValueObject(), &QLineEdit::textChanged, this, &EssentialInformation::OnAutoCompute);

	m_pLogic = new essentialLogic(this);
}

EssentialInformation::~EssentialInformation()
{}
void EssentialInformation::SetData(int baseID)
{
	this->baseID = baseID;
	baseInformation base;
	m_pLogic->SetBaseID(baseID);
	m_pLogic->QueryPatientInfoData(&base);
	serialNumber->SetValueData(base.number);
	name->SetValueData(base.name);
	age->SetValueData(base.age);
	if (base.sex == u8"男")
	{
		gender->setCurrentIndex(0);
	}
	else
	{
		gender->setCurrentIndex(1);
	}

	stature->SetValueData(base.height);
	weight->SetValueData(base.actual_weight);
	idealBodyWeight->SetValueData(base.ideal_weight);
	estimationBodySurfaceArea->SetValueData(base.BSI);
	bmi->SetValueData(base.BMI);
	apache2Score->SetValueData(base.APACHE2);
	sofaScore->SetValueData(base.SOFA);
	admissionNumber->SetValueData(base.admission_number);
	hospital->SetValueData(base.hospital);
	administrativeOffice->SetValueData(base.administrative_office);
	inpatientWard->SetValueData(base.inpatient_ward);
	bedNumber->SetValueData(base.bed_number);
	medicalGroup->SetValueData(base.medical_group);
	medicalTeamLeader->SetValueData(base.treat_group_leader);
	attendingDoctor->SetValueData(base.attending_doctor);
	bedTubeDoctor->SetValueData(base.bed_doctor);
	doctorTookPicture->SetValueData(base.acquisition_doctor);
	if (base.patients_source_type=="急诊入院")
	{
		emergencyAdmisionRadio->setAutoExclusive(true);
	}
	else if (base.patients_source_type=="普通病房")
	{
		generalWardRadio->setAutoExclusive(true);
	}
	else 
	{
		operationRadio->setAutoExclusive(true);
	}
	sourcePatientsValue->setText(base.patients_source);
	admissionTimeEdit->setDateTime(QDateTime::fromString(base.admission_date));
	icuAdmissionTimeEdit->setDateTime(QDateTime::fromString(base.icu_date));
	diagnosisValue->setText(base.principal_diagnosis);
}

int EssentialInformation::GetBaseID()
{
	return m_pLogic->GetBaseID(); //saveButton->isEnabled();
}

void EssentialInformation::getData(baseInformation& base)
{
	serialNumber->GetValue(base.number);
	name->GetValue(base.name);
	age->GetValue(base.age);
	base.sex = gender->currentText();
	stature->GetValue(base.height);
	weight->GetValue(base.actual_weight);
	idealBodyWeight->GetValue(base.ideal_weight);
	estimationBodySurfaceArea->GetValue(base.BSI);
	bmi->GetValue(base.BMI);
	apache2Score->GetValue(base.APACHE2);
	sofaScore->GetValue(base.SOFA);
	admissionNumber->GetValue(base.admission_number);
	hospital->GetValue(base.hospital);
	administrativeOffice->GetValue(base.administrative_office);
	inpatientWard->GetValue(base.inpatient_ward);
	bedNumber->GetValue(base.bed_number);
	medicalGroup->GetValue(base.medical_group);
	medicalTeamLeader->GetValue(base.treat_group_leader);
	attendingDoctor->GetValue(base.attending_doctor);
	bedTubeDoctor->GetValue(base.bed_doctor);
	doctorTookPicture->GetValue(base.acquisition_doctor);

	if (emergencyAdmisionRadio->isChecked())
	{
		base.patients_source_type = u8"急诊入院";
	}
	else if (generalWardRadio->isChecked())
	{
		base.patients_source_type = u8"普通病房";
	}
	else
	{
		base.patients_source_type = u8"手术";
	}

	base.patients_source = sourcePatientsValue->text();
	base.admission_date = admissionTimeEdit->text();
	base.icu_date = icuAdmissionTimeEdit->text();
	base.principal_diagnosis = diagnosisValue->toPlainText();
}

void EssentialInformation::OnAutoCompute()
{
	auto index = gender->currentIndex();
	QString strHeight;
	QString strWeight;
	stature->GetValue(strHeight);
	weight->GetValue(strWeight);

	int iHeight = strHeight.toInt();
	int iWeight = strWeight.toInt();
	float ideaWeight = 0.0f;

	if (index) 
	{
		//female
		ideaWeight = (iHeight - 70) * 0.6;
	}
	else
	{
		ideaWeight = (iHeight - 80) * 0.7;
	}

	float fBMI = (float)iWeight / pow(iHeight*0.01 , 2);
	float fBSI = 0.0061 * iHeight + 0.0124 * iWeight - 0.0099;

	idealBodyWeight->GetValueObject()->setText(QString::number(ideaWeight, 'f', 2));
	estimationBodySurfaceArea->GetValueObject()->setText(QString::number(fBSI, 'f', 2));
	bmi->GetValueObject()->setText(QString::number(fBMI, 'f', 2));

}

void EssentialInformation::OnSaveBtnOn()
{
	baseInformation info;
	getData(info);

	if (baseID != 0)
	{
		m_pLogic->UpdataBaseInfo(info);
	}
	else
	{
		m_pLogic->InsertPatientInfoData(info);
	}

	saveButton->setEnabled(false);
	saveButton->setToolTip(u8"已经保存信息");

	serialNumber->ReadOnly(true);
	name->ReadOnly(true);
	age->ReadOnly(true);
	gender->setEnabled(false);
	stature->ReadOnly(true);
	weight->ReadOnly(true);
	idealBodyWeight->ReadOnly(true);
	estimationBodySurfaceArea->ReadOnly(true);
	bmi->ReadOnly(true);
	apache2Score->ReadOnly(true);
	sofaScore->ReadOnly(true);
	admissionNumber->ReadOnly(true);
	hospital->ReadOnly(true);
	administrativeOffice->ReadOnly(true);
	inpatientWard->ReadOnly(true);
	bedNumber->ReadOnly(true);
	medicalGroup->ReadOnly(true);
	medicalTeamLeader->ReadOnly(true);
	attendingDoctor->ReadOnly(true);
	bedTubeDoctor->ReadOnly(true);
	doctorTookPicture->ReadOnly(true);

	emergencyAdmisionRadio->setEnabled(false);
	generalWardRadio->setEnabled(false);
	operationRadio->setEnabled(false);

	sourcePatientsValue->setReadOnly(true);
	admissionTimeEdit->setReadOnly(true);
	icuAdmissionTimeEdit->setReadOnly(true);
	diagnosisValue->setReadOnly(true);
}