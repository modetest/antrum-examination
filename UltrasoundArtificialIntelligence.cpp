#include "UltrasoundArtificialIntelligence.h"
#include "UMessageBox.h"


UltrasoundArtificialIntelligence::UltrasoundArtificialIntelligence(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->setStyleSheet("UltrasoundArtificialIntelligence{background-image: url(:/antrum/resouce/mainImage.png);}");

    this->setWindowTitle(u8"主页");
    QIcon icon(":/antrum/resouce/mainIcon.png");
    this->setWindowIcon(icon);
    

    m_Logic = new UltrasoundArtficialIntellgenceLogic(QApplication::applicationDirPath(),this);


    titleBarWidget* title = new titleBarWidget(this);
    ui.titleLayout->addWidget(title);

    ui.sreachWidget->setStyleSheet("QWidget{background-color:#303A4C};");
    ui.newPatientPushButton->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/new_patient.png);color:white;border:none;}QPushButton:hover {color: #555;}QPushButton:pressed{color:#333;}");
    ui.sreachLabel->setPixmap(QPixmap(":/antrum/resouce/sreach_image.png"));
    ui.sreachLineEdit->setStyleSheet("QLineEdit{background-color:transparent;color:white; border:none;}");
    ui.sreachPushButton->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/sreachBtn.png);color:white;border:none;}QPushButton:hover {color: #555;}QPushButton:pressed{color:#333;}");

    ui.mainTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.mainTableWidget->setGeometry(12, 157, 1896, 851);
    ui.mainTableWidget->verticalHeader()->setHidden(true);
    ui.mainTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.mainTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui.mainTableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    QStringList headerList; 
    headerList << QString::fromLocal8Bit("序号") << QString::fromLocal8Bit("编号") << QString::fromLocal8Bit("病人姓名") << QString::fromLocal8Bit("性别") << QString::fromLocal8Bit("年龄") << QString::fromLocal8Bit("所在医院") << QString::fromLocal8Bit("采集数据") << QString::fromLocal8Bit("操作");


    ui.mainTableWidget->setColumnCount(headerList.size());
    ui.mainTableWidget->setHorizontalHeaderLabels(headerList);
    ui.mainTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui.mainTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.mainTableWidget->setRowCount(10);
    ui.mainTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.mainTableWidget->setStyleSheet("QTableWidget{background-color:#283141;color:white;border:none} QTableWidget::item{text-align: center;} ");
    ui.mainTableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:#161C28;color:white;font:24px}");
    

    int pageIndex = 1;

    RefreshTableData(pageIndex);
   
    InitPageButton();  

    QObject::connect(ui.sreachPushButton, SIGNAL(clicked()), this, SLOT(OnSearchButton()));
    QObject::connect(ui.newPatientPushButton, SIGNAL(clicked()), this, SLOT(OnNewPatientButton()));

}

UltrasoundArtificialIntelligence::~UltrasoundArtificialIntelligence()
{
    qDebug()<< "~UltrasoundArtificialIntelligence";
}

void UltrasoundArtificialIntelligence::InitPageButton()
{
    int totalPage = 100;

    //翻页
    auto pageLayout = ui.pageWidget->layout();

    QSpacerItem* spacer = new QSpacerItem(1482, 80, QSizePolicy::Expanding, QSizePolicy::Maximum);

    pageLayout->addItem(spacer);

    QPushButton* previousButton = new QPushButton();
    QPushButton* nextButton = new QPushButton();

    previousButton->setFixedSize(QSize(56, 28));
    nextButton->setFixedSize(QSize(56, 28));

    previousButton->setStyleSheet("QPushButton{background-color:#303A4D;color:white;border:none;font:20px;}QPushButton:hover {color: #555;}QPushButton:pressed{color:#333;}");
    nextButton->setStyleSheet("QPushButton{background-color:#303A4D;color:white;border:none;font:20px;}QPushButton:hover {color: #555;}QPushButton:pressed{color:#333;}");
    previousButton->setText("<");
    nextButton->setText(">");

    pageLayout->addWidget(previousButton);

    QLabel* labelDi = new QLabel();
    QLabel* labelYe = new QLabel();
    labelDi->setText(u8"第");
    labelYe->setText(u8"页 ");
    labelDi->setStyleSheet("QLabel{background-color:#0D0E18; color:white; border:none; font:28px;}");
    labelYe->setStyleSheet("QLabel{background-color:#0D0E18; color:white; border:none; font:28px;}");

    pageLayout->addWidget(labelDi);
    curIndexLabel = new QLabel();
    curIndexLabel->setText(u8"1");
    curIndexLabel->setStyleSheet("QLabel{background-color:#0D0E18; color:white; border:none; font:28px;}");
    pageLayout->setSpacing(20);
    pageLayout->addWidget(curIndexLabel);
    pageLayout->setSpacing(20);
    pageLayout->addWidget(labelYe);

    pageLayout->addWidget(nextButton);


    QObject::connect(previousButton, SIGNAL(clicked()), this, SLOT(OnPreviousButton()));
    QObject::connect(nextButton, SIGNAL(clicked()), this, SLOT(OnNextButton()));
}

