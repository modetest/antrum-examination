#include "essentialLogic.h"

essentialLogic::essentialLogic(QObject* parent):QObject(parent)
{
    m_baseID = 0;
}

essentialLogic::~essentialLogic()
{

}

void essentialLogic::InsertPatientInfoData(baseInformation& baseInfo)
{
    QSqlQuery query;

    //基本信息
    query.prepare(
        "INSERT INTO basic_information ("
        "serial_number, name, age, sex, height, actual_weight, ideal_weight, BSI, BMI, APACHE2, SOFA, "
        "admission_number, hospital, administrative_office, inpatient_ward, bed_number, "
        "treat_group_leader, attending_doctor, bed_doctor, acquisition_doctor, patients_source_type,patients_source, "
        "admission_date, icu_date, principal_diagnosis"
        ") VALUES ("
        ":number, :name, :age, :sex, :height, :actual_weight, :ideal_weight, :BSI, :BMI, :APACHE2, :SOFA, "
        ":admission_number, :hospital, :administrative_office, :inpatient_ward, :bed_number, "
        ":treat_group_leader, :attending_doctor, :bed_doctor, :acquisition_doctor,:patients_source_type,:patients_source, "
        ":admission_date, :icu_date, :principal_diagnosis"
        ")"
    );

    query.bindValue(":number", baseInfo.number);
    query.bindValue(":name", baseInfo.name);
    query.bindValue(":age", baseInfo.age);
    query.bindValue(":sex", baseInfo.sex);
    query.bindValue(":height", baseInfo.height);
    query.bindValue(":actual_weight", baseInfo.actual_weight);
    query.bindValue(":ideal_weight", baseInfo.ideal_weight);
    query.bindValue(":BSI", baseInfo.BSI);
    query.bindValue(":BMI", baseInfo.BMI);
    query.bindValue(":APACHE2", baseInfo.APACHE2);
    query.bindValue(":SOFA", baseInfo.SOFA);
    query.bindValue(":admission_number", baseInfo.admission_number);
    query.bindValue(":hospital", baseInfo.hospital);
    query.bindValue(":administrative_office", baseInfo.administrative_office);
    query.bindValue(":inpatient_ward", baseInfo.inpatient_ward);
    query.bindValue(":bed_number", baseInfo.bed_number);
    query.bindValue(":treat_group_leader", baseInfo.treat_group_leader);
    query.bindValue(":attending_doctor", baseInfo.attending_doctor);
    query.bindValue(":bed_doctor", baseInfo.bed_doctor);
    query.bindValue(":acquisition_doctor", baseInfo.acquisition_doctor);
    query.bindValue(":patients_source_type", baseInfo.patients_source_type);
    query.bindValue(":patients_source", baseInfo.patients_source);
    query.bindValue(":admission_date", baseInfo.admission_date);
    query.bindValue(":icu_date", baseInfo.icu_date);
    query.bindValue(":principal_diagnosis", baseInfo.principal_diagnosis);

    if (!query.exec())
    {
        qDebug() << query.lastQuery() << query.lastError();
        return ;
    }

    m_baseID = query.lastInsertId().toInt();
}

