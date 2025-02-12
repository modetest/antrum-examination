#include "cameraSelect.h"

cameraSelect::cameraSelect(QWidget *parent)
	: QDialog(parent)
{
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setStyleSheet("QDialog{background-color:#283141}");

    QVBoxLayout* mainLay = new QVBoxLayout(this);
    mainLay->setSpacing(50);
    QVBoxLayout* ImageLay = new QVBoxLayout();
    ImageLay->setContentsMargins(0, 0, 0, 0);
    ImageLay->setSpacing(10);

    QVBoxLayout* btnLay = new QVBoxLayout();

    auto cameras = QCameraInfo::availableCameras();

    QLabel* textLabel = new QLabel(this);
    textLabel->setStyleSheet("QLabel{ color:white; font:30px }");
    textLabel->setText(u8"获取到多个视频信号请选择使用");

    m_group = new QButtonGroup(this);
    for (size_t i = 0; i < cameras.size(); i++)
    {
        QRadioButton* raBtn = new QRadioButton(this);
        raBtn->setText(cameras[i].description());
        raBtn->setStyleSheet("QRadioButton{color:white;font:21px;}");
        m_group->addButton(raBtn,i);
        ImageLay->addWidget(raBtn);
    }

    QPushButton* selectOk = new QPushButton(this);
    selectOk->setText(u8"确定");
    selectOk->setFixedSize(62, 26);
    selectOk->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/time_ok_1.png); color:white; border:none;font:21px }QPushButton:hover{ color: #555; }QPushButton:pressed{background-image: url(:/antrum/resouce/time_ok_2.png);color:#333; }");

    connect(selectOk, &QPushButton::clicked, this, [&]
        {
            int index = m_group->checkedId();
            if (index == -1)
                index = 0;
            this->done(index);
            this->close();
        });

    btnLay->addWidget(selectOk);

    mainLay->addWidget(textLabel);
    mainLay->addLayout(ImageLay);
    mainLay->addLayout(btnLay);
}

cameraSelect::~cameraSelect()
{}

void cameraSelect::OnOKBtn()
{

}