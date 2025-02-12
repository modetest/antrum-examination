#pragma once
#include <qdialog.h>
#include <QBoxLayout>
#include <Qlabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QPainter>
#include <QMetaType>

#include <qdebug.h>


class drawLabel : public QLabel
{
public:
	drawLabel(QWidget* parent = nullptr);
	virtual void paintEvent(QPaintEvent* event) override;

	void Draw(QList<QPoint>& points);
private:
	QPoint firstPoint;
	QPoint secondPoint;
};


class computeDistanceDialog :public QDialog
{
	Q_OBJECT
public:
	computeDistanceDialog(QImage& image,QWidget* parent = nullptr);
	~computeDistanceDialog();

	virtual void paintEvent(QPaintEvent* event) override;

public slots:
	void OnOKButton();
	void OnResetButton();
signals:
	void computeDistrance(QList<QPoint> points);

protected:
	bool eventFilter(QObject* target, QEvent* event) override;

private:
	drawLabel* imageLabel;
	QList<QPoint> posList;
	QPixmap pixmap;
};

