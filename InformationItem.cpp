#include "InformationItem.h"

InformationItem::InformationItem(QValidator* v, QWidget *parent)
	: QWidget(parent)
{
	mainLayout = new QHBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	left = new QSpacerItem(120,20);
	title = new QLabel(this);
	title->setStyleSheet("width:89px;height:42px;color:#ffffff;font-size:28px;");
	title->setFixedHeight(80);
	title->setFixedWidth(160);
	title->setWordWrap(true);
	value = new QLineEdit(this);
	value->setStyleSheet("width:300px;height:42px;color:#ffffff;font-size:23px;background-color:#354052;border-style:outset");
	value->setFixedWidth(300);

	if(v!= nullptr)
	value->setValidator(v);
	value->setMaxLength(10);
	
	right = new QSpacerItem(120, 20);

	//mainLayout->addItem(left);
	mainLayout->addWidget(title);
	mainLayout->addWidget(value);
	//mainLayout->addItem(right);
	//mainLayout->addWidget(unit);

}

InformationItem::~InformationItem()
{}
void InformationItem::InitInformationItem(const QString& title,const QString& unit) 
{
	this->title->setText(title);
	if (!unit.isEmpty()) 
	{
		this->unit = new QLabel(this->value);
		this->unit->setStyleSheet("width:22px;height:27px;color:rgba(255,255,255,0.50);font-size:18px;");
		this->unit->setText(unit);
		int width = this->fontMetrics().horizontalAdvance(this->unit->text());

		/*this->valueLayout = new QBoxLayout(QBoxLayout::LeftToRight);
		this->valueLayout->setContentsMargins(0, 0, 13, 0);
		this->valueLayout->addWidget(this->unit);
		this->value->setLayout(this->valueLayout);
		int width = this->fontMetrics().horizontalAdvance(unit);
		this->unit->setFixedWidth(width);
		QMargins margins = this->valueLayout->contentsMargins();*/

		this->value->setTextMargins(19, 3, 20 + width, 3);
		this->unit->move(this->value->width()-20-width, 12);
	}
}
void InformationItem::SetValueWidth(int width)
{
	this->value->setFixedWidth(width);
}
void InformationItem::SetValueHeith(int heith)
{
	this->value->setFixedHeight(heith);
}
void InformationItem::SetValueData(const QString& str)
{
	this->value->setText(str);
}

void InformationItem::GetValue(QString& str)
{
	str = value->text();
}

void InformationItem::ReadOnly(bool b)
{
	value->setReadOnly(b);
}

void InformationItem::SetWidth(int titleWidth,int ValueWidth)
{
	title->setFixedWidth(titleWidth);
	value->setFixedWidth(ValueWidth);
}

QLineEdit* InformationItem::GetValueObject()
{
	return value;
}
