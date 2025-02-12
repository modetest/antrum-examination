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
	InformationItem* serialNumber;//���
	InformationItem* name;//����
	InformationItem* age;//����
	//InformationItem* gender;//�Ա�
	QComboBox* gender;

	InformationItem* stature;//���
	InformationItem* weight;//����
	InformationItem* idealBodyWeight;//��������
	InformationItem* estimationBodySurfaceArea;//��������ֵ��BSI��
	InformationItem* bmi;//BMI
	InformationItem* apache2Score;//APACHE2����
	InformationItem* sofaScore;//SOFA����
	InformationItem* admissionNumber;//סԺ��
	InformationItem* hospital;//ҽԺ
	InformationItem* administrativeOffice;//����
	InformationItem* inpatientWard;//����
	InformationItem* bedNumber;//����
	InformationItem* medicalGroup;//ҽ����
	InformationItem* medicalTeamLeader;//ҽ���鳤
	InformationItem* attendingDoctor;//����ҽ��
	InformationItem* bedTubeDoctor;//�ܴ�ҽ��
	InformationItem* doctorTookPicture;//��ͼҽ��
	//������Դ
	QHBoxLayout* sourcePatientsBox;
	QLabel* sourcePatientsTitle;
	QRadioButton* emergencyAdmisionRadio;//������Ժ
	QRadioButton* generalWardRadio;//��ͨ����
	QRadioButton* operationRadio;//����
	QLineEdit* sourcePatientsValue;//������Դ�����

	QHBoxLayout* admissionTimeLayout;//��Ժʱ��
	QLabel* admissionTimeTitle;
	QDateTimeEdit* admissionTimeEdit;
	QHBoxLayout* icuAdmissionTimeLayout;//��ICUʱ��
	QLabel* icuAdmissionTimeTitle;
	QDateTimeEdit* icuAdmissionTimeEdit;
	//��Ҫ���
	QHBoxLayout* diagnosisLayout;
	QLabel* diagnosisTitle;
	QTextEdit* diagnosisValue;

	QPushButton* saveButton;

	essentialLogic* m_pLogic;

	int baseID;
};