void UltrasoundArtificialIntelligence::OnSearchButton()
{
    auto name =  ui.sreachLineEdit->text();
    QVector<tableData> vec;
    if (name != "")
    {
        m_Logic->SreachData(&vec,name);

        int totalCount = vec.size();

        ui.mainTableWidget->clearContents();

        if (totalCount == 0)
        { 
           return ;         
        }

        m_TableItemArray = new tableItemWidget * [totalCount];

        for (size_t i = 0; i < totalCount; i++)
        {
            m_TableItemArray[i] = new tableItemWidget(i);

            ui.mainTableWidget->setCellWidget(i, 7, m_TableItemArray[i]);

            QTableWidgetItem* item = new QTableWidgetItem(QString::number(vec[i].id));
            
            item->setTextAlignment(Qt::AlignCenter);

            ui.mainTableWidget->setItem(i, 0, item);

            ui.mainTableWidget->setItem(i, 1, new QTableWidgetItem(vec[i].serialNumber));
            ui.mainTableWidget->item(i, 1)->setTextAlignment(Qt::AlignCenter);

            ui.mainTableWidget->setItem(i, 2, new QTableWidgetItem(vec[i].name));
            ui.mainTableWidget->item(i, 2)->setTextAlignment(Qt::AlignCenter);

            ui.mainTableWidget->setItem(i, 3, new QTableWidgetItem(vec[i].sex));
            ui.mainTableWidget->item(i, 3)->setTextAlignment(Qt::AlignCenter);

            ui.mainTableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(vec[i].age)));
            ui.mainTableWidget->item(i, 4)->setTextAlignment(Qt::AlignCenter);

            ui.mainTableWidget->setItem(i, 5, new QTableWidgetItem(vec[i].hospital));
            ui.mainTableWidget->item(i, 5)->setTextAlignment(Qt::AlignCenter);

            ui.mainTableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(vec[i].collectCount)));
            ui.mainTableWidget->item(i, 6)->setTextAlignment(Qt::AlignCenter);


            QObject::connect(m_TableItemArray[i]->btnInfo, SIGNAL(clicked()), this, SLOT(OnInfoPushButton()));
            QObject::connect(m_TableItemArray[i]->btnDel, SIGNAL(clicked()), this, SLOT(OnDelPushButton()));
        }

    }
    else
    {
        RefreshTableData(1);
    }
}

