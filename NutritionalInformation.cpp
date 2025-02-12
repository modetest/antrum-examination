#include "NutritionalInformation.h"

NutritionalInformation::NutritionalInformation(QWidget *parent)
	: QWidget(parent)
{
	QPalette pal(this->palette());
	QColor color("#283141");
	pal.setColor(QPalette::Window, color);
	this->setAutoFillBackground(true);
	this->setPalette(pal);

	m_pLogic = new nutritionalLogic();

	mainlay = new QVBoxLayout(this);
	mainlay->setMargin(0);

	title = new QLabel();
	title->setText(u8" 营养信息");
	//title->setFixedSize(1863, 48);
	title->setFixedHeight(48);
	title->setStyleSheet("QLabel{background-color: #323E55;color:white;font-size:30px;}");
	mainlay->addWidget(title);


	QHBoxLayout* nutritionalTitleLaoyout = new QHBoxLayout();
	nutritionalTitleLaoyout->setContentsMargins(10, 0, 0, 0);
	nutritionalTitleLaoyout->setSpacing(60);
	QLabel* nutritionalQLabel = new QLabel(u8"呼吸支持方式：");
	nutritionalQLabel->setStyleSheet("width:89px;height:42px;color:#ffffff;font-size:28px;");
	
	ENCheckBox = new QCheckBox(u8"EN");
	PNCheckBox = new QCheckBox(u8"PN");
	IMCheckBox = new QCheckBox(u8"经口进食");

	ENCheckBox->setStyleSheet("QCheckBox{color:#ffffff;font-size:28px;}QCheckBox::indicator{width:18px; height:18px;} QCheckBox::indicator:unchecked {image:url(:/antrum/resouce/radiobtn_unchecked.png);} QCheckBox::indicator:checked{image: url(:/antrum/resouce/qradiobtn_checked.png);}");
	PNCheckBox->setStyleSheet("QCheckBox{color:#ffffff;font-size:28px;}QCheckBox::indicator{width:18px; height:18px;} QCheckBox::indicator:unchecked {image:url(:/antrum/resouce/radiobtn_unchecked.png);} QCheckBox::indicator:checked{image: url(:/antrum/resouce/qradiobtn_checked.png);}");
	IMCheckBox->setStyleSheet("QCheckBox{color:#ffffff;font-size:28px;}QCheckBox::indicator{width:18px; height:18px;} QCheckBox::indicator:unchecked {image:url(:/antrum/resouce/radiobtn_unchecked.png);} QCheckBox::indicator:checked{image: url(:/antrum/resouce/qradiobtn_checked.png);}");

	connect(ENCheckBox, &QCheckBox::toggled, this, &NutritionalInformation::OnENCheckBox);
	connect(PNCheckBox, &QCheckBox::clicked, this, &NutritionalInformation::OnPNCheckBox);
	connect(IMCheckBox, &QCheckBox::clicked, this, &NutritionalInformation::OnIMCheckBox);

	nutritionalTitleLaoyout->addWidget(nutritionalQLabel);
	nutritionalTitleLaoyout->addWidget(ENCheckBox);
	nutritionalTitleLaoyout->addWidget(PNCheckBox);
	nutritionalTitleLaoyout->addWidget(IMCheckBox);
	nutritionalTitleLaoyout->addStretch();
	mainlay->addLayout(nutritionalTitleLaoyout);



	QIntValidator* intValidator = new QIntValidator(1, 200, this);
	QDoubleValidator* doubleValidator = new QDoubleValidator(1, 99999, 2, this);

	//EN
	ENWidget = new QWidget(this);
	QVBoxLayout* ENLayout = new QVBoxLayout(ENWidget);

	QGridLayout* ENGridLayout = new QGridLayout();
	QLabel* ENLabel = new QLabel(u8"EN:");
	ENLabel->setStyleSheet("width:89px;height:42px;color:#ffffff;font-size:28px;");
	ENFormula = new InformationItem(nullptr, ENWidget);
	ENFormula->SetWidth(180, 300);
	ENFormula->InitInformationItem(u8"EN营养配方：", u8"");
	ENVolume = new InformationItem(nullptr, ENWidget);
	ENVolume->SetWidth(150, 200);
	ENVolume->InitInformationItem(u8"容量：", u8"ml");
	ENCalories = new InformationItem(nullptr, ENWidget);
	ENCalories->SetWidth(150, 200);
	ENCalories->InitInformationItem(u8"总热卡：", u8"kcal");
	ENProtein = new InformationItem(nullptr, ENWidget);
	ENProtein->SetWidth(200, 200);
	ENProtein->InitInformationItem(u8"蛋白质含量：", u8"g");

	QPushButton* ENOKBrn = new QPushButton(ENWidget);
	ENOKBrn->setText(u8"确定");
	ENOKBrn->setFixedSize(62, 26);
	ENOKBrn->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/time_ok_2.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333;}");

	connect(ENOKBrn, &QPushButton::clicked, this, &NutritionalInformation::OnENBtn);
	
	QHBoxLayout* HENlayout = new  QHBoxLayout();
	QLabel* ENstartTimeLabel = new QLabel(u8"开始时间：");
	ENstartTimeLabel->setStyleSheet("width:89px;height:42px;color:#ffffff;font-size:28px;");
	ENStartTime = new QDateTimeEdit();
	ENStartTime->setDateTime(QDateTime::currentDateTime());
	ENStartTime->setFixedSize(300, 42);
	ENStartTime->setStyleSheet("color:#ffffff;font-size:23px;background-color:#354052;border-style:outset;alternate-background-color:rgb(204, 229, 255);");
	HENlayout->addWidget(ENstartTimeLabel);
	HENlayout->addWidget(ENStartTime);

	ENGridLayout->addWidget(ENFormula, 0, 0, Qt::AlignRight);
	ENGridLayout->addWidget(ENVolume, 0, 1, Qt::AlignRight);
	ENGridLayout->addWidget(ENCalories, 0, 2, Qt::AlignRight);
	ENGridLayout->addWidget(ENProtein, 0, 3, Qt::AlignRight);
	ENGridLayout->addWidget(ENOKBrn, 0, 4, Qt::AlignRight);
	ENGridLayout->addLayout(HENlayout, 1, 0, Qt::AlignRight);

	ENWidget->hide();
	ENLayout->addLayout(ENGridLayout);
	mainlay->addWidget(ENWidget);

	//PN
	PNWidget = new QWidget(this);
	QVBoxLayout* PNLayout = new QVBoxLayout(PNWidget);

	QGridLayout* PNGridLayout = new QGridLayout();
	QLabel* PNLabel = new QLabel(u8"PN:");
	PNLabel->setStyleSheet("width:89px;height:42px;color:#ffffff;font-size:28px;");
	PNFormula = new InformationItem(nullptr, PNWidget);
	PNFormula->SetWidth(180, 300);
	PNFormula->InitInformationItem(u8"PN营养配方：", u8"");
	PNVolume = new InformationItem(nullptr, PNWidget);
	PNVolume->SetWidth(150, 200);
	PNVolume->InitInformationItem(u8"容量：", u8"ml");
	PNCalories = new InformationItem(nullptr, PNWidget);
	PNCalories->SetWidth(150, 200);
	PNCalories->InitInformationItem(u8"总热卡：", u8"kcal");
	PNProtein = new InformationItem(nullptr, PNWidget);
	PNProtein->SetWidth(200, 200);
	PNProtein->InitInformationItem(u8"蛋白质含量：", u8"g");

	QPushButton* PNOKBrn = new QPushButton(PNWidget);
	PNOKBrn->setText(u8"确定");
	PNOKBrn->setFixedSize(62, 26);
	PNOKBrn->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/time_ok_2.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333;}");

	connect(PNOKBrn, &QPushButton::clicked, this, &NutritionalInformation::OnPNBtn);

	QHBoxLayout* HPNlayout = new  QHBoxLayout();
	QLabel* PNstartTimeLabel = new QLabel(u8"开始时间：");
	PNstartTimeLabel->setStyleSheet("width:89px;height:42px;color:#ffffff;font-size:28px;");
	PNStartTime = new QDateTimeEdit();
	PNStartTime->setDateTime(QDateTime::currentDateTime());
	PNStartTime->setFixedSize(300, 42);
	PNStartTime->setStyleSheet("color:#ffffff;font-size:23px;background-color:#354052;border-style:outset;alternate-background-color:rgb(204, 229, 255);");
	HPNlayout->addWidget(PNstartTimeLabel);
	HPNlayout->addWidget(PNStartTime);

	PNGridLayout->addWidget(PNFormula, 0, 0, Qt::AlignRight);
	PNGridLayout->addWidget(PNVolume, 0, 1, Qt::AlignRight);
	PNGridLayout->addWidget(PNCalories, 0, 2, Qt::AlignRight);
	PNGridLayout->addWidget(PNProtein, 0, 3, Qt::AlignRight);
	PNGridLayout->addWidget(PNOKBrn, 0, 4, Qt::AlignRight);
	PNGridLayout->addLayout(HPNlayout, 1, 0, Qt::AlignRight);

	PNWidget->hide();
	PNLayout->addLayout(PNGridLayout);
	mainlay->addWidget(PNWidget);

	//经口进食
	IMWidget = new QWidget(this);
	QVBoxLayout* IMLayout = new QVBoxLayout(IMWidget);

	QGridLayout* IMGridLayout = new QGridLayout();
	QLabel* IMLabel = new QLabel(u8"IM:");
	IMLabel->setStyleSheet("width:89px;height:42px;color:#ffffff;font-size:28px;");
	IMFormula = new InformationItem(nullptr, IMWidget);
	IMFormula->SetWidth(180, 300);
	IMFormula->InitInformationItem(u8"营养配方：", u8"");
	IMVolume = new InformationItem(nullptr, IMWidget);
	IMVolume->SetWidth(150, 200);
	IMVolume->InitInformationItem(u8"容量：", u8"ml");
	IMCalories = new InformationItem(nullptr, IMWidget);
	IMCalories->SetWidth(150, 200);
	IMCalories->InitInformationItem(u8"总热卡：", u8"kcal");
	IMProtein = new InformationItem(nullptr, IMWidget);
	IMProtein->SetWidth(200, 200);
	IMProtein->InitInformationItem(u8"蛋白质含量：", u8"g");

	QPushButton* IMOKBrn = new QPushButton(IMWidget);
	IMOKBrn->setText(u8"确定");
	IMOKBrn->setFixedSize(62, 26);
	IMOKBrn->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/time_ok_2.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333;}");

	connect(IMOKBrn, &QPushButton::clicked, this, &NutritionalInformation::OnIMBtn);

	QHBoxLayout* HIMlayout = new  QHBoxLayout();
	QLabel* IMstartTimeLabel = new QLabel(u8"开始时间：");
	IMstartTimeLabel->setStyleSheet("width:89px;height:42px;color:#ffffff;font-size:28px;");
	IMStartTime = new QDateTimeEdit();
	IMStartTime->setDateTime(QDateTime::currentDateTime());
	IMStartTime->setFixedSize(300, 42);
	IMStartTime->setStyleSheet("color:#ffffff;font-size:23px;background-color:#354052;border-style:outset;alternate-background-color:rgb(204, 229, 255);");
	HIMlayout->addWidget(IMstartTimeLabel);
	HIMlayout->addWidget(IMStartTime);

	IMGridLayout->addWidget(IMFormula, 0, 0, Qt::AlignRight);
	IMGridLayout->addWidget(IMVolume, 0, 1, Qt::AlignRight);
	IMGridLayout->addWidget(IMCalories, 0, 2, Qt::AlignRight);
	IMGridLayout->addWidget(IMProtein, 0, 3, Qt::AlignRight);
	IMGridLayout->addWidget(IMOKBrn, 0, 4, Qt::AlignRight);
	IMGridLayout->addLayout(HIMlayout, 1, 0, Qt::AlignRight);

	IMWidget->hide();
	IMLayout->addLayout(IMGridLayout);
	mainlay->addWidget(IMWidget);
}

