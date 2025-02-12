#include "baseinfopage.h"
#include "EssentialInformation.h"
#include "ui_baseinfopage.h"

BaseInfoPage::BaseInfoPage(int id, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::BaseInfoPage)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    BaseID = id;
    clID = 0;
    m_logic = new BaseInfoLogic(this);

    //����ɫ
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Window, QBrush(QPixmap(":/antrum/resouce/mainImage.png")));
    this->setPalette(pal);

    auto mainLayout = this->layout();

    //���ذ�ť
    QPushButton* backButton = new QPushButton(this);
    backButton->setFixedSize(QSize(110, 40));
    backButton->setText(u8"����");
    backButton->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/button-back.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333; }");
    mainLayout->addWidget(backButton);
    connect(backButton, &QPushButton::clicked, this, [=]() {
        this->close();
        });

    QScrollArea* srollArea = new QScrollArea();
    srollArea->setWidgetResizable(true);
    srollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QWidget* contentWidget = new QWidget();
    contentWidget->setObjectName("contentWidget");
    srollArea->setStyleSheet("#contentWidget{background-color:#171f2b;border:none}QScrollArea{background-color:#171f2b;border:none} QScrollBar{background-color:#323E55;;border:none } QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background:#171f2b;}");

    srollArea->setWidget(contentWidget);

    QVBoxLayout* scrollLayout = new QVBoxLayout(contentWidget);
        
    EssentialInformation* es = new EssentialInformation(contentWidget);

    es->SetData(BaseID);

    scrollLayout->addWidget(es);

    //��ť
    newClAqButton = new QPushButton(this);
    newClAqButton->setFixedSize(QSize(110, 40));
    newClAqButton->setText(u8"�����ɼ�");
    newClAqButton->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/button-back.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333; }");
    mainLayout->addWidget(newClAqButton);
    connect(newClAqButton, &QPushButton::clicked, this ,&BaseInfoPage::OnNewAqButton);
    scrollLayout->addWidget(newClAqButton);


    table = new QTableWidget(srollArea);
    table->setFixedWidth(1900);
    table->verticalHeader()->setHidden(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    QStringList headerList;
    headerList <<QString::fromLocal8Bit("���") << QString::fromLocal8Bit("�ɼ����") << QString::fromLocal8Bit(" �ɼ�ʱ��") << QString::fromLocal8Bit("��ʱ��") << QString::fromLocal8Bit("θ���������") << QString::fromLocal8Bit("ƽ���������") << QString::fromLocal8Bit("ƽ���������") << QString::fromLocal8Bit("����");
    table->setColumnCount(headerList.size());
    table->setHorizontalHeaderLabels(headerList);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setStyleSheet("QTableWidget{background-color:#283141;color:white;border:none} ");
    table->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:#161C28;color:white;font:24px}");
    scrollLayout->addWidget(table);

    cl = new ClinicalInformation(contentWidget);
    cl->setVisible(false);
    scrollLayout->addWidget(cl);

    nu = new NutritionalInformation(contentWidget);
    nu->setVisible(false);
    scrollLayout->addWidget(nu);

    QHBoxLayout* btnLayout = new QHBoxLayout();

    newAqButton = new QPushButton(contentWidget);
    newAqButton->setVisible(false);
    newAqButton->setFixedSize(QSize(110, 40));
    newAqButton->setText(u8"ʵʱ�ɼ�");
    newAqButton->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/button-back.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333; }");
    connect(newAqButton, SIGNAL(clicked()), this, SLOT(onNewButtonClicked()));


    importImageButton = new QPushButton(contentWidget);
    importImageButton->setVisible(false);
    importImageButton->setFixedSize(QSize(110, 40));
    importImageButton->setText(u8"���ݵ���");
    importImageButton->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/button-back.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{color:#333; }");
    connect(importImageButton, SIGNAL(clicked()), this, SLOT(onImportButtonClicked()));

    btnLayout->addWidget(newAqButton);
    btnLayout->addWidget(importImageButton);

    scrollLayout->addLayout(btnLayout);

    mainLayout->addWidget(srollArea);

    RefreshTableData();
}

BaseInfoPage::~BaseInfoPage()
{
    delete ui;
}

