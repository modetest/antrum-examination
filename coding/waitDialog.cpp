#include "waitDialog.h"

waitDialog::waitDialog(QWidget* parent) :QDialog(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint); //ȥ������߿�
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
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);//������ʽ

    //2. �������룬Լ��painter��������
    QBitmap bmp(this->size());
    bmp.fill();//fill(const QColor &fillColor = Qt::white);

    QPainter painter(&bmp);
    //painter.setRenderHint(QPainter::Antialiasing, true);//�����ͼ�о��,�˷�ʽ��������bitmap��Ч
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);//����ֵ
    painter.drawRoundedRect(bmp.rect(), 36, 36);//��bitmap�л���Բ�Ǿ���

    setMask(bmp);//���þ���������ȷ�����ؼ��ɻ�������

    QDialog::paintEvent(event);

}
