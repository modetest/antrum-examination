#pragma once
#include <qdialog.h>
#include <QStyleOption>
#include <QPainter>
#include <QPainter>
#include <QBitmap>

class waitDialog : public QDialog
{
	Q_OBJECT
public:
	waitDialog(QWidget* parent = nullptr);
	~waitDialog();
protected:
	void paintEvent(QPaintEvent* event) override;
};

