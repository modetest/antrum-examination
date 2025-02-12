#include "acquireImageWidget.h"
#include "ui_acquireImage.h"

acquireImage::acquireImage(cv::dnn::Net* net, int id, int clid, QString path, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::acquireImage)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);

    QIcon icon(":/antrum/resouce/mainIcon.png");
    this->setWindowTitle(u8"胃窦监测");
    this->setWindowIcon(icon);

   
    QString RootPath = QApplication::applicationDirPath();

    if (path.toInt() == 100)
    {
        isRuntime = true;
    }
    else
    {
        isRuntime = false;
    }
    endTime = 0;
    baseID = QString::number(id);
    clID = QString::number(clid);
    times = 1;

    //背景色
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Window, QBrush(QPixmap(":/antrum/resouce/mainImage.png")));
    this->setPalette(pal);

    m_pLogic = new acquireImageLogic(net, path,this);
    m_pLogic->SetDiskPath(RootPath, id, clid);
    QObject::connect(m_pLogic, SIGNAL(doneMat(float)), this, SLOT(DoneImage(float)),Qt::BlockingQueuedConnection);
    QObject::connect(m_pLogic, SIGNAL(ViewImage(bool)), this, SLOT(ViewImage(bool)), Qt::BlockingQueuedConnection);
    QObject::connect(m_pLogic, SIGNAL(NotMatch()), this, SLOT(NotMatch()), Qt::BlockingQueuedConnection);
    QObject::connect(m_pLogic, SIGNAL(Match()), this, SLOT(Match()), Qt::BlockingQueuedConnection);
    

    auto picPath = QApplication::applicationDirPath() +"/resultSet/"+baseID+ "/"+clID;
    m_pAnLogic = new AnalysisLogic(picPath, this);
    QObject::connect(m_pAnLogic, SIGNAL(AnalysisMat(float)), this, SLOT(DoneImageAnalysis(float)), Qt::BlockingQueuedConnection);
    QObject::connect(m_pAnLogic, SIGNAL(Finish()), this, SLOT(reAnalysisFinish()), Qt::BlockingQueuedConnection);

    axisRangeX = 50;
    axisRangeY = 1;
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

  /*  titleBarWidget* title = new titleBarWidget(this);
    mainLayout->addWidget(title);*/

    //返回按钮
    backButton = new QPushButton(this);
    backButton->setFixedSize(QSize(110, 40));
    backButton->setText(u8"返回");
    backButton->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/button-back.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333; }");
    mainLayout->addWidget(backButton);

    connect(backButton, &QPushButton::clicked, this, [&]() {
        if (m_pLogic->isRunning())
        {
            qDebug() << "terminate";
            m_pLogic->terminate();
        }
        this->close();
     
        });

    //滚动
    QScrollArea* srollArea = new QScrollArea();
    srollArea->setWidgetResizable(true);
    srollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   
    contentWidget = new QWidget(this);
    contentWidget->setObjectName("contentWidget");
    srollArea->setStyleSheet("#contentWidget{background-color:#171f2b;border:none}QScrollArea{background-color:#171f2b;border:none} QScrollBar{background-color:#323E55;;border:none } QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background:#171f2b;}");

    srollArea->setWidget(contentWidget);

    QVBoxLayout* scrollLayout = new QVBoxLayout(contentWidget);
    //图像
    QHBoxLayout* upLayout = new QHBoxLayout();
    upLayout->setContentsMargins(10, 10, 10, 10);
    scrollLayout->addLayout(upLayout);
    QVBoxLayout* imageButtonLayout = new QVBoxLayout();//左上图像按钮
    buttonLayout = new QGridLayout(); //按钮

    imageLabel = new QLabel(contentWidget);
    imageLabel->setFixedSize(QSize(804, 604));
    imageLabel->setStyleSheet("QLabel{border-width: 2px;border-style: solid;border-color: #0887E1;}");
    imageButtonLayout->addWidget(imageLabel);

    //嵌入窗口
    //cv::namedWindow(ImageTitle, cv::WINDOW_GUI_NORMAL);
    //HWND hWnd = (HWND)cvGetWindowHandle(ImageTitle.c_str());//获得名为 title 窗口的句柄。
    //HWND hParent = GetParent(hWnd);//获得名为 title 窗口的父句柄。
    //SetParent(hWnd, (HWND)image->winId());//把label的句柄设为 hWnd 的父句柄。(HWND)ui.label_10->winId()获取label的句柄。
    ////ShowWindow(hParent, SW_HIDE);//显示hParent句柄代表的窗口，第二个参数设为SW_HIDE，隐藏之前的窗口。
    //cv::resizeWindow(ImageTitle, cv::Size(image->width(), image->height()));//重置窗口大小，以适应label的大小。

    startAcquire = new QPushButton(contentWidget);
    dataCorrect = new QPushButton(contentWidget);
    restartAcquire = new QPushButton(contentWidget);
    reAnalysis = new QPushButton(contentWidget);

    startAcquire->setFixedSize(QSize(180, 50));
    dataCorrect->setFixedSize(QSize(180, 50));
    restartAcquire->setFixedSize(QSize(180, 50));
    reAnalysis->setFixedSize(QSize(180, 50));

    startAcquire->setText(u8"开始采集");
    dataCorrect->setText(u8"数据矫正");
    restartAcquire->setText(u8"重新采集");
    reAnalysis->setText(u8"重新分析");

    startAcquire->setEnabled(true);
    dataCorrect->setEnabled(false);
    restartAcquire->setEnabled(false);
    reAnalysis->setEnabled(false);


    startAcquire->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/button-ground.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{background-image: url(:/antrum/resouce/button-select.png);color:#333; }");
    dataCorrect->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/button-ground.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{background-image: url(:/antrum/resouce/button-select.png);color:#333;}");
    restartAcquire->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/button-ground.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{background-image: url(:/antrum/resouce/button-select.png);color:#333;  }");
    reAnalysis->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/button-ground.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{background-image: url(:/antrum/resouce/button-select.png);color:#333;  }");

    QObject::connect(startAcquire, SIGNAL(clicked()), this, SLOT(OnStartAcquireButton()));
    QObject::connect(dataCorrect, SIGNAL(clicked()), this, SLOT(OnDataCorrectButton()));
    QObject::connect(restartAcquire, SIGNAL(clicked()), this, SLOT(OnRestartAcquireButton()));
    QObject::connect(reAnalysis, SIGNAL(clicked()), this, SLOT(OnReAnalysisButton()));

    buttonLayout->addWidget(startAcquire, 0, 0, Qt::Alignment(Qt::AlignRight));
    if (isRuntime)
    {
        TimeWidget = new QWidget(contentWidget);
        QHBoxLayout* timeLay = new QHBoxLayout(TimeWidget);
        timeLay->setContentsMargins(0, 0, 0, 0);
        timeLay->setSpacing(6);

        widget = new QWidget(TimeWidget);
        widget->setObjectName("timeWidget");
        widget->setFixedSize(135, 36);
        widget->setStyleSheet("#timeWidget{background-image: url(:/antrum/resouce/Rectangle_border.png)}");


        timeButton = new QPushButton(TimeWidget);
        timeButton->setFixedSize(23, 26);
        timeButton->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/time_1.png); border:none}QPushButton:hover{background-image: url(:/antrum/resouce/time_2.png);}QPushButton:pressed{background-image: url(:/antrum/resouce/time_2.png); }");
        buttonLayout->addWidget(timeButton, 0, 1, Qt::Alignment(Qt::AlignLeft));
        connect(timeButton,&QPushButton::clicked , this, [&]() {
            timeButton->setVisible(false);
            TimeWidget->setVisible(true);
            });
   
        QHBoxLayout* lay = new QHBoxLayout(widget);
        lay->setContentsMargins(2, 0, 3, 0);
        QPushButton* timeBtn = new QPushButton(widget);
        timeBtn->setFixedSize(23, 26);
        timeBtn->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/time_1.png); border:none}QPushButton:hover{background-image: url(:/antrum/resouce/time_2.png);}QPushButton:pressed{background-image: url(:/antrum/resouce/time_2.png); }");
        connect(timeBtn, &QPushButton::clicked, this, [&]() {
            TimeWidget->setVisible(false);
            timeButton->setVisible(true);
            });
        edit = new QLineEdit(widget);
        edit->setStyleSheet("QLineEdit{background-image:none;background-color:#171F2B;color:white;border:none;font:18px}");
        edit->setPlaceholderText(u8"输入时间");
        edit->setValidator(new QIntValidator(0,999, edit));

        QLabel* labe = new QLabel(widget);
        labe->setText(u8"分");
        labe->setStyleSheet("QLabel{color:white;font:18px}");

        timeOk = new QPushButton(widget);
        timeOk->setText(u8"确定");
        timeOk->setFixedSize(62, 26);
        timeOk->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/time_ok_1.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{background-image: url(:/antrum/resouce/time_ok_2.png);color:#333; }");
        connect(timeOk, &QPushButton::clicked, this, &acquireImage::OnTimeButton);

        lay->addWidget(timeBtn);
        lay->addWidget(edit);
        lay->addWidget(labe);

        timeLay->addWidget(widget);
        timeLay->addWidget(timeOk);

        TimeWidget->setVisible(false);
        buttonLayout->addWidget(TimeWidget, 0, 1, Qt::Alignment(Qt::AlignLeft));

        auto cameras = QCameraInfo::availableCameras();//获取可用摄像头设备列表
        
        m_pLogic->SetCameraCount(cameras.size());

        for (size_t i = 0; i < cameras.size(); i++)
        {
            qDebug()<< cameras.at(i).description();
        }
     
    }
    buttonLayout->addWidget(dataCorrect, 0, 3, Qt::Alignment(Qt::AlignHCenter));
    buttonLayout->addWidget(restartAcquire, 1, 0, Qt::Alignment(Qt::AlignRight));
    buttonLayout->addWidget(reAnalysis, 1, 3, Qt::Alignment(Qt::AlignHCenter));

    imageButtonLayout->addLayout(buttonLayout);
    upLayout->addLayout(imageButtonLayout);

    //信息显示
    QTabWidget* tab = new QTabWidget(contentWidget);

    tab->setStyleSheet(" QTabWidget::pane{} QTabBar::tab{background-color: #242d3f; color:white; font:21px; height:25; width:120;}  QTabBar::tab:selected{ background-color:#2cbfff; color:white; }");

    baseTab = new InfoTabWidget(id);
    tab->addTab(baseTab, u8"基本信息");

    clinicalTab = new Form(id);
    tab->addTab(clinicalTab, u8"临床信息");

    nutTab = new nutritionTab(id);
    tab->addTab(nutTab, u8"营养信息");

    upLayout->addWidget(tab);

    //曲线图
    QFont font;
    font.setPointSize(21);

    QFont font10;
    font10.setPointSize(10);
    
    m_chart = new  QtCharts::QChart();
    m_chart->setTitle(u8"胃窦面积变化率");
    m_chart->setTitleFont(font);
    m_chart->setTitleBrush(QColor(255, 255, 255));

    // 创建温度曲线图1并设置属性
    m_seriesl = new QtCharts::QLineSeries(contentWidget);
    m_seriesl->setName(u8"胃窦");
    m_seriesl->setColor(QColor("#3cb7f7"));
    m_seriesl->setPen(QPen(QColor("#3cb7f7"), 2));
    

    newSeries = new  QtCharts::QLineSeries(contentWidget);
    newSeries->setColor(QColor("#FF0000"));
    newSeries->setName(u8"拟合曲线");
    QPen tmpPen(QColor(QColor("#00FF00")), 2);
    newSeries->setPen(tmpPen);

    m_chart->legend()->setFont(font);
    m_chart->legend()->setVisible(false);
    m_seriesl->setBrush(QColor(255, 255, 255));
    m_chart->addSeries(m_seriesl);
    newSeries->setBrush(QColor(255, 255, 255));
    m_chart->addSeries(newSeries);

    // 设置横轴属性
    axisX = new QtCharts::QValueAxis(m_chart);
    axisX->setRange(0, axisRangeX);
    axisX->setGridLineVisible(false);
    axisX->setTickCount(20);
    axisX->setTitleText(u8"帧 (frame)");
    m_chart->addAxis(axisX, Qt::AlignBottom);
  
    // 设置纵轴属性
    axisY = new QtCharts::QValueAxis(m_chart);
    axisY->setRange(0, axisRangeY);
    axisY->setTickCount(10);
    axisY->setTitleText(u8"面积 （cm2)");
    m_chart->addAxis(axisY, Qt::AlignLeft);
   
    axisX->setTitleFont(font);
    axisY->setTitleFont(font);

    axisX->setTitleBrush(QColor(255, 255, 255));
    axisY->setTitleBrush(QColor(255, 255, 255));

    axisX->setLabelsFont(font10);//设置字体样式等，自己修改即可
    axisY->setLabelsFont(font10);//设置字体样式等，自己修改即可

    axisX->setLabelsColor(QColor(255, 255, 255));  //设置x轴的标签颜色，就是显示的文字颜色
    axisY->setLabelsColor(QColor(255, 255, 255));

    m_seriesl->attachAxis(axisX);
    m_seriesl->attachAxis(axisY);
    newSeries->attachAxis(axisX);
    newSeries->attachAxis(axisY);

    chartView = new QtCharts::QChartView(m_chart);
    chartView->setFixedSize(QSize(1900, 600));
    chartView->setRenderHint(QPainter::Antialiasing); // 抗锯齿
    m_chart->setBackgroundVisible(false);  //去背景

    QColor color("#283141");
    QPalette palchart(chartView->palette());
    palchart.setColor(QPalette::Window, color);
    chartView->setAutoFillBackground(true);
    QPen pen(color);
    chartView->setPalette(palchart);
    scrollLayout->addWidget(chartView);

    QGridLayout* gridlayout = new QGridLayout();
    gridlayout->setContentsMargins(10, 10, 10, 10);

    exportBut = new QPushButton(u8"报告预览");
    exportBut->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/button-ground.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{background-image: url(:/antrum/resouce/button-select.png);color:#333; }");
    exportBut->setFixedSize(QSize(180, 50));
    exportBut->setEnabled(false);
    connect(exportBut, &QPushButton::clicked, this, &acquireImage::OnExportButton);

    QLabel* startTime = new QLabel(contentWidget);
    QLabel* endTime = new QLabel(contentWidget);
    QLabel* auTime = new QLabel(contentWidget);
    QLabel* avgDiastole = new QLabel(contentWidget);//舒展
    QLabel* avgContract = new QLabel(contentWidget); //收缩
    QLabel* dcCount = new QLabel(contentWidget);
      
  
    startTime->setText(u8"图像采集时间：");
    startTime->setStyleSheet("QLabel{font:21px;color:white;}");
    endTime->setText(u8"结束时间：");
    endTime->setStyleSheet("QLabel{font:21px;color:white;}");
    auTime->setText(u8"采集时长：");
    auTime->setStyleSheet("QLabel{font:21px;color:white;}");
    avgDiastole->setText(u8"平均舒张面积：");
    avgDiastole->setStyleSheet("QLabel{font:21px;color:white;}");
    avgContract->setText(u8"平均收缩面积：");
    avgContract->setStyleSheet("QLabel{font:21px;color:white;}");
    dcCount->setText(u8"收缩总数：");
    dcCount->setStyleSheet("QLabel{font:21px;color:white;}");
    
    startTimeEdit = new QDateTimeEdit(contentWidget);
    endTimeEdit = new QLineEdit(contentWidget);
    auTimeEdit = new QLineEdit(contentWidget);   
    avgDiastoleEdit = new QLineEdit(contentWidget);//舒展
    avgContractEdit = new QLineEdit(contentWidget); //收缩
    dcCountEdit = new QLineEdit(contentWidget);

    QLabel* auTimeSuffix = new QLabel(auTimeEdit);
    auTimeSuffix->setStyleSheet("color:#616A77;font:16px;");
    auTimeSuffix->setText(u8"秒");
    auTimeSuffix->move(auTimeEdit->x() + (480 - 32), auTimeEdit->y() + 5);

    QLabel* avgDiastoleSuffix = new QLabel(avgDiastoleEdit);
    avgDiastoleSuffix->setStyleSheet("color:#616A77;font:16px;");
    avgDiastoleSuffix->setText("cm2");
    avgDiastoleSuffix->move(avgDiastoleEdit->x() + (480 - 32), avgDiastoleEdit->y() + 5);

    QLabel* avgContractSuffix = new QLabel(avgContractEdit);
    avgContractSuffix->setStyleSheet("color:#616A77;font:16px;");
    avgContractSuffix->setText("cm2");
    avgContractSuffix->move(avgContractEdit->x() + (480 - 32), avgContractEdit->y() + 5);

    QLabel* dcCountSuffix = new QLabel(dcCountEdit);
    dcCountSuffix->setStyleSheet("color:#616A77;font:16px;");
    dcCountSuffix->setText(u8"次");
    dcCountSuffix->move(dcCountEdit->x() + (480 - 32), dcCountEdit->y() + 5);

    startTimeEdit->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    startTimeEdit->setDateTime(QDateTime::currentDateTime());
    connect(startTimeEdit, &QDateTimeEdit::dateTimeChanged,this,&acquireImage::OnTimeChange);

    startTimeEdit->setStyleSheet("QDateTimeEdit{background-color:#354052;color:white; border:none; font:21px; }");

    endTimeEdit->setStyleSheet("QLineEdit{background-color:#354052;color:white; border:none; font:21px; }");
    endTimeEdit->setReadOnly(true);

    auTimeEdit->setStyleSheet("QLineEdit{background-color:#354052;color:white; border:none; font:21px; }");
    auTimeEdit->setReadOnly(true);

    avgDiastoleEdit->setStyleSheet("QLineEdit{background-color:#354052;color:white; border:none; font:21px; }");
    avgDiastoleEdit->setReadOnly(true);

    avgContractEdit->setStyleSheet("QLineEdit{background-color:#354052;color:white; border:none; font:21px; }");
    avgContractEdit->setReadOnly(true);

    dcCountEdit->setStyleSheet("QLineEdit{background-color:#354052;color:white; border:none; font:21px; }");
    dcCountEdit->setReadOnly(true);

    
    gridlayout->addWidget(exportBut,0, 5, Qt::AlignRight);

    gridlayout->addWidget(startTime, 1, 0);
    gridlayout->addWidget(startTimeEdit, 1, 1);

    gridlayout->addWidget(endTime, 1, 2);
    gridlayout->addWidget(endTimeEdit, 1, 3);

    gridlayout->addWidget(auTime, 1, 4);
    gridlayout->addWidget(auTimeEdit, 1, 5);

    gridlayout->addWidget(avgDiastole, 2, 0);
    gridlayout->addWidget(avgDiastoleEdit, 2, 1);

    gridlayout->addWidget(avgContract, 2, 2);
    gridlayout->addWidget(avgContractEdit, 2, 3);

    gridlayout->addWidget(dcCount, 2, 4);
    gridlayout->addWidget(dcCountEdit, 2, 5);

    scrollLayout->addLayout(gridlayout);

    table = new QTableWidget(contentWidget);
    //table->setFixedSize(QSize(1900, 600));
    table->setFixedWidth(1900);
    table->verticalHeader()->setHidden(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    QStringList headerList; headerList << QString::fromLocal8Bit("序号") << QString::fromLocal8Bit(" 周期时间") << QString::fromLocal8Bit("收缩舒张次数") << QString::fromLocal8Bit("收缩面积") << QString::fromLocal8Bit("舒张面积");
    table->setColumnCount(headerList.size());
    table->setHorizontalHeaderLabels(headerList);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //table->setRowCount(1);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setStyleSheet("QTableWidget{background-color:#283141;color:white;border:none} ");
    table->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:#161C28;color:white;font:24px}");

    scrollLayout->addWidget(table);

    mainLayout->addWidget(srollArea);
    o = 0;
    m_pSmooth = new SmoothCurve(this);
    connect(m_pSmooth, &SmoothCurve::SmoothDone, this, &acquireImage::SmoothDone);

    //实时采集图像
    if (isRuntime)
    {
        if (m_pLogic->GetCameraCount() > 1)
        {
            cameraSelect* s = new cameraSelect(this);
            auto index = s->exec();
            m_pLogic->SetCameraIndex(index);
        }
        m_pLogic->SetType(100);
        m_pLogic->start();
    }
}

acquireImage::~acquireImage()
{
    qDebug() << "~acquireImage";
    delete ui;
    m_pLogic->deleteLater();
    m_chart->deleteLater();
}

void acquireImage::DoneImage(float a)
{
    if (o > axisRangeX-10)
    {
        axisRangeX += 50;
        axisX->setRange(0, axisRangeX);
    }
    if (a + 2  > axisRangeY)
    {
        axisRangeY = a + 2;
        axisY->setRange(0, axisRangeY);
    }

    m_seriesl->append(o, a);
    m_pSmooth->VecAppend(QPointF(o,a));

    if (!m_pLogic->GetMat().empty())
    {
        imageLabel->setPixmap(QPixmap::fromImage(QImage((const unsigned char*)(m_pLogic->GetMat().data), m_pLogic->GetMat().cols, m_pLogic->GetMat().rows, m_pLogic->GetMat().step, QImage::Format_RGB888).rgbSwapped()));
    }
    o++;
}

void acquireImage::DoneImageAnalysis(float a)
{
    m_seriesl->append(o, a);
    m_pSmooth->VecAppend(QPointF(o, a));

    if (!m_pLogic->GetMat().empty())
    {
        imageLabel->setPixmap(QPixmap::fromImage(QImage((const unsigned char*)(m_pAnLogic->GetMat().data), m_pAnLogic->GetMat().cols, m_pAnLogic->GetMat().rows, m_pAnLogic->GetMat().step, QImage::Format_RGB888).rgbSwapped()));
    }
    o++;
}

void acquireImage::reAnalysisFinish()
{
    qDebug() << "reAnalysisFinish! ";
    //计算参数填表
    SmoothDone();

    saveData();

    m_pAnLogic->SetIsRun(false);
    startAcquire->setEnabled(false);
    dataCorrect->setEnabled(true);
    restartAcquire->setEnabled(true);
    reAnalysis->setText(u8"重新分析");
    reAnalysis->setEnabled(true);
    backButton->setEnabled(true);
    exportBut->setEnabled(true);
}

void acquireImage::ViewImage(bool b)
{
    if (!b)
    {
        QMessageBox msgBox;

        msgBox.information(this, u8"错误", u8"没有识别到摄像头", QMessageBox::Ok, QMessageBox::Ok);

        msgBox.exec();
        
        return;
    }


    if (!m_pLogic->GetMat().empty())
    {
        imageLabel->setPixmap(QPixmap::fromImage(QImage((const unsigned char*)(m_pLogic->GetMat().data), m_pLogic->GetMat().cols, m_pLogic->GetMat().rows, m_pLogic->GetMat().step, QImage::Format_RGB888).rgbSwapped()));
    }
}

void acquireImage::SmoothDone()
{
    newSeries->clear();

    auto OutVec = m_pSmooth->GetOutVec();
    auto stFindPV = m_pSmooth->GetPV();
    auto InVec = m_pSmooth->GetInVec();

    newSeries->append(OutVec);

    float sumPeak = 0.0f;
    float sumValley = 0.0f;
    
    data.clear();

    if (stFindPV.Pcnt > stFindPV.Vcnt)
    {
        stFindPV.Pcnt--;
    }
    else if (stFindPV.Pcnt < stFindPV.Vcnt)
    {
        stFindPV.Vcnt--;
    }
    /*qDebug() << "stFindPV.Pcnt" << stFindPV.Pcnt;
    qDebug() << "stFindPV.Vcnt" << stFindPV.Vcnt;*/

    if (stFindPV.Pcnt == stFindPV.Vcnt)
    {

        for (int i = 0; i < stFindPV.Pcnt; i++)
        {
            //printf("波峰:(%d,%f,%f)\n", stFindPV.Pos_Peak[i] + 1, OutVec[stFindPV.Pos_Peak[i] + 1].y(), InVec[stFindPV.Pos_Peak[i] + 1].y());
            sumPeak = sumPeak + InVec[stFindPV.Pos_Peak[i] + 1].y();
            sumValley = sumValley + InVec[stFindPV.Pos_Valley[i] + 1].y();
            
            int temp = 0;

            if (stFindPV.Pos_Peak[i] + 1 > stFindPV.Pos_Valley[i] + 1)
            {
                temp = (stFindPV.Pos_Peak[i] + 1) - (stFindPV.Pos_Valley[i] + 1);
            }
            else
            {
                temp = (stFindPV.Pos_Valley[i] + 1) -(stFindPV.Pos_Peak[i] + 1) ;
            }

            acTableData tempData;
            tempData.Number = i+1;
            tempData.time = qAbs((InVec[stFindPV.Pos_Peak[i] + 1].x() - InVec[stFindPV.Pos_Valley[i] + 1].x())/5);
            tempData.count = i+1;
            tempData.diastoleArea = InVec[stFindPV.Pos_Peak[i] + 1].y();
            tempData.contractArea = InVec[stFindPV.Pos_Valley[i] + 1].y();
            data.append(tempData);
        }

        //for (int i = 0; i < stFindPV.Vcnt; i++)
        //{
        //    //printf("波谷:(%d,%f,%f)\n", stFindPV.Pos_Valley[i] + 1, OutVec[stFindPV.Pos_Valley[i] + 1].y(), InVec[stFindPV.Pos_Valley[i] + 1].y());
        //    
        //}

        auto avgPeak = sumPeak / stFindPV.Pcnt;
        auto avgValley = sumValley / stFindPV.Vcnt;
     
        avgDiastoleEdit->setText(QString::number(avgPeak) );
        avgContractEdit->setText(QString::number(avgValley));
        dcCountEdit->setText(QString::number(stFindPV.Pcnt));

        setTableData();
    }
    int sec = o / 5;
    auTimeEdit->setText(QString::number(sec));
}

void acquireImage::OnTimeout()
{
    m_pSmooth->start();
}

void acquireImage::OnEndTimeout()
{
    qDebug() << "OnEndTimeout";
    m_pLogic->SetIsRun(false);
    
    auto dateTime = startTimeEdit->dateTime();
    if (dateTime.isNull())
    {
        endTimeEdit->setText(auTimeEdit->text());

    }
    else
    {
        OnTimeChange(startTimeEdit->dateTime());
    }
   
    timer->stop();
    endTimer->stop();
    saveData();
    SmoothDone();
    startAcquire->setText(u8"采集完成");
    startAcquire->setEnabled(false);
    dataCorrect->setEnabled(true);
    restartAcquire->setEnabled(true);
    reAnalysis->setEnabled(true);
    backButton->setEnabled(true);
    exportBut->setEnabled(true);

    edit->setReadOnly(false);
    timeOk->setEnabled(true);
}

void acquireImage::OnTimeChange(const QDateTime& dateTime)
{
    //采集时长
    auto auTime = auTimeEdit->text();
    if (auTime == "")
        return;
    auto t = dateTime.addSecs(auTime.toInt());
    endTimeEdit->setText(t.toString("yyyy/MM/dd hh:mm:ss"));
}

void acquireImage::setTableData()
{
    table->clearContents();

    table->setRowCount(data.size());
    table->setFixedHeight(data.size()*42 +50);

    for (size_t i = 0; i < data.size(); i++)
    {
        table->setItem(i, 0, new QTableWidgetItem(QString::number(data[i].Number)));
        table->item(i, 0)->setTextAlignment(Qt::AlignCenter);

        table->setItem(i, 1, new QTableWidgetItem(QString::number(data[i].time)));
        table->item(i, 1)->setTextAlignment(Qt::AlignCenter);

        table->setItem(i, 2, new QTableWidgetItem(QString::number(data[i].count)));
        table->item(i, 2)->setTextAlignment(Qt::AlignCenter);

        table->setItem(i, 3, new QTableWidgetItem(QString::number(data[i].contractArea)));
        table->item(i, 3)->setTextAlignment(Qt::AlignCenter);

        table->setItem(i, 4, new QTableWidgetItem(QString::number(data[i].diastoleArea)));
        table->item(i, 4)->setTextAlignment(Qt::AlignCenter);
    }

}

void acquireImage::saveData()
{
    //表格
    writeExcel();

    //图片
    saveImage();
    //sql
    collectionData data;
    data.pid = clID;
    data.startTime = startTimeEdit->text();
    data.endTime = endTimeEdit->text();
    data.cycleTime = auTimeEdit->text();
    data.count = dcCountEdit->text();
    data.diastoleArea = avgDiastoleEdit->text();//收缩
    data.contractArea = avgContractEdit->text();//舒展
    m_pLogic->saveData(baseID.toInt(), data);
}

void acquireImage::writeExcel()
{
    //excel
    excel = new QAxObject();
    if (nullptr == excel)
        qDebug() << "excel is null";
    excel->setControl("Excel.Application");
    if (excel->isNull())
    {
        excel->setControl("KET.Application"); // 若上面加载失败，则尝试加载WPS-Excel
    }
    excel->setProperty("Visible", false);        // 不显示Excel
    excel->setProperty("DisplayAlerts", false);  // 不弹出警告提示窗

    //excel
    QString result = QApplication::applicationDirPath() + "\\resultSet\\";
    auto filename = result + baseID +"\\" + clID + "\\result.xlsx";
   
    auto workbooks = excel->querySubObject("Workbooks");    // 获取工作簿集合
    if (nullptr == workbooks)
    {
        excel->dynamicCall("Quit()");
        return;
    }

    workbooks->dynamicCall("Add()");    // 新增一个工作簿
 
    auto workbook = excel->querySubObject("ActiveWorkBook");    // 获取当前活动工作簿

    auto worksheets = workbook->querySubObject("WorkSheets");       // 获取工作表集合

    auto pSheets = workbook->querySubObject("Sheets");
    auto pSheet = pSheets->querySubObject("Item(int)", 1);

    pSheet->querySubObject("Cells(int,int)", 1, 1)->dynamicCall("Value", u8"序号");
    pSheet->querySubObject("Cells(int,int)", 1, 2)->dynamicCall("Value", u8"周期时间");
    pSheet->querySubObject("Cells(int,int)", 1, 3)->dynamicCall("Value", u8"收缩次数");
    pSheet->querySubObject("Cells(int,int)", 1, 4)->dynamicCall("Value", u8"收缩面积");
    pSheet->querySubObject("Cells(int,int)", 1, 5)->dynamicCall("Value", u8"舒张面积");
   
    int cellRow = 2;
    for (size_t i = 0; i < data.size(); i++)
    {
        pSheet->querySubObject("Cells(int,int)", cellRow, 1)->dynamicCall("Value", data[i].Number);
        pSheet->querySubObject("Cells(int,int)", cellRow, 2)->dynamicCall("Value", data[i].time);
        pSheet->querySubObject("Cells(int,int)", cellRow, 3)->dynamicCall("Value", data[i].count);
        pSheet->querySubObject("Cells(int,int)", cellRow, 4)->dynamicCall("Value", data[i].diastoleArea);
        pSheet->querySubObject("Cells(int,int)", cellRow, 5)->dynamicCall("Value", data[i].contractArea);
        ++cellRow;
    }
    QString AbsolutePath = filename.replace("/", "\\");

    workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(AbsolutePath));

    if (nullptr != excel)
    {
        excel->dynamicCall("Quit()");

        delete excel;
        excel = nullptr;
    }

}

void acquireImage::AddExcelToWord()
{
    QAxObject* word = new QAxObject("Word.Application");
    word->setProperty("Visible", true);

    QAxObject* docs = word->querySubObject("Documents");
    QString filePath = "D:/export.docx";
    QAxObject* doc = docs->querySubObject("Open(const QString&)", filePath);

    // 追加添加Excel工作表
    QAxObject* excel = new QAxObject("Excel.Application");
    excel->setProperty("Visible", false);
    QAxObject* excels = excel->querySubObject("Workbooks");
    QAxObject* workbook = excels->querySubObject("Add");
    QAxObject* worksheet = workbook->querySubObject("Worksheets(int)", 1);

    // 保存并关闭Excel
    workbook->dynamicCall("SaveAs(const QString&)", QString("C:/path/to/your/excel/document.xlsx"));
    workbook->dynamicCall("Close(Boolean)", false);

    // 清理资源
    workbook->setParent(0);
    worksheet->setParent(0);
    excels->setParent(0);
    workbook->setParent(0);

    excel->setParent(0);

    // 清理Word文档对象
    doc->setParent(0);
    docs->setParent(0);
    word->setParent(0);
}

void acquireImage::saveImage()
{
    QString name = QApplication::applicationDirPath() + "/resultSet/" + baseID + "/" + clID;

    //curve
    chartView->setBackgroundBrush(QBrush(QColor("#283141")));
    QImage curveImg(chartView->size(), QImage::Format_ARGB32);
    QPainter curvePainter(&curveImg);
    chartView->render(&curvePainter);
    m_pLogic->saveCurveImage(name, curveImg);

    // 主界面
    auto size = QSize(1920, contentWidget->size().height());    
    QImage img(size, QImage::Format_ARGB32);
    QPainter painter(&img);
    contentWidget->render(&painter);

    //基本信息
    QImage BaseImg(baseTab->size(), QImage::Format_ARGB32);
    QPainter BasePainter(&BaseImg);       
    baseTab->render(&BasePainter);

    //药物
    QImage ClinicalImg(clinicalTab->size(), QImage::Format_ARGB32);
    QPainter ClinicalPainter(&ClinicalImg);
    clinicalTab->render(&ClinicalPainter);
    
    //营养
    if (nutTab->GetVectorSize())
    {
        QImage NutritionImg(nutTab->size(), QImage::Format_ARGB32);
        QPainter NutritionPainter(&NutritionImg);
        nutTab->render(&NutritionPainter);
        m_pLogic->saveImage(name, img, BaseImg, ClinicalImg,NutritionImg);
    }
    else
    {
        m_pLogic->saveImage(name, img, BaseImg, ClinicalImg);
    }
}

void acquireImage::showWaitWidget()
{
    m_watiDialog = new waitDialog(this);
    m_watiDialog->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    m_watiDialog->setFixedSize(QSize(608,225));
    QVBoxLayout* mainLay = new QVBoxLayout(m_watiDialog);
    
    QLabel* move = new QLabel(m_watiDialog);
    move->setFixedSize(QSize(608, 225));
    movi = new QMovie(":/antrum/resouce/matching.gif");
    move->setMovie(movi);
    move->setScaledContents(false);
    move->setAlignment(Qt::AlignCenter);
    move->setStyleSheet("QLabel {background-color:transparent;}");
    movi->start();

    m_watiDialog->setModal(true);
    m_watiDialog->show();
}

void acquireImage::OnStartAcquireButton()
{
    if (m_pLogic->GetIsRun())
    {
        m_pLogic->SetIsRun(false);
        OnTimeChange(startTimeEdit->dateTime());
        timer->stop();
        saveData();
        startAcquire->setText(u8"采集完成");
        startAcquire->setEnabled(false);
        dataCorrect->setEnabled(true);
        restartAcquire->setEnabled(true);
        reAnalysis->setEnabled(true);
        backButton->setEnabled(true);
        exportBut->setEnabled(true);

        if (isRuntime)
        {
            timeOk->setEnabled(true);
            edit->setReadOnly(false);
        }
    }
    else
    {
        m_pLogic->SetIsRun(true);
        
        if (!isRuntime)
            m_pLogic->start();
     
        if (endTime != 0)
        {
            endTimer = new QTimer(this);
            endTimer->start(1000 * 60 * endTime);
            QObject::connect(endTimer, SIGNAL(timeout()), this, SLOT(OnEndTimeout()));
        }


        startAcquire->setText(u8"停止采集");
        dataCorrect->setEnabled(false);
        restartAcquire->setEnabled(false);
        reAnalysis->setEnabled(false);
        backButton->setEnabled(false);
        if (isRuntime)
        {
            timeOk->setEnabled(false);
            edit->setReadOnly(true);
        }
       
        timer = new QTimer(this);
        timer->start(1000* MAXSECOND);//1000ms == 1s
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(OnTimeout()));

        showWaitWidget();
    }
}

