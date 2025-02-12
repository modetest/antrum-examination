#include "tableItemWidget.h"

tableItemWidget::tableItemWidget(int index, QWidget* parent):QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    //this->setAttribute(Qt::WA_TranslucentBackground);
    //this->setWindowOpacity(1);
    this->setStyleSheet("QWidget{background-color:#161C28;border-radius:4px;}");

	QHBoxLayout* layout = new QHBoxLayout();
	this->setLayout(layout);

    QLabel* infoLabelImage = new QLabel(this);
    QLabel* infoLabelText = new QLabel(this);
    infoLabelImage->setAttribute(Qt::WA_TranslucentBackground);
    infoLabelText->setAttribute(Qt::WA_TranslucentBackground);
    infoLabelImage->setPixmap(QPixmap(":/antrum/resouce/tableInfobtn.png"));
    infoLabelImage->setFixedWidth(12);
    //设置描述按钮功能的文字
    infoLabelText->setText(u8"详情");
    //设置文字的字样、字体、颜色等样式及文本框宽度
    infoLabelText->setStyleSheet("font:16px;color:white;");
    QHBoxLayout* LayoutInfoLayout = new QHBoxLayout();
    LayoutInfoLayout->addWidget(infoLabelImage);
    LayoutInfoLayout->addWidget(infoLabelText);

    QLabel* delLabelImage = new QLabel(this);
    QLabel* delLabelText = new QLabel(this);
    delLabelImage->setAttribute(Qt::WA_TranslucentBackground);
    delLabelText->setAttribute(Qt::WA_TranslucentBackground);
    delLabelImage->setPixmap(QPixmap(":/antrum/resouce/tableDelbtn.png"));
    delLabelImage->setFixedWidth(12);
    //设置描述按钮功能的文字
    delLabelText->setText(u8"删除");
    //设置文字的字样、字体、颜色等样式及文本框宽度
    delLabelText->setStyleSheet("font:16px;color:red;");
    QHBoxLayout* LayoutDelLayout = new QHBoxLayout();
    LayoutDelLayout->addWidget(delLabelImage);
    LayoutDelLayout->addWidget(delLabelText);
    btnInfo = new QPushButton(this);
    btnInfo->setLayout(LayoutInfoLayout);
    btnInfo->setFixedSize(QSize(70,36));
    btnDel = new QPushButton(this);
    btnDel->setLayout(LayoutDelLayout);
    btnDel->setFixedSize(QSize(70, 36));

    btnInfo->setStyleSheet("QPushButton{color:white;border:none;}QPushButton:hover {background-color: #555;}QPushButton:pressed{background-color:#333;}");
    btnDel->setStyleSheet("QPushButton{color:white;border:none;}QPushButton:hover {background-color: #555;}QPushButton:pressed{background-color:#333;}");

    layout->addWidget(btnInfo);
    QFrame* spacer = new QFrame(this);
    spacer->setFrameShape(QFrame::HLine);
    spacer->setFixedWidth(1);
    spacer->setFixedHeight(36);
    spacer->setStyleSheet("QFrame{background-color:#3B4A65}");
    layout->addWidget(spacer);
    layout->addWidget(btnDel);

    m_index = index;

}

tableItemWidget::~tableItemWidget()
{
    btnDel->deleteLater();
    btnInfo->deleteLater();
}

int tableItemWidget::getIndex()
{
    return m_index;
}


