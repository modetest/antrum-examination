#include "selectDisplay.h"

selectDisplay::selectDisplay(const short t, const QString title, QString& text, QWidget* parent):type(t), key(title), QWidget(parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setStyleSheet("QWidget{background-color:#354052;}");

	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	mainLayout->setSpacing(0);
	
	QLabel* textLabel = new  QLabel(this);
	textLabel->setText(text);
	textLabel->setStyleSheet("QLabel{color:white;font:24px}");
	textLabel->setFixedHeight(24);
	textLabel->setWordWrap(true);

	QPushButton* delBtn = new QPushButton(this);
	delBtn->setStyleSheet("QPushButton{background-image:url(:/antrum/resouce/delDisplay-normal.png);border:none;}QPushButton:hover{ background-image:url(:/antrum/resouce/delDisplay-clicked.png);border:none;}QPushButton:pressed{background-image:url(:/antrum/resouce/delDisplay-clicked.png); border:none;}");
	delBtn->setFixedSize(QSize(24, 24));
	mainLayout->addWidget(textLabel);
	mainLayout->addWidget(delBtn);

	QFont font("Arial", 21);
	QFontMetrics fm(font);
	auto width = fm.horizontalAdvance(text); // 获取字符串s的总像素宽度
	width = width + 58;
	this->setFixedSize(QSize(width, 48));

	connect(delBtn, SIGNAL(clicked()), this, SLOT(OnDelButton()));
}

selectDisplay::~selectDisplay()
{

}



void selectDisplay::OnDelButton()
{
	emit removeItme(type, key);
	this->close();
}