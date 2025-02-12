#include "BaseInfoLogic.h"

BaseInfoLogic::BaseInfoLogic(QObject* parent) : QObject(parent)
{
    m_sql = sqliteManage::getInstance();
}
BaseInfoLogic::~BaseInfoLogic()
{
}

bool BaseInfoLogic::newClinicalRecord(int& baseID,int& clid, const clinicalInformation& clInfo, QVector<nutritionalInformation> nuInfo)
{
    bool success = false;
  
    QSqlQuery query;

    //临床信息
    query.prepare(
        "INSERT INTO clinical_information (pid,"
        "heart_rate, heart_Rhythm, SBP, DBP, MAP, SPO2, PI, RR, USCode,AcTime "
        ") VALUES (:pid,"
        ":hreat_rate, :heart_Rhythm, :SBP, :DBP, :MAP, :SPO2, :PI, :RR, :USCode, :AcTime"
        ")"
    );

    query.bindValue(":pid", baseID);
    query.bindValue(":hreat_rate", clInfo.hreat_rate);
    query.bindValue(":heart_Rhythm", clInfo.heart_Rhythm);
    query.bindValue(":SBP", clInfo.SBP);
    query.bindValue(":DBP", clInfo.DBP);
    query.bindValue(":MAP", clInfo.MAP);
    query.bindValue(":SPO2", clInfo.SPO2);
    query.bindValue(":PI", clInfo.PI);
    query.bindValue(":RR", clInfo.RR);
    query.bindValue(":USCode", clInfo.USCode);
    query.bindValue(":AcTime", clInfo.AcTime);

    if (!query.exec()) {
        qDebug() << query.lastQuery() << query.lastError();
        m_sql->GetDb()->rollback();
        return success;
    }
    auto clID = query.lastInsertId().toString();
    clid = clID.toInt();
    //呼吸
    int lenBreatheSize = clInfo.vecBreathe.size();
    QString temp;
    if (lenBreatheSize > 0)
    {
        temp = "(" + clID + ",'" + clInfo.vecBreathe[0].pattern + "'," + clInfo.vecBreathe[0].LMin + "," + clInfo.vecBreathe[0].FiO2 + "," + clInfo.vecBreathe[0].IPAP + "," + clInfo.vecBreathe[0].EPAP + "," + clInfo.vecBreathe[0].PEEP + ",'" + clInfo.vecBreathe[0].hreathModel + "')";

        for (size_t i = 1; i < lenBreatheSize; i++)
        {
            temp = temp + "," + "(" + clID + ",'" + clInfo.vecBreathe[i].pattern + "'," + clInfo.vecBreathe[i].LMin + "," + clInfo.vecBreathe[i].FiO2 + "," + clInfo.vecBreathe[i].IPAP + "," + clInfo.vecBreathe[i].EPAP + "," + clInfo.vecBreathe[i].PEEP + ",'" + clInfo.vecBreathe[i].hreathModel + "')";
        }

        QString str = QString("INSERT INTO breathe_information(pid, "
            "pattern, LMin, FiO2, IPAP, EPAP, PEEP, hreathModel "
            ") VALUES %1").arg(temp);

        if (!query.exec(str)) {
            qDebug() << query.lastQuery() << query.lastError();
            m_sql->GetDb()->rollback();
            return success;
        }
    }
    //药物
    QVector<medicData> vecData;

    int lenVaSize = clInfo.vecActivity.size();
    int lenAnSize = clInfo.vecAnalgesia.size();
    int lenCaSize = clInfo.vecCalm.size();
    int lenMuSize = clInfo.vecMuscleRelaxation.size();

    for (size_t i = 0; i < lenVaSize; i++)
    {
        medicData tempdata;
        tempdata.type = VASOACTIVITY;
        tempdata.me.name = clInfo.vecActivity[i].name;
        tempdata.me.TotalVolume = clInfo.vecActivity[i].TotalVolume;
        tempdata.me.MediaVolume = clInfo.vecActivity[i].MediaVolume;
        tempdata.me.PumpingSpeed = clInfo.vecActivity[i].PumpingSpeed;
        tempdata.me.PumpingVolume = clInfo.vecActivity[i].PumpingVolume;
        tempdata.me.Score = clInfo.vecActivity[i].Score;
        vecData.append(tempdata);
    }
    for (size_t i = 0; i < lenAnSize; i++)
    {
        medicData tempdata;
        tempdata.type = ANALGESIA;
        tempdata.me.name = clInfo.vecAnalgesia[i].name;
        tempdata.me.TotalVolume = clInfo.vecAnalgesia[i].TotalVolume;
        tempdata.me.MediaVolume = clInfo.vecAnalgesia[i].MediaVolume;
        tempdata.me.PumpingSpeed = clInfo.vecAnalgesia[i].PumpingSpeed;
        tempdata.me.PumpingVolume = clInfo.vecAnalgesia[i].PumpingVolume;
        tempdata.me.Score = clInfo.vecAnalgesia[i].Score;
        vecData.append(tempdata);
    }
    for (size_t i = 0; i < lenCaSize; i++)
    {
        medicData tempdata;
        tempdata.type = CALM;
        tempdata.me.name = clInfo.vecCalm[i].name;
        tempdata.me.TotalVolume = clInfo.vecCalm[i].TotalVolume;
        tempdata.me.MediaVolume = clInfo.vecCalm[i].MediaVolume;
        tempdata.me.PumpingSpeed = clInfo.vecCalm[i].PumpingSpeed;
        tempdata.me.PumpingVolume = clInfo.vecCalm[i].PumpingVolume;
        tempdata.me.Score = clInfo.vecCalm[i].Score;
        vecData.append(tempdata);
    }
    for (size_t i = 0; i < lenMuSize; i++)
    {
        medicData tempdata;
        tempdata.type = MUSCLERELAXATION;
        tempdata.me.name = clInfo.vecMuscleRelaxation[i].name;
        tempdata.me.TotalVolume = clInfo.vecMuscleRelaxation[i].TotalVolume;
        tempdata.me.MediaVolume = clInfo.vecMuscleRelaxation[i].MediaVolume;
        tempdata.me.PumpingSpeed = clInfo.vecMuscleRelaxation[i].PumpingSpeed;
        tempdata.me.PumpingVolume = clInfo.vecMuscleRelaxation[i].PumpingVolume;
        tempdata.me.Score = clInfo.vecMuscleRelaxation[i].Score;
        vecData.append(tempdata);
    }

    int len = vecData.size();

    if (len > 0)
    {
        temp = "(" + clID + ",'" + vecData[0].me.name + "'," + vecData[0].me.TotalVolume + "," + vecData[0].me.MediaVolume + "," + vecData[0].me.PumpingSpeed + "," + vecData[0].me.PumpingVolume + "," + vecData[0].me.Score + "," + QString::number(vecData[0].type, 10) + ")";

        for (size_t i = 1; i < len; i++)
        {
            temp = temp + "," + "(" + clID + ",'" + vecData[i].me.name + "'," + vecData[i].me.TotalVolume + "," + vecData[i].me.MediaVolume + "," + vecData[i].me.PumpingSpeed + "," + vecData[i].me.PumpingVolume + "," + vecData[i].me.Score + "," + QString::number(vecData[i].type, 10) + ")";
        }


        QString str = QString("INSERT INTO clinical_information_medicine(pid, "
            "name, TotalVolume, MediaVolume, PumpingSpeed, PumpingVolume, Score, type"
            ") VALUES %1").arg(temp);

        if (!query.exec(str)) {
            qDebug() << query.lastQuery() << query.lastError();
            m_sql->GetDb()->rollback();
            return success;
        }

    }
    //营养
    int nuSize = nuInfo.size();

    if (nuSize > 0)
    {
        temp = "(" + clID + ",'" + nuInfo[0].name + "','" + nuInfo[0].Formula + "'," + nuInfo[0].Volume + "," + nuInfo[0].Kcal + "," + nuInfo[0].Protein + ",'" + nuInfo[0].StartTime + "')";

        for (size_t i = 1; i < nuSize; i++)
        {
            temp = temp + "," + "(" + clID + ",'" + nuInfo[i].name + "','" + nuInfo[i].Formula + "'," + nuInfo[i].Volume + "," + nuInfo[i].Kcal + "," + nuInfo[i].Protein + ",'" + nuInfo[i].StartTime + "')";
        }

        QString str = QString("INSERT INTO nutrition_information(pid, "
            "name, Formula, Volume, Kcal, Protein, StartTime"
            ") VALUES %1").arg(temp);

        if (!query.exec(str)) {
            qDebug() << query.lastQuery() << query.lastError();
            m_sql->GetDb()->rollback();
            return success;
        }
    }

    query.finish();
    m_sql->GetDb()->commit();
    success = true;
    return success;
}

