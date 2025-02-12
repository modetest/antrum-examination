#pragma once

#include <qstring.h>
#include <qvector.h>
#include <qdebug.h>

enum BreatheOrMedicType
{
	BREATHE,
	VASOACTIVITY,
	ANALGESIA,
	CALM,
	MUSCLERELAXATION
};


struct baseInformation //������Ϣ
{
	QString number;//���
	QString name;  //����
	QString age;  //���
	QString sex; //�Ա�
	QString height; //���
	QString actual_weight; //���� 
	QString ideal_weight; //��������
	QString BSI;
	QString BMI;
	QString APACHE2;
	QString SOFA;
	QString admission_number;//סԺ��
	QString hospital; //ҽԺ
	QString administrative_office;//����
	QString inpatient_ward;//����
	QString bed_number;//����
	QString medical_group;//ҽ����
	QString treat_group_leader;//  ҽ���鳤
	QString  attending_doctor;//����ҽ��
	QString  bed_doctor;//�ܴ�ҽ��
	QString acquisition_doctor; // ��ͼҽ��
	QString patients_source_type;////������Դ��ѡ
	QString patients_source;//������Դ
	QString admission_date;//��Ժʱ��
	QString icu_date; //ICUʱ��
	QString principal_diagnosis;// ��Ҫ���
};

struct breathe
{
	QString  pattern;
	QString  LMin;
	QString  FiO2;
	QString  IPAP;
	QString  EPAP;
	QString  PEEP;
	QString  hreathModel;
};

struct medicine
{
	int pid;
	QString name;//ҩƷ
	QString  TotalVolume;
	QString  MediaVolume;
	QString  PumpingSpeed;
	QString  PumpingVolume;
	QString  Score;
};

struct medicData
{
	int type;
	medicine me;
};
struct clinicalInformation //�ٴ�
{
	QString pid;
	QString hreat_rate; // ����
	QString heart_Rhythm;//����
	QString SBP; //����ѹ
	QString DBP; //����ѹ
	QString MAP; //ƽ������ѹ
	QString SPO2;
	QString PI;
	QString RR;
	QString USCode;//�������
	QString AcTime; // ��ʼʱ��

	QVector<breathe>  vecBreathe;

	QVector<medicine>  vecActivity;
	QVector<medicine>  vecAnalgesia;
	QVector<medicine>  vecCalm;
	QVector<medicine>  vecMuscleRelaxation;
};

struct nutritionalInformation
{
	QString name;
	QString	Formula;
	QString Volume;
	QString Kcal;
	QString Protein;
	QString StartTime;
};
struct collectionData
{
	QString id;
	QString pid;
	QString	acTime;
	QString cycleTime;
	QString	startTime;//��ʼʱ��
	QString	endTime;//����ʱ��
	QString count;//�������Ŵ���
	QString diastoleArea;//�������
	QString contractArea;//�������
};
struct collectionSumData 
{
	QString GatheringTime;//�ɼ�ʱ��
	QString AcquisitionTime;//�ɼ�ʱ��
	QString MeanDiastolicArea;//ƽ���������
	QString MeanShrinkageArea;//ƽ���������
	QString NumberContractions;//��������

	QVector<collectionData>  CollectionDatas;//�ɼ��б�
};





