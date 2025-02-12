#pragma once
#include <qwidget.h>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "UltrasoundArtficialIntellgenceLogic.h"



class tableItemWidget :public QWidget
{
	Q_OBJECT
public:
	tableItemWidget(int index,QWidget* parent = nullptr);
	~tableItemWidget();


	int getIndex();
public:
	QPushButton* btnDel;
	QPushButton* btnInfo;

private:
	int m_index;
};