int UltrasoundArtificialIntelligence::RefreshTableData(int pageIndex)
{
    //查询数据 统计 计算页数
    QVector<tableData> vec;
    m_Logic->QueryPatientData(&vec, pageIndex);

    int totalCount = vec.size();

    if (totalCount == 0)
    {
        if (pageIndex == 1)
            ui.mainTableWidget->clearContents();
        return 0;
       /* if (pageIndex > 1)
        {
            pageIndex -= 1;
            m_Logic->QueryPatientData(&vec, pageIndex);
            totalCount = vec.size();
        }
        else
        {
            return 0;
        }*/
    }

    ui.mainTableWidget->clearContents();

    m_TableItemArray = new tableItemWidget * [totalCount];

    for (size_t i = 0; i < totalCount; i++)
    {
        m_TableItemArray[i] = new tableItemWidget(i);

        ui.mainTableWidget->setCellWidget(i, 7, m_TableItemArray[i]);

        QTableWidgetItem* item = new QTableWidgetItem(QString::number(vec[i].id));

        item->setTextAlignment(Qt::AlignCenter);

        ui.mainTableWidget->setItem(i, 0, item);

        ui.mainTableWidget->setItem(i, 1, new QTableWidgetItem(vec[i].serialNumber));
        ui.mainTableWidget->item(i,1)->setTextAlignment(Qt::AlignCenter);

        ui.mainTableWidget->setItem(i, 2, new QTableWidgetItem(vec[i].name));
        ui.mainTableWidget->item(i, 2)->setTextAlignment(Qt::AlignCenter);


        ui.mainTableWidget->setItem(i, 3, new QTableWidgetItem(vec[i].sex));
        ui.mainTableWidget->item(i, 3)->setTextAlignment(Qt::AlignCenter);

        ui.mainTableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(vec[i].age)));
        ui.mainTableWidget->item(i, 4)->setTextAlignment(Qt::AlignCenter);


        ui.mainTableWidget->setItem(i, 5, new QTableWidgetItem(vec[i].hospital));
        ui.mainTableWidget->item(i, 5)->setTextAlignment(Qt::AlignCenter);


        ui.mainTableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(vec[i].collectCount)));
        ui.mainTableWidget->item(i, 6)->setTextAlignment(Qt::AlignCenter);

        QObject::connect(m_TableItemArray[i]->btnInfo, SIGNAL(clicked()), this, SLOT(OnInfoPushButton()));
        QObject::connect(m_TableItemArray[i]->btnDel, SIGNAL(clicked()), this, SLOT(OnDelPushButton()));
    }
    return totalCount;
}

void UltrasoundArtificialIntelligence::OnNewPatientButton()
{
    newPatient* newPatientObject = new  newPatient(this);
    QObject::connect(newPatientObject, SIGNAL(Done(int,int,QString&)), this, SLOT(OnNewPatientDone(int, int, QString&)));
    newPatientObject->exec();
}

void UltrasoundArtificialIntelligence::OnInfoPushButton()
{
   QPushButton* button = dynamic_cast<QPushButton*>(QObject::sender()); //找到信号发送者
   tableItemWidget* pTableWidget = dynamic_cast<tableItemWidget*>(button->parent()); //getIndex();
   QTableWidgetItem* item = ui.mainTableWidget->item(pTableWidget->getIndex(), 0);
   
   BaseInfoPage* m_pBaseInfoPage = new BaseInfoPage(item->text().toInt(),this);
   QObject::connect(m_pBaseInfoPage, SIGNAL(Done(int, int, QString&)), this, SLOT(OnNewPatientDone(int, int, QString&)));
   m_pBaseInfoPage->exec();
   RefreshTableData(1);
}

void UltrasoundArtificialIntelligence::OnDelPushButton()
{
    UMessageBox msgBox(u8"是否删除数据？", ui.mainTableWidget);
    auto res = msgBox.exec();
    if (res == QDialog::Accepted)
    {
        QPushButton* button = dynamic_cast<QPushButton*>(QObject::sender()); //找到信号发送者
        tableItemWidget* pTableWidget = dynamic_cast<tableItemWidget*>(button->parent()); //getIndex();
        QTableWidgetItem* item = ui.mainTableWidget->item(pTableWidget->getIndex(), 0);
        m_Logic->DeletePatientData(item->text().toInt());
        auto pageIndex = curIndexLabel->text().toInt();
        RefreshTableData(pageIndex);
    }
}

void UltrasoundArtificialIntelligence::OnPreviousButton()
{
    auto curIndex = curIndexLabel->text().toInt();
    if (curIndex == 1)
        return;
    int index =  curIndex - 1;
    RefreshTableData(index);
    curIndexLabel->setText(QString::number(index));
}

void UltrasoundArtificialIntelligence::OnNextButton()
{
    auto curIndex = curIndexLabel->text().toInt();
    int index = curIndex + 1;
    auto size = RefreshTableData(index);

    if (curIndex+1 == index && size!=0)
    {
        curIndexLabel->setText(QString::number(index));
    }
}

void UltrasoundArtificialIntelligence::OnNewPatientDone(int id, int clid, QString& path)
{
    int pageIdex = 1;
    RefreshTableData(pageIdex);
    acquireImage* newAcquireImage = new  acquireImage(m_Logic->GetModelNet(), id, clid, path, this);
    newAcquireImage->show();
}