NutritionalInformation::~NutritionalInformation()
{

}

void NutritionalInformation::GetData(QVector<nutritionalInformation>& vec)
{
	m_pLogic->GetENPageData(vec);
	m_pLogic->GetPNPageData(vec);
	m_pLogic->GetOFPageData(vec);
}

void NutritionalInformation::OnENCheckBox(bool b)
{
	if (b)
	{
		ENWidget->show();
	}
	else
	{
		ENWidget->hide();
	}
}
void NutritionalInformation::OnPNCheckBox(bool b)
{
	if (b)
	{
		PNWidget->show();
	}
	else
	{
		PNWidget->hide();
	}
}

void NutritionalInformation::OnIMCheckBox(bool b)
{
	if (b)
	{
		IMWidget->show();
	}
	else
	{
		IMWidget->hide();
	}
}

void NutritionalInformation::OnENBtn()
{
	nutritionalInformation data;
	data.name = "EN";
	ENFormula->GetValue(data.Formula);
	ENVolume->GetValue(data.Volume);
	ENCalories->GetValue(data.Kcal);
	ENProtein->GetValue(data.Protein);
	data.StartTime =  ENStartTime->text();
	
	if (m_pLogic->isHave(data.name, EN))
		return;
	m_pLogic->CheckZero(data);

	QString displayStr = data.Formula + ", "+ data.Volume+"ml, "+ data.Kcal+ "kcal, "+ data.Protein +u8"g, 开始时间："+data.StartTime;

	selectDisplay* dis = new selectDisplay(EN, data.name, displayStr, ENWidget);
	ENWidget->layout()->addWidget(dis);
	connect(dis, &selectDisplay::removeItme, this, &NutritionalInformation::OnDeleteItem);
	m_pLogic->insert(data.name, data, EN);
}

