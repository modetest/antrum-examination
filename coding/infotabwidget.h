#ifndef INFOTABWIDGET_H
#define INFOTABWIDGET_H

#include <QWidget>

#include "Data.h"
#include "sqliteManage.h"

namespace Ui {
class InfoTabWidget;
}

class InfoTabLogic : public QObject
{
    Q_OBJECT
public:
	InfoTabLogic(QObject* parent = nullptr);
	~InfoTabLogic();

    void GetBaseInfo(int id,baseInformation& info);

private:
    sqliteManage* m_sql;
};

class InfoTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InfoTabWidget(int id,QWidget *parent = nullptr);
    ~InfoTabWidget();

private:
    Ui::InfoTabWidget *ui;
};

#endif // INFOTABWIDGET_H
