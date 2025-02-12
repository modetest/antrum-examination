#pragma once

#include <QDialog>
#include <QBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QComboBox>
#include <QRadioButton>
#include <QTextEdit>
#include <QDateTimeEdit>
#include <QBuffer>
#include <QFile>

#include "ui_reportWidget.h"
#include "Data.h"
#include "InformationItem.h"
#include "reportLogic.h"
#include "tinyxml2.h"


class reportWidget : public QDialog
{
	Q_OBJECT

public:
	reportWidget(QString& baseID, QString& clID, QWidget *parent = nullptr);
	~reportWidget();

public slots:
	void  OnSaveReportBtn();

private:
	Ui::reportWidgetClass ui;
private:
	void InitBaseInfo();
	void InitMediantsInfo();
	void InitCollectionInfo(QString& rootPath);

	void insertImageToTemplate(QString& imgPath);
	QString ImagToBase64(QString& strImagePath);
	void print_child_nodes(tinyxml2::XMLNode* root, QString& imgPath);
private:

	QString strBaseID;
	QString strClID;

	baseInformation baseInfo;
	clinicalInformation clinicalInfo;
	QVector<nutritionalInformation> nuInfo;
	collectionData collectInfo;

	QVBoxLayout* mainlay;
	QVBoxLayout* scrollLayout;
	QWidget* scrollWidget;
	QWidget* baseWidget;
	QWidget* clinicalWidget;
	QWidget* collectionWidget;


	InformationItem* rhythm;
	InformationItem* checkName;
	InformationItem* recordName ;
	InformationItem* reviewName ;
};