void NutritionalInformation::OnPNBtn()
{
	nutritionalInformation data;
	data.name = "PN";
	PNFormula->GetValue(data.Formula);
	PNVolume->GetValue(data.Volume);
	PNCalories->GetValue(data.Kcal);
	PNProtein->GetValue(data.Protein);
	data.StartTime = PNStartTime->text();

	if (m_pLogic->isHave(data.name, PN))
		return;
	m_pLogic->CheckZero(data);

	QString displayStr = data.Formula + ", " + data.Volume + "ml, " + data.Kcal + "kcal, " + data.Protein + u8"g, 开始时间：" + data.StartTime;

	selectDisplay* dis = new selectDisplay(PN, data.name, displayStr, PNWidget);
	PNWidget->layout()->addWidget(dis);
	connect(dis, &selectDisplay::removeItme, this, &NutritionalInformation::OnDeleteItem);
	m_pLogic->insert(data.name, data, PN);
}

void NutritionalInformation::OnIMBtn()
{
	nutritionalInformation data;
	data.name = u8"经口进食";
	IMFormula->GetValue(data.Formula);
	IMVolume->GetValue(data.Volume);
	IMCalories->GetValue(data.Kcal);
	IMProtein->GetValue(data.Protein);
	data.StartTime = IMStartTime->text();

	if (m_pLogic->isHave(data.name, IM))
		return;
	m_pLogic->CheckZero(data);

	QString displayStr = data.Formula + ", " + data.Volume + "ml, " + data.Kcal + "kcal, " + data.Protein + u8"g, 开始时间：" + data.StartTime;

	selectDisplay* dis = new selectDisplay(IM, data.name, displayStr, IMWidget);
	IMWidget->layout()->addWidget(dis);
	connect(dis, &selectDisplay::removeItme, this, &NutritionalInformation::OnDeleteItem);
	m_pLogic->insert(data.name, data, IM);
}

void NutritionalInformation::OnDeleteItem(short t, QString& key)
{
	m_pLogic->remove(key, (nutritionalEnum)t);
}
