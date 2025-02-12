#pragma once

#include <QWidget>
#include <qlabel.h>
#include <QTextEdit>
#include <QPushButton>
#include <QRadioButton>
#include <qboxlayout.h>
#include <QDateTimeEdit>
#include <QComboBox>

#include "InformationItem.h"
#include "essentialLogic.h"

class EssentialInformation : public QWidget
{
	Q_OBJECT

public:
	EssentialInformation(QWidget *parent = nullptr);
	~EssentialInformation();
public:
	void SetData(int baseID);
	int GetBaseID();
	void getData(baseInformation& base);

public slots:
	void OnSaveBtnOn();

	void OnAutoCompute();


	
private:
	QVBoxLayout* mainlay;
	QLabel* title;
	QGridLayout* itemData;
	InformationItem* serialNumber;//编号
	InformationItem* name;//姓名
	InformationItem* age;//年龄
	//InformationItem* gender;//性别
	QComboBox* gender;

	InformationItem* stature;//身高
	InformationItem* weight;//体重
	InformationItem* idealBodyWeight;//理想体重
	InformationItem* estimationBodySurfaceArea;//体表面积估值（BSI）
	InformationItem* bmi;//BMI
	InformationItem* apache2Score;//APACHE2评分
	InformationItem* sofaScore;//SOFA评分
	InformationItem* admissionNumber;//住院号
	InformationItem* hospital;//医院
	InformationItem* administrativeOffice;//科室
	InformationItem* inpatientWard;//病区
	InformationItem* bedNumber;//床号
	InformationItem* medicalGroup;//医疗组
	InformationItem* medicalTeamLeader;//医疗组长
	InformationItem* attendingDoctor;//主治医生
	InformationItem* bedTubeDoctor;//管床医生
	InformationItem* doctorTookPicture;//采图医生
	//病人来源
	QHBoxLayout* sourcePatientsBox;
	QLabel* sourcePatientsTitle;
	QRadioButton* emergencyAdmisionRadio;//急诊入院
	QRadioButton* generalWardRadio;//普通病房
	QRadioButton* operationRadio;//手术
	QLineEdit* sourcePatientsValue;//病人来源输入框

	QHBoxLayout* admissionTimeLayout;//入院时间
	QLabel* admissionTimeTitle;
	QDateTimeEdit* admissionTimeEdit;
	QHBoxLayout* icuAdmissionTimeLayout;//入ICU时间
	QLabel* icuAdmissionTimeTitle;
	QDateTimeEdit* icuAdmissionTimeEdit;
	//主要诊断
	QHBoxLayout* diagnosisLayout;
	QLabel* diagnosisTitle;
	QTextEdit* diagnosisValue;

	QPushButton* saveButton;

	essentialLogic* m_pLogic;

	int baseID;
};
