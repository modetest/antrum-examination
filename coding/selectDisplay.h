#pragma once
#include <qwidget.h>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qdebug.h>

class selectDisplay :
    public QWidget
{
    Q_OBJECT
public:
    selectDisplay(const short t,const QString title,QString& text ,QWidget* parent = nullptr);
    ~selectDisplay();

signals:
    void removeItme(short t,QString& key);

public slots:
    void OnDelButton();

private:
    short type;
    QString key;
};

