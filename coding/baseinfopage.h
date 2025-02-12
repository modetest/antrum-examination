#ifndef BASEINFOPAGE_H
#define BASEINFOPAGE_H

#include <QDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <qscrollarea.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

#include "BaseInfoLogic.h"
#include "tableItemWidget.h"
#include "UMessageBox.h"

#include "ClinicalInformation.h"
#include "NutritionalInformation.h"

namespace Ui {
class BaseInfoPage;
}

class BaseInfoPage : public QDialog
{
    Q_OBJECT

public:
    explicit BaseInfoPage(int id, QWidget*parent = nullptr);
    ~BaseInfoPage();

signals:
    void Done(int id, int clid, QString& filePath);

public slots:
    void OnNewAqButton();
    void onNewButtonClicked();
    void onImportButtonClicked();
    void OnInfoPushButton();
    void OnDelPushButton();

private:
    bool GetPageData();
    void RefreshTableData();

private:
    Ui::BaseInfoPage *ui;

    int BaseID;
    int clID;

    BaseInfoLogic* m_logic;

    QPushButton* newClAqButton;

    QTableWidget* table;

    ClinicalInformation* cl;
    NutritionalInformation* nu;

    QPushButton* newAqButton;
    QPushButton* importImageButton;

    tableItemWidget** m_TableItemArray;
};

#endif // BASEINFOPAGE_H
