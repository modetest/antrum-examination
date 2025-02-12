#include "SmoothCurve.h"

bool sortByY(const QPointF& p1, const QPointF& p2)
{
	return p1.y() < p2.y();//升序排列  
}

SmoothCurve::SmoothCurve(QObject* parent): QThread(parent)
{
	
}

SmoothCurve::~SmoothCurve()
{
}
QList<QPointF>& SmoothCurve::GetInVec()
{
	return InVec;
}

QList<QPointF>& SmoothCurve::GetOutVec()
{
	return OutVec;
}

void SmoothCurve::VecAppend(QPointF point)
{
	InVec.append(point);
}

void SmoothCurve::ClearVec()
{
	InVec.clear();
}

SFindPV& SmoothCurve::GetPV()
{
	return stFindPV;
}

void SmoothCurve::run()
{
	if (InVec.size() == 0)
		return;

	OutVec.clear();

	detectOutliers(InVec);

	OutVec = SmoothData(InVec, 200,1,20);
	//OutVec = exponentialSmoothing(InVec, 0.5);

	stFindPV.Pos_Peak.clear();
	stFindPV.Pos_Valley.clear();
	stFindPV.Pcnt = 0;
	stFindPV.Vcnt = 0;
	
	FindPV(&stFindPV);

	emit SmoothDone();
}

void SmoothCurve::FindPV(SFindPV* pFindPV)
{
	QVector<int> DiffVec;

	//向前差分 归一化
	for (size_t i = 0; i < OutVec.size()-1; i++)
	{
		if (OutVec[i + 1].y() - OutVec[i].y() > 0)
		{
			DiffVec.append(1);
		}
		else if (OutVec[i + 1].y() - OutVec[i].y() < 0)
		{
			DiffVec.append(-1);
		}
		else
		{
			DiffVec.append(0);
		}
	}


	//step 2 :对相邻相等的点进行领边坡度处理
	for (size_t i = 0; i < DiffVec.size()-1; i++)
	{
		if (DiffVec[i] == 0)
		{
			if (i == (DiffVec.size() - 2))
			{
				if (DiffVec[i - 1] >= 0)
					DiffVec[i] = 1;
				else
					DiffVec[i] = -1;
			}
			else
			{
				if (DiffVec[i + 1] >= 0)
					DiffVec[i] = 1;
				else
					DiffVec[i] = -1;
			}
		}
	}

	//step 3 :对相邻相等的点进行领边坡度处理
	for (int i = 0; i < DiffVec.size()-1; i++)
	{
	    if (DiffVec[i + 1] - DiffVec[i] == -2) //波峰识别
	        {
	            //pFindPV->Pos_Peak[pFindPV->Pcnt] = i + 1;
				pFindPV->Pos_Peak.append(i + 1);
	            pFindPV->Pcnt++;
	        }
	    else if (DiffVec[i + 1] - DiffVec[i] == 2) //波谷识别
			{
	            //pFindPV->Pos_Valley[pFindPV->Vcnt] = i + 1;
				pFindPV->Pos_Valley.append(i + 1);
	            pFindPV->Vcnt++;
	        }
	}
}

QList<QPointF> SmoothCurve::LinearFitted5(const QList<QPointF>& Datas)
{
	QList<QPointF> result;

	if (Datas.size() <= 5)
	{
		result = Datas;
		return result;
	}
	else
	{
		int N = Datas.size();
		double tmpY;

		std::vector<QPointF> tmpData(5);

		int i = 0;
		for (; i < N - 5; i++)
		{
			tmpData.clear();
			for (int j = i; j < i + 5; j++)
			{
				tmpData.push_back(Datas[j]);
			}
			std::sort(tmpData.begin(), tmpData.end(), sortByY);

			tmpY = (tmpData[1].y() + tmpData[2].y() + tmpData[3].y()) / 3;
			result.push_back(QPointF(Datas[i].x(), tmpY));
		}

		result[result.size() - 1].setX(Datas[Datas.size() - 1].x());
	}

	return result;
}

