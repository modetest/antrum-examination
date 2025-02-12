#include "newpatient.h"
#include "ui_newpatient.h"



newPatient::newPatient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newPatient)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window |Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);

    m_Logic = new newPatientLogic();

    //背景色
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Window, QBrush(QPixmap(":/autrum/resouce/mainImage.png")));
    this->setPalette(pal);

    auto mainLayout = this->layout();
    mainLayout->setContentsMargins(10, 10, 10, 10);
    //返回按钮
    QPushButton* backButton = new QPushButton(this);
    backButton->setFixedSize(QSize(110, 40));
    backButton->setText(u8"返回");
    backButton->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/button-back.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333; }");
    mainLayout->addWidget(backButton);
    connect(backButton, &QPushButton::clicked, this, [&]() {
        this->close();
        });

    srollArea = new QScrollArea();
    srollArea->setWidgetResizable(true);
    srollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    srollArea->setStyleSheet("QScrollArea{border:none; background-color:transparent; } QScrollBar{background-color:#323E55;;border:none } QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background:#171f2b;}"
    );


    QWidget* contentWidget = new QWidget(this);
    contentWidget->setObjectName("contentWidget");
    contentWidget->setStyleSheet("#contentWidget{background-color:#171f2b;border:none}");

    //srollArea->setStyleSheet("#contentWidget{background-color:#171f2b;border:none} QScrollArea{background-color:#171f2b;border:none} QScrollBar{background-color:#323E55;;border:none } QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background:#171f2b;}");

    srollArea->setWidget(contentWidget);

    QVBoxLayout* scrollLayout = new QVBoxLayout(contentWidget);
 
    es = new EssentialInformation(contentWidget);

    scrollLayout->addWidget(es);

    cl = new ClinicalInformation(contentWidget);

    scrollLayout->addWidget(cl);

    nu = new NutritionalInformation(contentWidget);

    scrollLayout->addWidget(nu);

    QHBoxLayout* btnLayout = new QHBoxLayout();

    //按钮
    QPushButton* newAqButton = new QPushButton(this);
    newAqButton->setFixedSize(QSize(110, 40));
    newAqButton->setText(u8"实时采集");
    newAqButton->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/button-back.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333; }");
    connect(newAqButton, SIGNAL(clicked()), this, SLOT(onNewButtonClicked()));


    QPushButton* importImageButton = new QPushButton(this);
    importImageButton->setFixedSize(QSize(110, 40));
    importImageButton->setText(u8"数据导入");
    importImageButton->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/button-back.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333; }");
    connect(importImageButton, SIGNAL(clicked()), this, SLOT(onImportButtonClicked()));


    btnLayout->addWidget(newAqButton);
    btnLayout->addWidget(importImageButton);

    scrollLayout->addLayout(btnLayout);
 
    mainLayout->addWidget(srollArea);

}

newPatient::~newPatient()
{
    delete ui;
}

void newPatient::onNewButtonClicked()
{
    int baseID = 0;
    int clID = 0;
    QString s = "100";
    if (getPageData(baseID, clID))
    {
        if (baseID != 0)
        {
            emit Done(baseID, clID, s);
            this->close();
        }
    }
    else
    {
        QMessageBox msgBox;//1.构造QMessageBox的实例

        msgBox.information(this, u8"错误", u8"添加数据库错误！", QMessageBox::Ok, QMessageBox::Ok);

        msgBox.exec();//3.调用exec（）来显示消息。
    }
}

void newPatient::onImportButtonClicked()
{

    int baseID = 0;
    int clID = 0;

    QString s = QFileDialog::getOpenFileName(this, u8"选择导入的文件", "/", "Vedio files(*.mp4 *.avi)");

    if (s == "")
        return;

    if (getPageData(baseID, clID))
    {
        qDebug() << baseID;
        if (baseID != 0)
        {
            emit Done(baseID, clID, s);
            this->close();
        } 
    }
    else
    {
        QMessageBox msgBox;//1.构造QMessageBox的实例

        msgBox.information(this, u8"错误", u8"添加数据库错误！", QMessageBox::Ok, QMessageBox::Ok);
       
        msgBox.exec();//3.调用exec（）来显示消息。
    }
    
}

bool newPatient::getPageData(int& id, int& clid)
{
    baseInformation baseInfo;
    id = es->GetBaseID();
    if (es != 0)
    {
        es->getData(baseInfo);
    }
  
    clinicalInformation clinicalInfo;
    cl->GetData(clinicalInfo);
    
    QVector<nutritionalInformation> vecInfo;
    nu->GetData(vecInfo);

    //写数据
    auto success = m_Logic->newPatientRecord(id, clid,baseInfo, clinicalInfo, vecInfo);

    //cl->SaveRhythmHeart();

    return true;
}