void essentialLogic::QueryPatientInfoData(baseInformation* info)
{
    QSqlQuery query;

    query.prepare("SELECT "
        "serial_number, name, age, sex, height, actual_weight, ideal_weight, BSI, BMI, APACHE2, SOFA, "
        "admission_number, hospital, administrative_office, inpatient_ward, bed_number, "
        "treat_group_leader, attending_doctor, bed_doctor, acquisition_doctor,patients_source_type, patients_source, "
        "admission_date, icu_date, principal_diagnosis "
        "FROM basic_information WHERE id = :id;"
    );

    query.bindValue(":id", m_baseID);

    if (query.exec()) {
        while (query.next())
        {
            info->number = query.value(0).toString();
            info->name = query.value(1).toString();
            info->age = query.value(2).toString();
            info->sex = query.value(3).toString();
            info->height = query.value(4).toString();
            info->actual_weight = query.value(5).toString();
            info->ideal_weight = query.value(6).toString();
            info->BSI = query.value(7).toString();
            info->BMI = query.value(8).toString();
            info->APACHE2 = query.value(9).toString();
            info->SOFA = query.value(10).toString();
            info->admission_number = query.value(11).toString();
            info->hospital = query.value(12).toString();
            info->administrative_office = query.value(13).toString();
            info->inpatient_ward = query.value(14).toString();
            info->bed_number = query.value(15).toString();
            info->treat_group_leader = query.value(16).toString();
            info->attending_doctor = query.value(17).toString();
            info->bed_doctor = query.value(18).toString();
            info->acquisition_doctor = query.value(19).toString();
            info->patients_source_type = query.value(20).toString();
            info->patients_source = query.value(21).toString();
            info->admission_date = query.value(22).toString();
            info->icu_date = query.value(23).toString();
            info->principal_diagnosis = query.value(24).toString();
        }
    }
    else
    {
        qDebug() << "sql query error: "<< query.lastError() ;
    }
}

void essentialLogic::UpdataBaseInfo(baseInformation& baseInfo)
{
    QSqlQuery query;

    //基本信息
    query.prepare(
        "UPDATE  basic_information SET "
        "serial_number = :number, name = :name, age = :age, sex = :sex, height = :height, actual_weight = :actual_weight, ideal_weight = :ideal_weight, BSI = :BSI, BMI = :BMI, APACHE2 = :APACHE2, SOFA = :SOFA, "
        "admission_number = :admission_number, hospital = :hospital, administrative_office = :administrative_office, inpatient_ward = :inpatient_ward, bed_number =:bed_number,"
        "treat_group_leader = :treat_group_leader, attending_doctor = :attending_doctor, bed_doctor = :bed_doctor, acquisition_doctor = :acquisition_doctor, patients_source_type = :patients_source_type,patients_source = :patients_source, "
        "admission_date = :admission_date, icu_date = :icu_date, principal_diagnosis = :principal_diagnosis WHERE id = :id;"    
    );

    query.bindValue(":number", baseInfo.number);
    query.bindValue(":name", baseInfo.name);
    query.bindValue(":age", baseInfo.age);
    query.bindValue(":sex", baseInfo.sex);
    query.bindValue(":height", baseInfo.height);
    query.bindValue(":actual_weight", baseInfo.actual_weight);
    query.bindValue(":ideal_weight", baseInfo.ideal_weight);
    query.bindValue(":BSI", baseInfo.BSI);
    query.bindValue(":BMI", baseInfo.BMI);
    query.bindValue(":APACHE2", baseInfo.APACHE2);
    query.bindValue(":SOFA", baseInfo.SOFA);
    query.bindValue(":admission_number", baseInfo.admission_number);
    query.bindValue(":hospital", baseInfo.hospital);
    query.bindValue(":administrative_office", baseInfo.administrative_office);
    query.bindValue(":inpatient_ward", baseInfo.inpatient_ward);
    query.bindValue(":bed_number", baseInfo.bed_number);
    query.bindValue(":treat_group_leader", baseInfo.treat_group_leader);
    query.bindValue(":attending_doctor", baseInfo.attending_doctor);
    query.bindValue(":bed_doctor", baseInfo.bed_doctor);
    query.bindValue(":acquisition_doctor", baseInfo.acquisition_doctor);
    query.bindValue(":patients_source_type", baseInfo.patients_source_type);
    query.bindValue(":patients_source", baseInfo.patients_source);
    query.bindValue(":admission_date", baseInfo.admission_date);
    query.bindValue(":icu_date", baseInfo.icu_date);
    query.bindValue(":principal_diagnosis", baseInfo.principal_diagnosis);

    query.bindValue(":id", m_baseID);

    if (!query.exec())
    {
        qDebug() << query.lastQuery() << query.lastError();
        return;
    }
}

void essentialLogic::SetBaseID(int baseID)
{
    m_baseID = baseID;
}

int essentialLogic::GetBaseID()
{
    return m_baseID;
}
