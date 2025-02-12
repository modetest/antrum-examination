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


struct baseInformation //基本信息
{
	QString number;//编号
	QString name;  //姓名
	QString age;  //年纪
	QString sex; //性别
	QString height; //身高
	QString actual_weight; //体重 
	QString ideal_weight; //理想体重
	QString BSI;
	QString BMI;
	QString APACHE2;
	QString SOFA;
	QString admission_number;//住院号
	QString hospital; //医院
	QString administrative_office;//科室
	QString inpatient_ward;//病区
	QString bed_number;//床号
	QString medical_group;//医疗组
	QString treat_group_leader;//  医疗组长
	QString  attending_doctor;//主治医生
	QString  bed_doctor;//管床医生
	QString acquisition_doctor; // 采图医生
	QString patients_source_type;////病人来源单选
	QString patients_source;//病人来源
	QString admission_date;//入院时间
	QString icu_date; //ICU时间
	QString principal_diagnosis;// 主要诊断
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
	QString name;//药品
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
struct clinicalInformation //临床
{
	QString pid;
	QString hreat_rate; // 心率
	QString heart_Rhythm;//心律
	QString SBP; //收缩压
	QString DBP; //舒张压
	QString MAP; //平均动脉压
	QString SPO2;
	QString PI;
	QString RR;
	QString USCode;//超声编号
	QString AcTime; // 开始时间

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
	QString	startTime;//开始时间
	QString	endTime;//结束时间
	QString count;//收缩舒张次数
	QString diastoleArea;//舒张面积
	QString contractArea;//收缩面积
};
struct collectionSumData 
{
	QString GatheringTime;//采集时间
	QString AcquisitionTime;//采集时长
	QString MeanDiastolicArea;//平均舒张面积
	QString MeanShrinkageArea;//平均收缩面积
	QString NumberContractions;//收缩总数

	QVector<collectionData>  CollectionDatas;//采集列表
};





