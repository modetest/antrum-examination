#pragma once

#include <QWidget>
#include <qlabel.h>
#include <QLineEdit>
#include <qboxlayout.h>
#include <QSpacerItem>
#include <QValidator>


class InformationItem : public QWidget
{
	Q_OBJECT

public:
	InformationItem(QValidator* v,QWidget *parent = nullptr);
	~InformationItem();
public:
	void InitInformationItem(const QString& title,const QString& unit);
	void SetValueWidth(int width);
	void SetValueHeith(int heith);
	void SetValueData(const QString& str);
	void GetValue(QString& str);
	void ReadOnly(bool b);
	void SetWidth(int titleWidth, int ValueWidth);
	QLineEdit* GetValueObject();

private:
	QHBoxLayout* mainLayout;
	QSpacerItem* left;
	QLabel* title;
	QLineEdit* value;
	//QBoxLayout* valueLayout;
	QSpacerItem* right;
	QLabel* unit;
};