QList<QPointF> SmoothCurve::LinearSmooth5(const QList<QPointF>& Datas)
{
	QList<QPointF> result;

	if (Datas.size() <= 5)
	{
		result = Datas;
		return result;
	}
	else
	{
		int N = Datas.size();
		double tmpY;
		tmpY = (3.0 * Datas[0].y() + 2.0 * Datas[1].y() + Datas[2].y() - Datas[4].y()) / 5.0;
		if (tmpY <= 0)
		{
			tmpY = 0;
		}
		result.push_back(QPointF(Datas[0].x(), tmpY));

		tmpY = (4.0 * Datas[0].y() + 3.0 * Datas[1].y() + 2 * Datas[2].y() + Datas[3].y()) / 10.0;
		result.push_back(QPointF(Datas[1].x(), tmpY));


		for (int i = 2; i <= N - 3; i++)
		{
			tmpY = (Datas[i - 2].y() + Datas[i - 1].y() + Datas[i].y() + Datas[i + 1].y() + Datas[i + 2].y()) / 5.0;

			result.push_back(QPointF(Datas[i].x(), tmpY));

		}
		tmpY = (4.0 * Datas[N - 1].y() + 3.0 * Datas[N - 2].y() + 2 * Datas[N - 3].y() + Datas[N - 4].y()) / 10.0;
		result.push_back(QPointF(Datas[N - 2].x(), tmpY));

		tmpY = (3.0 * Datas[N - 1].y() + 2.0 * Datas[N - 2].y() + Datas[N - 3].y() - Datas[N - 5].y()) / 5.0;
		result.push_back(QPointF(Datas[N - 1].x(), tmpY));

	}

	return result;
}

QList<QPointF> SmoothCurve::SmoothData(const QList<QPointF>& Datas, int param1, int param2, int param3)
{
	if (Datas.size() < 5)
	{
		return Datas;
	}

	QList<QPointF> tmpDatas = Datas;
	
	//int smoothCount = Datas.size() / param1 + 1;
	
	int smoothCount = 1;

	for (int i = 0; i < smoothCount; i++)
	{
		tmpDatas = LinearFitted5(tmpDatas);
	}

	QList<QPointF>  result;
	for (int i = 0; i < tmpDatas.size(); )
	{
		result.push_back(tmpDatas.at(i));
		i += param2;
	}
	result.push_back(tmpDatas.at(tmpDatas.size() - 1));

	for (int i = 0; i < param3; i++)
	{
		result = LinearSmooth5(result);
	}
	return result;
}

// 指数平滑
QList<QPointF> SmoothCurve::exponentialSmoothing(const QList<QPointF>& Datas, double alpha)
{
	QList<QPointF> smoothed;
	smoothed.append(Datas[0]); // Initialize first smoothed value with first observed value

	for (int i = 1; i < Datas.size(); ++i) {

		QPointF tempPoint;
		tempPoint.setX(Datas[i].x());
		tempPoint.setY(alpha * Datas[i].y() + (1 - alpha) * smoothed[i - 1].y());

		smoothed.append(tempPoint);
	}
	qDebug() << smoothed.size();

	QList<QPointF>  result;


	for (int i = 0; i < 30; i++)
	{
		result = LinearSmooth5(smoothed);
	}

	return result;
}

double SmoothCurve::detectOutliers(const QList<QPointF>& Datas)
{
	double sum = 0.0;

	QList<double> temp;

	for (int i = 0; i < Datas.size();i++) {
		temp.append(Datas[i].y());
		sum += Datas[i].y();
	}
	float avg = sum / Datas.size();

	double variance = 0.0;
	for (int i = 0; i < Datas.size(); i++) {
		variance += pow(Datas[i].y() - avg, 2);
	}
	variance /= (double)Datas.size();

	double stdev = sqrt((double)variance);

	float median_value = 0.0;

	std::sort(temp.begin(), temp.end());
	int size = Datas.size();
	if (size % 2 == 0) {
		median_value = (temp[size / 2 - 1] + temp[size / 2]) / 2.0;
	}
	else {
		median_value = temp[size / 2];
	}

	double lower_bound = median_value - 1.5 * stdev;
	double upper_bound = median_value + 1.5 * stdev;

	if (lower_bound - 0.5 > 0)
	{
		lower_bound = lower_bound - 0.5;
	}
	
	upper_bound = upper_bound + 0.5;


	/*qDebug() << "Mean: " << avg ;
	qDebug() << "Standard Deviation: " << stdev ; 
	qDebug() << "Median: " << median_value ;
	qDebug() << "Lower Bound: " << lower_bound ;
	qDebug() << "Upper Bound: " << upper_bound ;*/

	for (int i = 0; i < Datas.size(); i++) {
		if (Datas[i].y() < lower_bound || Datas[i].y() > upper_bound) {
			//qDebug()<<"exception:" << Datas[i];
		}
	}

	return 0.0;
}
