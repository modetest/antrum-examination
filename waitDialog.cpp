#include "waitDialog.h"

waitDialog::waitDialog(QWidget* parent) :QDialog(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint); //去除标题边框
    this->setAttribute(Qt::WA_TranslucentBackground);
}

waitDialog::~waitDialog()
{
}

void waitDialog::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);//绘制样式

    //2. 设置掩码，约定painter绘制区域
    QBitmap bmp(this->size());
    bmp.fill();//fill(const QColor &fillColor = Qt::white);

    QPainter painter(&bmp);
    //painter.setRenderHint(QPainter::Antialiasing, true);//解决绘图有锯齿,此方式针对掩码的bitmap无效
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);//掩码值
    painter.drawRoundedRect(bmp.rect(), 36, 36);//在bitmap中绘制圆角矩形

    setMask(bmp);//设置矩形掩码以确定本控件可绘制区域

    QDialog::paintEvent(event);

}