void acquireImage::OnDataCorrectButton()
{
    QProcess process;
    
    //QString cmd = QApplication::applicationDirPath()+ "/labelme/labelme.exe";

    QString cmd = "./labelme/labelme.exe";

    QStringList list;
    process.execute(cmd, list);

    //QProcess::execute(cmd);
}

void acquireImage::OnRestartAcquireButton()
{
    //删除数据
    auto path = QApplication::applicationDirPath();
    m_pLogic->removeClinicalData(path,baseID,clID);
    m_pLogic->SetDiskPath(path, baseID.toInt(), clID.toInt());

    m_seriesl->clear();
    newSeries->clear();
    m_pSmooth->ClearVec();
    
    times = 1;
    o = 0;

    m_pLogic->setUnitZore();
    m_pLogic->SetIsRun(true);
    m_pLogic->start();
    timer->start(1000 * MAXSECOND);
    auTimeEdit->clear();
    avgDiastoleEdit->clear();//舒展
    avgContractEdit->clear(); //收缩
    dcCountEdit->clear();
    table->clearContents();
    data.clear();

    startAcquire->setText(u8"停止采集");
    startAcquire->setEnabled(true);
    dataCorrect->setEnabled(false);
    restartAcquire->setEnabled(false);
    reAnalysis->setEnabled(false);
    backButton->setEnabled(false);
    exportBut->setEnabled(false);
    if (isRuntime)
    {
        timeOk->setEnabled(false);
        edit->setReadOnly(true);
    }
  
    if (endTime != 0)
    {
        endTimer = new QTimer(this);
        endTimer->start(1000 *60* endTime);
        QObject::connect(endTimer, SIGNAL(timeout()), this, SLOT(OnEndTimeout()));
    }

    auto s = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}

