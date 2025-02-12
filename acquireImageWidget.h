#ifndef ACQUIREIMAGE_H
#define ACQUIREIMAGE_H

#include <QWidget>
#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <qxyseries.h>
#include <QValueAxis>
#include <QScrollArea>
#include <QTableWidget>
#include <QLineEdit>
#include <QHeaderView>
#include <QFileDialog>
#include <QTime>
#include <QTimer>
#include <QDateTime>
#include <qpixmap.h>
#include <qimage.h>
#include <QProcess>
#include <QAxObject>
#include <QDateTimeEdit>
#include <QBoxLayout>
#include <QPushbutton>
#include <QLabel>
#include <QMessageBox>
#include <QDesktopServices>
#include <QScreen>
#include <QMovie>
//#include <QFile>

//#include <QtWinExtras/QtWinExtras>   //winAPI，需要使用它来获取opencv窗口句柄

#include "acquireImageLogic.h"
#include "SmoothCurve.h"
#include "waitDialog.h"
#include "infotabwidget.h"
#include "nutritiontab.h"
#include "form.h"
#include "computeDistanceDialog.h"
#include "cameraSelect.h"



namespace Ui {
class acquireImage;
}

const int MAXSECOND = 10;

struct acTableData
{
    int Number;
    int time;
    int count;
    float diastoleArea;
    float contractArea;
};

class acquireImage : public QWidget
{
    Q_OBJECT

public:
    explicit acquireImage(cv::dnn::Net* net,int id,int clid,QString path,QWidget *parent = nullptr);
    ~acquireImage();
  
public slots:
    void OnStartAcquireButton();
    void OnDataCorrectButton();
    void OnRestartAcquireButton();
    void OnReAnalysisButton();
    void OnExportButton();
    void OnTimeButton();
    
    void NotMatch();
    void Match();
    void ComputeDistance(QList<QPoint> points);

    void DoneImage(float a);
    void DoneImageAnalysis(float a);
    void reAnalysisFinish();
    void ViewImage(bool b);
    void SmoothDone();
    void OnTimeout();
    void OnEndTimeout();
    void OnTimeChange(const QDateTime& dateTime);

private:
    void setTableData();
    void saveData();
    void writeExcel();
    void AddExcelToWord();
    void saveImage();
    void showWaitWidget();

private:
    Ui::acquireImage *ui;
    
    acquireImageLogic* m_pLogic;
    AnalysisLogic* m_pAnLogic;

    QWidget* TimeWidget;
    QWidget* widget;
    QWidget* contentWidget;
    InfoTabWidget* baseTab;
    Form* clinicalTab;
    nutritionTab* nutTab;


    QtCharts::QLineSeries* m_seriesl;
    QtCharts::QLineSeries* newSeries;
    QtCharts::QValueAxis* axisX;
    QtCharts::QValueAxis* axisY;
    int axisRangeX;
    int axisRangeY;

    QGridLayout* buttonLayout;
    QPushButton* timeButton;
    QPushButton* timeOk;
    QPushButton* backButton;
    QLineEdit* edit;
    QPushButton* exportBut;
    QPushButton* startAcquire;
    QPushButton* dataCorrect;
    QPushButton* restartAcquire;
    QPushButton* reAnalysis;

    QDateTimeEdit* startTimeEdit;
    QLineEdit* endTimeEdit;
    QLineEdit* auTimeEdit;//周期时间
    QLineEdit* avgDiastoleEdit;//舒展
    QLineEdit* avgContractEdit; //收缩
    QLineEdit* dcCountEdit;

    QtCharts::QChart* m_chart;
    QtCharts::QChartView* chartView;

    QTimer* timer;
    QTimer* endTimer;
    QLabel* imageLabel;

    QTableWidget* table;

    QString baseID;
    QString clID;
    QString filePath;

    int o ;
    SmoothCurve* m_pSmooth;
    waitDialog* m_watiDialog;
    QMovie* movi;
    QVector<acTableData> data;
    QAxObject* excel;

    bool isRuntime;

    int times;

    int endTime;

   
};

#endif // ACQUIREIMAGE_H