void BaseInfoLogic::QueryClinicalData(int& baseID, QVector<collectionData>& data)
{
    QSqlQuery query;
    QVector<int> clinicalIDs;
    query.prepare("SELECT id FROM clinical_information WHERE pid = :id");
    query.bindValue(":id",baseID);
    if (query.exec())
    {
        while (query.next())
        {
            clinicalIDs.append(query.value(0).toInt());
        }
    }

    QString clinicalStr;
    if (clinicalIDs.size() != 0)
        clinicalStr = QString::number(clinicalIDs[0]);
    for (size_t i = 1; i < clinicalIDs.size(); i++)
    {
        clinicalStr = clinicalStr + ","+ QString::number(clinicalIDs[i]);
    }
    
    QString s = QString("SELECT pid,AcTime,cycleTime,count,diastoleArea,contractArea FROM collection WHERE pid IN (%1)").arg(clinicalStr);
    if (query.exec(s))
    {
        while (query.next())
        {
            collectionData temp;
            temp.pid = query.value(0).toString();
            temp.acTime = query.value(1).toString();
            temp.cycleTime = query.value(2).toString();
            temp.count = query.value(3).toString();
            temp.diastoleArea = query.value(4).toString();
            temp.contractArea = query.value(5).toString();
            data.append(temp);
        }
    }
    query.finish();
}

