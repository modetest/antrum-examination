#include "nutritiontab.h"
#include "ui_nutritiontab.h"

nutritionTab::nutritionTab(int id,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::nutritionTab)
{
    ui->setupUi(this);
    QVBoxLayout* mainLayout = (QVBoxLayout*)ui->verticalWidget->layout();
    nutritionTabLogic* m_logic = new nutritionTabLogic(this);
    vec.clear();
    m_logic->GetNuDta(id, vec);
    if (vec.size() > 0)
    {
        mainLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Maximum));
        QLabel* title = new QLabel(ui->verticalWidget);
        title->setText(u8"营养治疗方案：");
        title->setStyleSheet("QLabel{color:white;font:28px}");
        mainLayout->addWidget(title);
        for (size_t i = 0; i < vec.size(); i++)
        {
            QString str;
            if (vec[i].name == "EN")
            {
                str = vec[i].name + u8"：营养配方 " + vec[i].Formula + u8", 容量 " + vec[i].Volume + u8"ml, 总热卡 " + vec[i].Kcal + u8"kcal, 蛋白质含量 " + vec[i].Protein + u8"g, 开始时间 " + vec[i].StartTime;
            }
            else
            {
                str = vec[i].name + u8"：营养配方 " + vec[i].Formula + u8", 容量 " + vec[i].Volume + u8"ml, 总热卡 " + vec[i].Kcal + u8"kcal, 蛋白质含量 " + vec[i].Protein + u8"g";
            }
           
            QHBoxLayout* box = new QHBoxLayout();
            box->setContentsMargins(60, 0, 0, 0);
            QLabel* label = new QLabel(ui->verticalWidget);
            label->setText(str);
            label->setWordWrap(true);
            label->setFixedWidth(980);
            label->setStyleSheet("QLabel{background-color:#354052;color:white;border:none;font:21px}"); 
            box->addWidget(label);
            mainLayout->addLayout(box);
        }
    }

    mainLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

nutritionTab::~nutritionTab()
{
    delete ui;
}

int nutritionTab::GetVectorSize()
{
    return vec.size();
}

nutritionTabLogic::nutritionTabLogic(QObject* parent):QObject(parent)
{

}

nutritionTabLogic::~nutritionTabLogic()
{
}

void nutritionTabLogic::GetNuDta(int id, QVector<nutritionalInformation>& vec)
{
    QSqlQuery query;
    int clinialID = 0;

    query.prepare("SELECT id FROM clinical_information WHERE pid = :id ORDER BY id DESC LIMIT 1;");

    query.bindValue(":id", id);

    if (query.exec())
    {
        while (query.next())
        {
            clinialID = query.value(0).toInt();
        }
    }

    if (clinialID == 0)
        return;
    

    query.prepare("SELECT name,Formula,Volume,Kcal,Protein,StartTime FROM nutrition_information WHERE pid = :id;");

    query.bindValue(":id", clinialID);

    if (query.exec())
    {
        while (query.next())
        {
            nutritionalInformation info;
            info.name = query.value(0).toString();
            info.Formula = query.value(1).toString();
            info.Volume = query.value(2).toString();
            info.Kcal = query.value(3).toString();
            info.Protein = query.value(4).toString();
            info.StartTime = query.value(5).toString();
            vec.append(info);
        }
    }
}

