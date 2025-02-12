#pragma once

#include <QVector>
#include <QObject>
#include <qdebug.h>
#include <QDir>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui_c.h>

#include "Data.h"
#include "sqliteManage.h"

const int PageCount = 10;

struct tableData
{
    int  id;
    QString serialNumber;
    QString name;
    QString  sex;
    int age;
    QString hospital;
    int  collectCount;
};

class UltrasoundArtficialIntellgenceLogic :
    public QObject
{
    Q_OBJECT
public:
    UltrasoundArtficialIntellgenceLogic(QString rooPath,QObject* parent = nullptr);
    ~UltrasoundArtficialIntellgenceLogic();

public:
    void QueryPatientData(QVector<tableData>* vec,int pageIndex);
    void SreachData(QVector<tableData>* vec,QString& name);
    void DeletePatientData(int id);

    cv::dnn::Net* GetModelNet();

private:
    QString rootPath;
    sqliteManage* m_sql;
    cv::dnn::Net net;
};

