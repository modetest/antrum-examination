#include "UMessageBox.h"

UMessageBox::UMessageBox(QString text, QWidget* parent) : QDialog(parent)
{
    this->setObjectName("magBox");
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setFixedSize(QSize(350, 200));

    this->setStyleSheet("#magBox{background-color:#162540;}");



    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QLabel* textLabel = new QLabel(this);
    textLabel->setStyleSheet("QLabel{color:white;font:32px}");
    textLabel->setText(text);
    mainLayout->addWidget(textLabel);
    mainLayout->setContentsMargins(50, 50, 50, 50);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* appBtn = new QPushButton(this);
    QPushButton* cancelBtn = new QPushButton(this);
  
    appBtn->setText(u8"确定");
    cancelBtn->setText(u8"取消");
    appBtn->setFixedSize(QSize(110, 40));
    cancelBtn->setFixedSize(QSize(110, 40));

    appBtn->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/message_box_btn2.png);color:white;border:none;font:21px;} QPushButton:hover {background-image: url(:/antrum/resouce/message_box_btn.png);color: #555;} QPushButton:pressed{background-image: url(:/antrum/resouce/message_box_btn.png);color:#333;}");
    cancelBtn->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/message_box_btn2.png);color:white;border:none;font:21px;} QPushButton:hover {background-image: url(:/antrum/resouce/message_box_btn.png);color: #555;} QPushButton:pressed{background-image: url(:/antrum/resouce/message_box_btn.png);color:#333;}");

    btnLayout->addWidget(appBtn);
    btnLayout->addWidget(cancelBtn);

    mainLayout->addLayout(btnLayout);
    connect(appBtn, &QPushButton::clicked, this, [&]() {
        accept();
        });

    connect(cancelBtn, &QPushButton::clicked, this, [&]() {
        reject();
        });
}

UMessageBox::~UMessageBox()
{
}