void BaseInfoPage::RefreshTableData()
{
    //��ѯ���� ͳ�� ����ҳ��
    QVector<collectionData> vec;
    m_logic->QueryClinicalData(BaseID,vec);

    int totalCount = vec.size();

    table->clearContents();
    table->setRowCount(totalCount);
    table->setFixedHeight(42 * totalCount +50);

    m_TableItemArray = new tableItemWidget * [totalCount];

    for (size_t i = 0; i < totalCount; i++)
    {
        m_TableItemArray[i] = new tableItemWidget(i);

        table->setCellWidget(i, 7, m_TableItemArray[i]);

        table->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        table->item(i, 0)->setTextAlignment(Qt::AlignCenter);

        table->setItem(i, 1, new QTableWidgetItem(vec[i].pid));
        table->item(i, 1)->setTextAlignment(Qt::AlignCenter);

        table->setItem(i, 2, new QTableWidgetItem(vec[i].acTime));
        table->item(i, 2)->setTextAlignment(Qt::AlignCenter);

        table->setItem(i, 3, new QTableWidgetItem(vec[i].cycleTime));
        table->item(i, 3)->setTextAlignment(Qt::AlignCenter);

        table->setItem(i, 4, new QTableWidgetItem(vec[i].count));
        table->item(i, 4)->setTextAlignment(Qt::AlignCenter);

        table->setItem(i, 5, new QTableWidgetItem(vec[i].diastoleArea));
        table->item(i, 5)->setTextAlignment(Qt::AlignCenter);

        table->setItem(i, 6, new QTableWidgetItem(vec[i].contractArea));
        table->item(i, 6)->setTextAlignment(Qt::AlignCenter);

        QObject::connect(m_TableItemArray[i]->btnInfo, SIGNAL(clicked()), this, SLOT(OnInfoPushButton()));
        QObject::connect(m_TableItemArray[i]->btnDel, SIGNAL(clicked()), this, SLOT(OnDelPushButton()));
    }
}


void BaseInfoPage::onNewButtonClicked()
{
    QString s = "100";
    if (GetPageData())
    {
        if (BaseID != 0)
        {
            emit Done(BaseID, clID, s);
            this->close();
        }
    }
    else
    {
        QMessageBox msgBox;//1.����QMessageBox��ʵ��

        msgBox.information(this, u8"����", u8"������ݿ����", QMessageBox::Ok, QMessageBox::Ok);

        msgBox.exec();//3.����exec��������ʾ��Ϣ��
    }
}

void BaseInfoPage::OnInfoPushButton()
{
    QPushButton* button = dynamic_cast<QPushButton*>(QObject::sender()); //�ҵ��źŷ�����
    tableItemWidget* pTableWidget = dynamic_cast<tableItemWidget*>(button->parent()); //getIndex();
    QTableWidgetItem* item = table->item(pTableWidget->getIndex(), 1);

    auto folderPath = QApplication::applicationDirPath() + "/resultSet/" + QString::number(BaseID) + "/" + item->text();
    QUrl url("file:///" + folderPath, QUrl::TolerantMode);

    if (QDesktopServices::openUrl(url)) {
        qDebug() << u8"�ɹ����ļ���" << folderPath;
    }
    else {
        qDebug() << u8"�޷����ļ���" << folderPath;
    }

}

void BaseInfoPage::OnDelPushButton()
{
    UMessageBox msgBox(u8"�Ƿ�ɾ�����ݣ�", table);
    auto res = msgBox.exec();
    if (res == QDialog::Accepted)
    {
        QPushButton* button = dynamic_cast<QPushButton*>(QObject::sender()); //�ҵ��źŷ�����
        tableItemWidget* pTableWidget = dynamic_cast<tableItemWidget*>(button->parent()); //getIndex();
        QTableWidgetItem* item = table->item(pTableWidget->getIndex(), 1);
        auto rootPath = QApplication::applicationDirPath();
        m_logic ->DeletePatientData(rootPath,BaseID,item->text().toInt());
        RefreshTableData();
    }
}

void BaseInfoPage::onImportButtonClicked()
{
    QString s = QFileDialog::getOpenFileName(this, u8"ѡ������ļ�", "/", "Vedio files(*.mp4 *.avi)");

    if (s == "")
        return;

    if (GetPageData())
    {
        if (BaseID != 0)
        {
            emit Done(BaseID, clID, s);
            this->close();
        }
    }
    else
    {
        QMessageBox msgBox;//1.����QMessageBox��ʵ��

        msgBox.information(this, u8"����", u8"������ݿ����", QMessageBox::Ok, QMessageBox::Ok);

        msgBox.exec();//3.����exec��������ʾ��Ϣ��
    }
}

bool BaseInfoPage::GetPageData()
{

    clinicalInformation clinicalInfo;
    cl->GetData(clinicalInfo);

    QVector<nutritionalInformation> vecInfo;
    nu->GetData(vecInfo);

    //д����
    auto success = m_logic->newClinicalRecord(BaseID, clID,clinicalInfo, vecInfo);

    //cl->SaveRhythmHeart();

    return success;
    return true;
}

void BaseInfoPage::OnNewAqButton()
{
    newClAqButton->setVisible(false);
    table->setVisible(false);

    cl->setVisible(true);
    nu->setVisible(true);

    newAqButton->setVisible(true);
    importImageButton->setVisible(true);
}