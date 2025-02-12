#include "reportLogic.h"

reportLogic::reportLogic(QObject *parent)
	: QObject(parent)
{}

reportLogic::~reportLogic()
{}

void reportLogic::queryData(int baseID, int clID, baseInformation & baseInfo, clinicalInformation & clinInfo, QVector<nutritionalInformation>&nuInfo, collectionData & collectInfo)
{
    //基本信息
    QSqlQuery query;
    query.prepare("SELECT "
        "serial_number, name, age, sex, height, actual_weight, ideal_weight, BSI, BMI, APACHE2, SOFA, "
        "admission_number, hospital, administrative_office, inpatient_ward, bed_number, "
        "treat_group_leader, attending_doctor, bed_doctor, acquisition_doctor,patients_source_type, patients_source, "
        "admission_date, icu_date, principal_diagnosis "
        "FROM basic_information WHERE id = :id;"
    );

    query.bindValue(":id", baseID);

    if (query.exec()) {
        while (query.next())
        {
            baseInfo.number = query.value(0).toString();
            baseInfo.name = query.value(1).toString();
            baseInfo.age = query.value(2).toString();
            baseInfo.sex = query.value(3).toString();
            baseInfo.height = query.value(4).toString();
            baseInfo.actual_weight = query.value(5).toString();
            baseInfo.ideal_weight = query.value(6).toString();
            baseInfo.BSI = query.value(7).toString();
            baseInfo.BMI = query.value(8).toString();
            baseInfo.APACHE2 = query.value(9).toString();
            baseInfo.SOFA = query.value(10).toString();
            baseInfo.admission_number = query.value(11).toString();
            baseInfo.hospital = query.value(12).toString();
            baseInfo.administrative_office = query.value(13).toString();
            baseInfo.inpatient_ward = query.value(14).toString();
            baseInfo.bed_number = query.value(15).toString();
            baseInfo.treat_group_leader = query.value(16).toString();
            baseInfo.attending_doctor = query.value(17).toString();
            baseInfo.bed_doctor = query.value(18).toString();
            baseInfo.acquisition_doctor = query.value(19).toString();
            baseInfo.patients_source_type = query.value(20).toString();
            baseInfo.patients_source = query.value(21).toString();
            baseInfo.admission_date = query.value(22).toString();
            baseInfo.icu_date = query.value(23).toString();
            baseInfo.principal_diagnosis = query.value(24).toString();
        }
    }
    else
    {
        qDebug() << "sql query error: " << query.lastError();
    }

    //临床信息
    query.prepare("SELECT heart_rate,heart_Rhythm,SBP,DBP,MAP,SPO2,PI,RR,USCode,AcTime,id FROM clinical_information WHERE id = :id ORDER BY id DESC LIMIT 1;");

    query.bindValue(":id", clID);

    if (query.exec())
    {
        while (query.next())
        {
            clinInfo.hreat_rate = query.value(0).toString();
            clinInfo.heart_Rhythm = query.value(1).toString();
            clinInfo.SBP = query.value(2).toString();
            clinInfo.DBP = query.value(3).toString();
            clinInfo.MAP = query.value(4).toString();
            clinInfo.SPO2 = query.value(5).toString();
            clinInfo.PI = query.value(6).toString();
            clinInfo.RR = query.value(7).toString();
            clinInfo.USCode = query.value(8).toString();
            clinInfo.AcTime = query.value(9).toString();
        }
    }

    //呼吸
    query.prepare("SELECT pattern,LMin,FiO2,IPAP,EPAP,PEEP,hreathModel FROM breathe_information WHERE pid = :id;");

    query.bindValue(":id", clID);

    if (query.exec())
    {

        while (query.next())
        {
            breathe bInfo;
            bInfo.pattern = query.value(0).toString();
            bInfo.LMin = query.value(1).toString();
            bInfo.FiO2 = query.value(2).toString();
            bInfo.IPAP = query.value(3).toString();
            bInfo.EPAP = query.value(4).toString();
            bInfo.PEEP = query.value(5).toString();
            bInfo.hreathModel = query.value(6).toString();
            clinInfo.vecBreathe.append(bInfo);
        }
    }

    //药物
    query.prepare("SELECT name,TotalVolume,MediaVolume,PumpingSpeed,PumpingVolume,Score,type FROM clinical_information_medicine WHERE pid = :id;");

    query.bindValue(":id", clID);

    if (query.exec())
    {

        while (query.next())
        {
            medicine cInfo;
            short type = 0;
            cInfo.name = query.value(0).toString();
            cInfo.TotalVolume = query.value(1).toString();
            cInfo.MediaVolume = query.value(2).toString();
            cInfo.PumpingSpeed = query.value(3).toString();
            cInfo.PumpingVolume = query.value(4).toString();
            cInfo.Score = query.value(5).toString();
            type = query.value(6).toInt();

            clinInfo.vecActivity.append(cInfo);

            /*switch (type)
            {
            case VASOACTIVITY:
                clinInfo.vecActivity.append(cInfo);
                break;
            case ANALGESIA:
                clinInfo.vecAnalgesia.append(cInfo);
                break;
            case CALM:
                clinInfo.vecCalm.append(cInfo);
                break;
            case MUSCLERELAXATION:
                clinInfo.vecMuscleRelaxation.append(cInfo);
                break;
            default:
                break;
            }*/
        }
    }

    //营养
    query.prepare("SELECT name,Formula,Volume,Kcal,Protein,StartTime FROM nutrition_information WHERE pid = :id;");

    query.bindValue(":id", clID);

    if (query.exec())
    {
        while (query.next())
        {
            nutritionalInformation info;
            info.name = query.value(0).toString();
            info.Formula = query.value(1).toString();
            info.Volume = query.value(2).toString();
            info.Kcal = query.value(3).toString();
            info.Protein = query.value(4).toString();
            info.StartTime = query.value(5).toString();
            nuInfo.append(info);
        }
    }

    QString s = QString("SELECT pid,AcTime,cycleTime,count,diastoleArea,contractArea FROM collection WHERE pid IN (%1)").arg(clID);
    if (query.exec(s))
    {
        while (query.next())
        {
            collectInfo.pid = query.value(0).toString();
            collectInfo.acTime = query.value(1).toString();
            collectInfo.cycleTime = query.value(2).toString();
            collectInfo.count = query.value(3).toString();
            collectInfo.diastoleArea = query.value(4).toString();
            collectInfo.contractArea = query.value(5).toString();
        }
    }
    query.finish();
}
