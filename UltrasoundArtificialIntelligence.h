#pragma once

#include <QtWidgets/QMainWindow>
#include <qdebug.h>
#include <QHeaderView>
#include <QButtonGroup>
#include <qmessagebox.h>


#include "ui_UltrasoundArtificialIntelligence.h"
#include "titleBarWidget.h"
#include "tableItemWidget.h"
#include "UltrasoundArtficialIntellgenceLogic.h"
#include "baseinfopage.h"
#include "newpatient.h"
#include "acquireImageWidget.h"

const  int pageButtonCount = 11;

class UltrasoundArtificialIntelligence : public QMainWindow
{
    Q_OBJECT

public:
    UltrasoundArtificialIntelligence(QWidget *parent = nullptr);
    ~UltrasoundArtificialIntelligence();

private:
    void InitPageButton();
    int RefreshTableData(int pageIndex);

public slots:
    void OnSearchButton();
    void OnNewPatientButton();
    void OnInfoPushButton();
    void OnDelPushButton();
    void OnPreviousButton();
    void OnNextButton();
    void OnNewPatientDone(int id, int clid, QString& path);

    //void OnButtonGroupClicked(int id);
signals:
    void newPatientSignal();
    void InfoSignal(int id);

private:
    Ui::UltrasoundArtificialIntelligenceClass ui;

    UltrasoundArtficialIntellgenceLogic* m_Logic;

    tableItemWidget** m_TableItemArray;

    //QButtonGroup* btnGroup;
    QLabel* curIndexLabel;
};
