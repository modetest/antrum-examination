#ifndef FORM_H
#define FORM_H

#include <QWidget>

#include "Data.h"
#include "sqliteManage.h"

namespace Ui {
class Form;
}

class FormLogic:public QObject
{
    Q_OBJECT
public:
	FormLogic(int id,QObject* parent  = nullptr);
	~FormLogic();

    void GetBaseData(int id,clinicalInformation& info);
    void GetBreatheData(clinicalInformation& info);
    void GetClinicalData(clinicalInformation& info);
    

private:
    int clinicalId;
};


class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(int id,QWidget *parent = nullptr);
    ~Form();

    QString& GetAcTime();

private:
    Ui::Form *ui;

    QString m_AcTime;
};

#endif // FORM_H
