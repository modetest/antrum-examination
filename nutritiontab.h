#ifndef NUTRITIONTAB_H
#define NUTRITIONTAB_H

#include <QWidget>
#include <qlabel.h>
#include <qlineedit.h>

#include "Data.h"
#include "sqliteManage.h"


namespace Ui {
class nutritionTab;
}

class nutritionTabLogic:public QObject
{
    Q_OBJECT
public:
	nutritionTabLogic(QObject* parent = nullptr);
	~nutritionTabLogic();
    
    void GetNuDta(int id, QVector<nutritionalInformation>& vec);

private:
    int clinialID;
};



class nutritionTab : public QWidget
{
    Q_OBJECT

public:
    explicit nutritionTab(int id,QWidget *parent = nullptr);
    ~nutritionTab();

    int GetVectorSize();
private:
    Ui::nutritionTab *ui;

    QVector<nutritionalInformation> vec;
};

#endif // NUTRITIONTAB_H
