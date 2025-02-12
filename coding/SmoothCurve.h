#pragma once
#include <qthread.h>
#include <qdebug.h>
#include <cmath>
#include <algorithm>


typedef struct _tag_FindPV
{
	QVector<int> Pos_Peak;    //波峰位置存储
	QVector<int> Pos_Valley;  //波谷位置存储
	int Pcnt;                //所识别的波峰计数
	int Vcnt;
}SFindPV;

class SmoothCurve : public QThread
{
	Q_OBJECT
public:
	SmoothCurve(QObject* parent = nullptr);
	~SmoothCurve();

	QList<QPointF>& GetInVec();
	QList<QPointF>& GetOutVec();
	void VecAppend(QPointF point);
	void ClearVec();
	SFindPV& GetPV();

signals:
	void SmoothDone();

protected:
	void run();

private:

	void FindPV(SFindPV* pFindPV);

	QList<QPointF> LinearFitted5(const QList<QPointF>& Datas);

	QList<QPointF> LinearSmooth5(const QList<QPointF>& Datas);

	QList<QPointF> SmoothData(const QList<QPointF>& Datas, int p1,int p2 ,int p3);

	QList<QPointF> exponentialSmoothing(const QList<QPointF>& Datas, double alpha);

	double detectOutliers(const QList<QPointF>& Datas);

private:
	QList<QPointF> InVec;
	QList<QPointF> OutVec;

	SFindPV stFindPV;
};