void acquireImage::OnReAnalysisButton()
{
    //处理界面
    if (m_pAnLogic->GetIsRun())
    {
        m_pAnLogic->SetIsRun(false);
        startAcquire->setEnabled(false);
        dataCorrect->setEnabled(true);
        restartAcquire->setEnabled(true);
        reAnalysis->setText(u8"重新分析");
        reAnalysis->setEnabled(true);
        backButton->setEnabled(true);
        exportBut->setEnabled(true);
    }
    else
    {
        //清除数据
        m_pAnLogic->SetIsRun(true);
        startAcquire->setEnabled(false);
        dataCorrect->setEnabled(true);
        restartAcquire->setEnabled(false);
        reAnalysis->setText(u8"终止分析");
        reAnalysis->setEnabled(true);
        backButton->setEnabled(false);
        exportBut->setEnabled(false);
        
        m_seriesl->clear();
        newSeries->clear();
        m_pSmooth->ClearVec();
        table->clearContents();
        avgDiastoleEdit->clear();//舒展
        avgContractEdit->clear(); //收缩
        dcCountEdit->clear();

        m_pAnLogic->SetUnit(m_pLogic->GetUnit());

        times = 1;
        o = 0;

        m_pAnLogic->start();
    }
}

void acquireImage::OnExportButton()
{
    reportWidget* report = new  reportWidget(baseID, clID);
    int res = report->exec();

    if (res == 10)
    {
        auto folderPath = QApplication::applicationDirPath() + "/resultSet/" + baseID + "/" + clID;
        QUrl url("file:///" + folderPath, QUrl::TolerantMode);

        if (QDesktopServices::openUrl(url)) {
            qDebug() << u8"成功打开文件夹" << folderPath;
        }
        else {
            qDebug() << u8"无法打开文件夹" << folderPath;
        }
    }  
}

void acquireImage::OnTimeButton()
{
    endTime = edit->text().toInt();
    edit->setReadOnly(true);
    timeOk->setEnabled(false);
}

void acquireImage::NotMatch()
{
    qDebug() << "not match";
    movi->stop();
    m_watiDialog->close();
    QImage image;
    if (!m_pLogic->GetMat().empty())
    {
        image =  QImage((const unsigned char*)(m_pLogic->GetMat().data), m_pLogic->GetMat().cols, m_pLogic->GetMat().rows, m_pLogic->GetMat().step, QImage::Format_RGB888).rgbSwapped();
        //imageLabel->setPixmap(QPixmap::fromImage());
    }
  
    computeDistanceDialog* compateNormDialog = new computeDistanceDialog(image,this);
    connect(compateNormDialog, &computeDistanceDialog::computeDistrance,this, &acquireImage::ComputeDistance);
    compateNormDialog->exec();
}

void acquireImage::Match()
{
    qDebug() << "Match";
    movi->stop();
    m_watiDialog->close();
}

void acquireImage::ComputeDistance(QList<QPoint> points)
{
    qDebug() << points;
    m_pLogic->SetUnit(points);
    m_pLogic->resumeThread();
}