void BaseInfoLogic::DeletePatientData(QString& rootPath, int baseID, int clinicalID)
{
    m_sql->GetDb()->transaction();

    QSqlQuery query;

    query.prepare("DELETE FROM clinical_information WHERE id = :clinicalID;");
    query.bindValue(":clinicalID", clinicalID);

    if (!query.exec()) {
        m_sql->GetDb()->rollback();
        qDebug() << "db error" << query.lastError();
        return;
    }

    query.prepare("DELETE FROM clinical_information_medicine WHERE pid = :clininalID;");
    query.bindValue(":clininalID", clinicalID);

    if (!query.exec()) {
        m_sql->GetDb()->rollback();
        qDebug() << "db error" << query.lastError();
        return;
    }
    
    query.prepare("DELETE FROM breathe_information WHERE pid = :clininalID;");
    query.bindValue(":clininalID", clinicalID);

    if (!query.exec()) {
        m_sql->GetDb()->rollback();
        qDebug() << "db error" << query.lastError();
        return;
    }

    query.prepare("DELETE FROM nutrition_information WHERE pid = :clininalID;");
    query.bindValue(":clininalID", clinicalID);

    if (!query.exec()) {
        m_sql->GetDb()->rollback();
        qDebug() << "db error" << query.lastError();
        return;
    }

    query.prepare("DELETE FROM collection WHERE pid = :clininalID;");
    query.bindValue(":clininalID", clinicalID);
    if (!query.exec()) {
        m_sql->GetDb()->rollback();
        qDebug() << "db error" << query.lastError();
        return;
    }
    m_sql->GetDb()->commit();

    //删除磁盘信息
    auto delPath = rootPath + "/resultSet/" + QString::number(baseID)+"/"+ QString::number(clinicalID);
    QDir directory(delPath);
    bool success = directory.removeRecursively();
    if (!success)
        qDebug() << "delete dir error!";
}
