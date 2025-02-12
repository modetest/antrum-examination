#pragma once

#include <filesystem>

#include <qobject.h>
#include <qdebug.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qfile.h>
#include <qthread.h>
#include <qvector.h>
#include <QMutex>
#include <QMutexLocker>
#include <QCameraInfo>
#include <QDir>
#include <QWaitCondition>
#include <QBuffer>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui_c.h>

#include "Data.h"
#include "sqliteManage.h"
#include "matchRuler.h"
#include "tinyxml2.h"
#include "reportWidget.h"


struct LabelmeJsonData
{
	std::string label;
	std::vector<cv::Point> pointArray;
};

class acquireImageLogic : public QThread
{
	Q_OBJECT
public:
	acquireImageLogic(cv::dnn::Net* net,QString rootPath,QObject* parent = nullptr);
	~acquireImageLogic();

	void SetDiskPath(QString& rootPath,int baseID,int clinialID);
	void removeClinicalData(QString& rootPath, QString& baseID, QString& clinialID);
	void reAnalysisData();

	void SetIsRun(bool b);
	bool GetIsRun();

	void SetType(short t);
	void setUnitZore();
	cv::Mat& GetMat();
	void saveData(int ppid,collectionData& data);
	void saveImage(QString& path, QImage& mainPage, QImage& base, QImage& clinical, QImage& nu);
	void saveCurveImage(QString& path, QImage& mainPage);

	void saveImage(QString& path, QImage& mainPage, QImage& base, QImage& clinical);
	void SetUnit(QList<QPoint>& u);
	void pauseThread();
	void resumeThread();
	float GetUnit();

	short GetCameraCount();
	void SetCameraCount(short count);
	void SetCameraIndex(int index);


protected:
	void run();

Q_SIGNALS:
	void doneMat(float a);
	void ViewImage(bool b);
	void NotMatch();
	void Match();

private:
	//void queryData(int baseID, int clID,baseInformation& baseInfo, clinicalInformation& clinInfo, QVector<nutritionalInformation>& nuInfo, collectionData& collectInfo);
	//void insertImageToTemplate(QString& path, QString& ImgPath);
	int connected_component(cv::Mat& image);
	float colorizeSegmentation(const cv::Mat& score, cv::Mat& segm, std::vector<cv::Vec3b> colors, cv::Mat& binary, float thres, std::string& path);
	void labelmeJson(std::string& path, LabelmeJsonData* gastricAntrum, LabelmeJsonData* leftLobeLiver, LabelmeJsonData* abdominalAorta, int width, int height);

	//void print_child_nodes(tinyxml2::XMLNode* root,QString &imgPath);
	//QString ImagToBase64(QString& strImagePath);
private:
	cv::dnn::Net* m_net;

	bool m_Thread_Video_stop;
	bool m_Thread_Video_pause;
	QMutex m_Qmutex_pauseLock;

	QMutex Mutex;
	std::vector<cv::Vec3b> colors;
	std::vector<int> cls_ids;

	cv::Mat m_frame;

	QString m_SrcVideoPath;
	std::string picPath;
	std::string videoPath;
	volatile bool isRun;
	QMutex m_mutex;
	int srcFrameWidth;
	int srcFrameHeight;

	short cameraCount;
	short cameraIndex;
	short m_sType; // 100 200;
	matchRuler* m_pMatchRler;

	float unit;
};

class AnalysisLogic : public QThread
{
	Q_OBJECT
public:
	AnalysisLogic(QString picPath,QObject* parent = nullptr);
	~AnalysisLogic();

	void SetIsRun(bool b);
	bool GetIsRun();
	cv::Mat& GetMat();
	void SetUnit(float u);

signals:
	void AnalysisMat(float a);
	void Finish();

protected:
	void run();
private:
	void analysisJson(std::string jsonPath, std::vector<cv::Point>& vecPoints);

private:
	volatile bool isRun;
	QString picPath;
	std::string writerSegPath;
	QMutex mutex;
	cv::Mat m_frame;
	float unit;
	
};

