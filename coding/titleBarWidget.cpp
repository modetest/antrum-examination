#include "titleBarWidget.h"
#include "ui_titleBarWidget.h"
#include "UMessageBox.h"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QPushButton>

titleBarWidget::titleBarWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::titleBarWidget)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Window, QColor("#283141"));
    this->setPalette(palette);
    //this->setStyleSheet("#QWidget{background-color:#283141;}");


    QHBoxLayout* horizontalLayout = new QHBoxLayout(this);
    this->layout()->setSpacing(15);
  
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    QLabel* tilitLabel = new QLabel(this);
    tilitLabel->setObjectName(QString::fromUtf8("tilitLabel"));

    horizontalLayout->addWidget(tilitLabel);

    QSpacerItem* horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);

    setPushButton = new QPushButton(this);
    setPushButton->setObjectName(QString::fromUtf8("setPushButton"));
    setPushButton->setMinimumSize(QSize(24, 24));

    horizontalLayout->addWidget(setPushButton);

    QLabel* tilitImageLabel = new QLabel(this);
    tilitImageLabel->setObjectName(QString::fromUtf8("tilitImageLabel"));

    tilitLabel->setPixmap(QPixmap(":/antrum/resouce/mainText.png"));
    tilitImageLabel->setPixmap(QPixmap(":/antrum/resouce/LOGO.png"));

    horizontalLayout->addWidget(tilitImageLabel);

    QLabel* setLabelImage = new QLabel();
    QLabel* setLabelText = new QLabel();
    setLabelImage->setObjectName("setLabelImage");
    setLabelText->setObjectName("setLabelText");
    setLabelImage->setStyleSheet("QLabel{background-color:transparent}");
    setLabelText->setStyleSheet("QLabel{background-color:transparent;font:21px;color:white;}");

    setLabelImage->setPixmap(QPixmap(":/antrum/resouce/exit_icon@2x.png"));
    //设置描述按钮功能的文字
    setLabelText->setText(u8"退出");
    //设置文字的字样、字体、颜色等样式及文本框宽度
    QHBoxLayout* LayoutSettingLayout = new QHBoxLayout();
    LayoutSettingLayout->addWidget(setLabelImage);
    LayoutSettingLayout->addWidget(setLabelText);
    setPushButton->setFixedSize(QSize(80, 43));
    setPushButton->setLayout(LayoutSettingLayout);

    setPushButton->setStyleSheet("QPushButton{border:none;border-radius:4px;}QPushButton:hover {background-color: #555;}QPushButton:pressed{background-color:#333;}");

    QObject::connect(setPushButton, SIGNAL(clicked()), this, SLOT(OnSetPushButton()));
}

titleBarWidget::~titleBarWidget()
{
    delete ui;
}


void titleBarWidget::OnSetPushButton()
{

    UMessageBox msgBox(u8"是否关闭程序？",this);
    auto res = msgBox.exec();
    if (res == QDialog::Accepted)
    {
        exit(0);
    }
}
