#include "computeDistanceDialog.h"

computeDistanceDialog::computeDistanceDialog(QImage& image,QWidget* parent) : QDialog(parent)
{
	qRegisterMetaType<QList<QString> >("QList<QPoint>");
	this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
	this->setStyleSheet("QDialog{background-color:#283141}");

	QVBoxLayout* mainLay = new QVBoxLayout(this);

	QLabel* textLabel = new QLabel(this);
	textLabel->setText(u8"自动匹配失败,请手动定位标尺!");
	textLabel->setStyleSheet("QLabel{color:white;font:bold 28px}");
	textLabel->setAlignment(Qt::AlignCenter);

	pixmap = QPixmap::fromImage(image);

	imageLabel = new drawLabel(this);
	imageLabel->installEventFilter(this);
	imageLabel->setFixedSize(image.size().width(), 910);
	imageLabel->setPixmap(pixmap);

	qDebug() << "image size: " << image.size();

	QHBoxLayout* btnLay = new QHBoxLayout();

	QPushButton* okbtn = new QPushButton(this);
	QPushButton* reSetbtn = new QPushButton(this);

	okbtn->setText(u8"确定");
	reSetbtn->setText(u8"重置");

	okbtn->setFixedSize(QSize(110, 40));
	reSetbtn->setFixedSize(QSize(110, 40));

	okbtn->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/button-back.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333; }");
	reSetbtn->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/button-back.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333; }");

	btnLay->addWidget(okbtn);
	btnLay->addWidget(reSetbtn);

	connect(okbtn, &QPushButton::clicked, this,&computeDistanceDialog::OnOKButton);
	connect(reSetbtn, &QPushButton::clicked, this, &computeDistanceDialog::OnResetButton);
	
	mainLay->addWidget(textLabel);
	mainLay->addWidget(imageLabel);
	mainLay->addLayout(btnLay);
}

computeDistanceDialog::~computeDistanceDialog()
{

}
void computeDistanceDialog::paintEvent(QPaintEvent* event)
{

}
void computeDistanceDialog::OnOKButton()
{
	if (posList.size() == 2)
	{
		emit computeDistrance(posList);

		this->close();
	}
}

void computeDistanceDialog::OnResetButton()
{
	posList.clear();
	imageLabel->Draw(posList);
}

bool computeDistanceDialog::eventFilter(QObject* target, QEvent* event)
{
	if (target == imageLabel)
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
			if (mouseEvent->button() == Qt::LeftButton) { // 判断左键按下
				//QPoint globalPos = mouseEvent->globalPos();
				QPoint localPos = mouseEvent->pos();
				
				if (posList.size() < 2)
				{
					posList.append(localPos);
					imageLabel->Draw(posList);
				    qDebug() << "Local Mouse Pos: " << localPos;
				}

			}
		}
	}

	return false;
}

drawLabel::drawLabel(QWidget* parent) : QLabel(parent)
{
	firstPoint.setX(0);
	firstPoint.setY(0);
	secondPoint.setX(0);
	secondPoint.setY(0);
}

void drawLabel::paintEvent(QPaintEvent* event)
{
	QLabel::paintEvent(event);
	QPainter painter(this);
	QPen pen;
	pen.setColor(QColor(255,0,0));
	pen.setWidth(5);
	painter.setPen(pen);
	if (firstPoint.x() != 0 && firstPoint.y() != 0)
	{
		painter.drawPoint(firstPoint);
	}
	if (secondPoint.x() != 0 && secondPoint.y() != 0)
	{
		painter.drawPoint(secondPoint);
	}
	
}

void drawLabel::Draw(QList<QPoint>& points)
{
	if (points.size() == 0)
	{
		this->firstPoint = QPoint(0, 0);
		this->secondPoint = QPoint(0, 0);
	}

	if (points.size() > 0)
	{
		this->firstPoint = points.first();
	}

	if (points.size() > 1)
	{
		this->secondPoint = points.last();
	}

	update();
}
