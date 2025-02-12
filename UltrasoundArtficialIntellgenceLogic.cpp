#include "UltrasoundArtficialIntellgenceLogic.h"

UltrasoundArtficialIntellgenceLogic::UltrasoundArtficialIntellgenceLogic(QString rootPath,QObject* parent) : QObject(parent)
{
	m_sql = sqliteManage::getInstance();
	//m_VecTableData = new QVector<tableData>();
    //InitONNX
    this->rootPath = rootPath;
    QString m_model = rootPath + "/middle_end2end_static.onnx";
    net = cv::dnn::readNetFromONNX(m_model.toStdString().c_str());
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
}

UltrasoundArtficialIntellgenceLogic::~UltrasoundArtficialIntellgenceLogic()
{
	/*if (m_VecTableData != nullptr)
	{
		delete m_VecTableData;
		m_VecTableData = nullptr;
	}*/
}

void UltrasoundArtficialIntellgenceLogic::QueryPatientData(QVector<tableData>* vec, int pageIndex)
{
    auto index = (pageIndex * PageCount) - PageCount;
    QString s = "";
   
    s = QString(u8"SELECT id,serial_number,name,sex,age,hospital FROM basic_information ORDER BY id DESC LIMIT %1 ,%2;").arg(index).arg(PageCount);

	QSqlQuery query;

    if (query.exec(s)) {
        while (query.next())
        {
            tableData temp;
            temp.id = query.value(0).toInt();
            temp.serialNumber = query.value(1).toString();
            temp.name = query.value(2).toString();
            temp.sex = query.value(3).toString();
            temp.age = query.value(4).toInt();
            temp.hospital = query.value(5).toString();
            temp.collectCount = query.value(6).toInt();
            
            vec->append(temp);
        }
    }
    else {
        //打印sql语句错误信息
        qDebug() << "init table error" << query.lastError();
    }

    for (size_t i = 0; i < vec->size(); i++)
    {
        query.prepare("SELECT COUNT(id) AS count FROM collection WHERE ppid = :ppid");
        query.bindValue(":ppid", (*vec)[i].id);
        
        if (query.exec()) {
            while (query.next())
            {
                (*vec)[i].collectCount = query.value(0).toInt();
            }
        }
    }
}

void UltrasoundArtficialIntellgenceLogic::SreachData(QVector<tableData>* vec,QString& name)
{
    QString s = "";

    s = QString(u8"SELECT id,serial_number,name,sex,age,hospital FROM basic_information WHERE name = '%1' ORDER BY id DESC ;").arg(name);

    QSqlQuery query;

    if (query.exec(s)) {
        while (query.next())
        {
            tableData temp;
            temp.id = query.value(0).toInt();
            temp.serialNumber = query.value(1).toString();
            temp.name = query.value(2).toString();
            temp.sex = query.value(3).toString();
            temp.age = query.value(4).toInt();
            temp.hospital = query.value(5).toString();
            temp.collectCount = query.value(6).toInt();

            vec->append(temp);
        }
    }
    else {
        //打印sql语句错误信息
        qDebug() << "init table error" << query.lastError();
    }

    for (size_t i = 0; i < vec->size(); i++)
    {
        query.prepare("SELECT COUNT(id) AS count FROM collection WHERE ppid = :ppid");
        query.bindValue(":ppid", (*vec)[i].id);

        if (query.exec()) {
            while (query.next())
            {
                (*vec)[i].collectCount = query.value(0).toInt();
            }
        }
    }
}


void UltrasoundArtficialIntellgenceLogic::DeletePatientData(int id)
{
  
    //开启事务
    m_sql->GetDb()->transaction();
    
    QSqlQuery query;
    
    query.prepare("DELETE FROM basic_information WHERE id = :id");
    query.bindValue(":id",id);

    if (!query.exec()) {
        m_sql->GetDb()->rollback();
        qDebug() << "db error" << query.lastError();
        return;
    }

    QVector<int> clininalID;
    query.prepare("SELECT id FROM clinical_information WHERE pid = :pid;");
    query.bindValue(":pid", id);

    if (query.exec()) {
        while (query.next())
        {
            clininalID.append(query.value(0).toInt());
        }
    }
    else {
        //打印sql语句错误信息
        qDebug() << "db error:" << query.lastError();
    }

    query.prepare("DELETE FROM clinical_information WHERE pid = :id;");
    query.bindValue(":id", id);

    if (!query.exec()) {
        m_sql->GetDb()->rollback();
        qDebug() << "db error" << query.lastError();
        return;
    }

    for (size_t i = 0; i < clininalID.size(); i++)
    {
        query.prepare("DELETE FROM clinical_information_medicine WHERE pid = :clininalID;");
        query.bindValue(":clininalID", clininalID[i]);

        if (!query.exec()) {
            m_sql->GetDb()->rollback();
            qDebug() << "db error" << query.lastError();
            return;
        }

        query.prepare("DELETE FROM breathe_information WHERE pid = :clininalID;");
        query.bindValue(":clininalID", clininalID[i]);

        if (!query.exec()) {
            m_sql->GetDb()->rollback();
            qDebug() << "db error" << query.lastError();
            return;
        }

        query.prepare("DELETE FROM nutrition_information WHERE pid = :clininalID;");
        query.bindValue(":clininalID", clininalID[i]);

        if (!query.exec()) {
            m_sql->GetDb()->rollback();
            qDebug() << "db error" << query.lastError();
            return;
        }

        query.prepare("DELETE FROM collection WHERE pid = :clininalID;");
        query.bindValue(":clininalID", clininalID[i]);
        if (!query.exec()) {
            m_sql->GetDb()->rollback();
            qDebug() << "db error" << query.lastError();
            return;
        }

    }

    m_sql->GetDb()->commit();

    //删除磁盘信息
    auto delPath = rootPath + "/resultSet/"+QString::number(id);
    QDir directory(delPath);
    bool success = directory.removeRecursively();
    if (!success)
        qDebug() << "delete dir error!";
}

cv::dnn::Net* UltrasoundArtficialIntellgenceLogic::GetModelNet()
{
    return &net;
}


