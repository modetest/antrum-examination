#ifndef NEWPATIENT_H
#define NEWPATIENT_H

#include <qdialog.h>
#include <qscrollarea.h>
#include <qpushbutton.h>
#include <QTableWidget>
#include <QHeaderView>
#include <qmessagebox.h>
#include <QFileDialog>
#include <QScrollBar>

#include "Essentialinformation.h"
#include "Clinicalinformation.h"
#include "Nutritionalinformation.h"
#include "newPatientLogic.h"

namespace Ui {
class newPatient;
}

class newPatient : public QDialog
{
    Q_OBJECT

public:
    explicit newPatient(QWidget *parent = nullptr);
    ~newPatient();
public slots:
    void onNewButtonClicked();
    void onImportButtonClicked();


private:
    bool getPageData(int& id,int& clid);

signals:
    void Done(int id,int clid,QString& filePath);

private:
    Ui::newPatient *ui;
    newPatientLogic* m_Logic;
    EssentialInformation* es;
    ClinicalInformation* cl;
    NutritionalInformation* nu;

    QScrollArea* srollArea;
};

#endif // NEWPATIENT_H
