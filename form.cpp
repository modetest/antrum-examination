#include "form.h"
#include "ui_form.h"

Form::Form(int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    QVBoxLayout* mainLayout = (QVBoxLayout*)ui->scrollAreaWidgetContents->layout();

    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    FormLogic* m_logic = new FormLogic(id,this);
    clinicalInformation info;
    m_logic->GetBaseData(id,info);
    ui->xllineEdit->setText(info.hreat_rate);
    ui->xnlineEdit->setText(info.heart_Rhythm);
    ui->RRLineEdit->setText(info.RR);
    ui->souLineEdit->setText(info.SBP);
    ui->suLineEdit->setText(info.DBP);
    ui->mapLineEdit->setText(info.MAP);
    ui->PILineEdit->setText(info.PI);
    ui->SPOlineEdit->setText(info.SPO2);
    ui->USCOdelineEdit->setText(info.USCode);

    m_AcTime = info.AcTime;

    //呼吸
    m_logic->GetBreatheData(info);
    if (info.vecBreathe.size() > 0)
    {
        mainLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Maximum));
        QLabel* title = new QLabel(ui->scrollAreaWidgetContents);
        title->setText(u8"呼吸方式：");
        title->setStyleSheet("QLabel{color:white;font:28px}");
        mainLayout->addWidget(title);
        for (size_t i = 0; i < info.vecBreathe.size(); i++)
        {
            QString str;
         
            if (info.vecBreathe[i].pattern == u8"高流量吸氧")
            {
                str = info.vecBreathe[i].pattern + ": " + info.vecBreathe[i].LMin + " L/min, FiO2 " + info.vecBreathe[i].FiO2 + " %";
            }
            else if (info.vecBreathe[i].pattern == u8"无创呼吸机辅助呼吸")
            {
                str = info.vecBreathe[i].pattern + ": FiO2 " + info.vecBreathe[i].FiO2 + " %, IPAP " + info.vecBreathe[i].IPAP + " cmH2O, EPAP " + info.vecBreathe[i].EPAP + " cmH2O";
            }
            else if (info.vecBreathe[i].pattern == u8"有创呼吸机辅助呼吸")
            {
                str = info.vecBreathe[i].pattern + ": FiO2 " + info.vecBreathe[i].FiO2 + " %, PEEP " + info.vecBreathe[i].PEEP + u8" cmH2O, 呼吸机模式 " + info.vecBreathe[i].hreathModel;
            }
            else
            {
                str = info.vecBreathe[i].pattern + ": " + info.vecBreathe[i].LMin + " L/min";

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

    //药物
    m_logic->GetClinicalData(info);
    if (info.vecActivity.size() > 0)
    {
        mainLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Maximum));
        QLabel* title = new QLabel(ui->scrollAreaWidgetContents);
        title->setText(u8"血管活性药物：");
        title->setStyleSheet("QLabel{color:white;font:28px}");
        mainLayout->addWidget(title);
        for (size_t i = 0; i < info.vecActivity.size(); i++)
        {
            QString str;
            str = info.vecActivity[i].name + u8"：药物总剂量 " + info.vecActivity[i].TotalVolume + u8"mg, 溶媒容积" + info.vecActivity[i].MediaVolume + u8"ml, 泵入速度 " + info.vecActivity[i].PumpingSpeed + u8"ml/h, 泵入剂量 " + info.vecActivity[i].PumpingVolume + "ug/kg/min";

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
    if (info.vecAnalgesia.size() > 0)
    {
        mainLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Maximum));
        QLabel* title = new QLabel(ui->scrollAreaWidgetContents);
        title->setText(u8"镇痛药物：");
        title->setStyleSheet("QLabel{color:white;font:28px}");
        mainLayout->addWidget(title);
        for (size_t i = 0; i < info.vecAnalgesia.size(); i++)
        {
            QString str;
            str = info.vecAnalgesia[i].name + u8"：药物总剂量 " + info.vecAnalgesia[i].TotalVolume + u8"mg, 溶媒容积" + info.vecAnalgesia[i].MediaVolume + u8"ml, 泵入速度 " + info.vecAnalgesia[i].PumpingSpeed + u8"ml/h, 泵入剂量 " + info.vecAnalgesia[i].PumpingVolume + u8"ug/kg/min, CPOT评分 "+ info.vecAnalgesia[i].Score;

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
    if (info.vecCalm.size() > 0)
    {
        mainLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Maximum));
        QLabel* title = new QLabel(ui->scrollAreaWidgetContents);
        title->setText(u8"镇静药物：");
        title->setStyleSheet("QLabel{color:white;font:28px}");
        mainLayout->addWidget(title);
        for (size_t i = 0; i < info.vecCalm.size(); i++)
        {
            QString str;
            str = info.vecCalm[i].name + u8"：药物总剂量 " + info.vecCalm[i].TotalVolume + u8"mg, 溶媒容积" + info.vecCalm[i].MediaVolume + u8"ml, 泵入速度 " + info.vecCalm[i].PumpingSpeed + u8"ml/h, 泵入剂量 " + info.vecCalm[i].PumpingVolume + "ug/kg/min";

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
    if (info.vecMuscleRelaxation.size() > 0)
    {
        mainLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Maximum));
        QLabel* title = new QLabel(ui->scrollAreaWidgetContents);
        title->setText(u8"肌松药物：");
        title->setStyleSheet("QLabel{color:white;font:28px}");
        mainLayout->addWidget(title);
        for (size_t i = 0; i < info.vecMuscleRelaxation.size(); i++)
        {
            QString str;
            str = info.vecMuscleRelaxation[i].name + u8"：药物总剂量 " + info.vecMuscleRelaxation[i].TotalVolume + u8"mg, 溶媒容积" + info.vecMuscleRelaxation[i].MediaVolume + u8"ml, 泵入速度 " + info.vecMuscleRelaxation[i].PumpingSpeed + u8"ml/h, 泵入剂量 " + info.vecMuscleRelaxation[i].PumpingVolume + "ug/kg/min";

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

Form::~Form()
{
    delete ui;
}

QString& Form::GetAcTime()
{
    return m_AcTime;
}

FormLogic::FormLogic(int id, QObject* parent)
{
    clinicalId = 0;
}

FormLogic::~FormLogic()
{
}

void FormLogic::GetBaseData(int id, clinicalInformation& info)
{
    QSqlQuery query;

    query.prepare("SELECT heart_rate,heart_Rhythm,SBP,DBP,MAP,SPO2,PI,RR,USCode,AcTime,id FROM clinical_information WHERE pid = :id ORDER BY id DESC LIMIT 1;");

    query.bindValue(":id", id);

    if (query.exec())
    {
        while (query.next())
        {
            info.hreat_rate = query.value(0).toString();
            info.heart_Rhythm = query.value(1).toString();
            info.SBP = query.value(2).toString();
            info.DBP = query.value(3).toString();
            info.MAP = query.value(4).toString();
            info.SPO2 = query.value(5).toString();
            info.PI = query.value(6).toString(); 
            info.RR = query.value(7).toString();
            info.USCode = query.value(8).toString(); 
            info.AcTime = query.value(9).toString();
            clinicalId = query.value(10).toInt();
        }
    }
}

void FormLogic::GetBreatheData(clinicalInformation& info)
{
    QSqlQuery query;

    if (clinicalId == 0)
        return;

   //query.prepare("SELECT name,TotalVolume,MediaVolume,PumpingSpeed,PumpingVolume,Score,type FROM clinical_information_medicine WHERE pid = :id;");
    query.prepare("SELECT pattern,LMin,FiO2,IPAP,EPAP,PEEP,hreathModel FROM breathe_information WHERE pid = :id;");

    query.bindValue(":id", clinicalId);

    if (query.exec())
    {
      
        while (query.next())
        {
            breathe bInfo;
            bInfo.pattern = query.value(0).toString();
            bInfo.LMin = query.value(1).toString();
            bInfo.FiO2 = query.value(2).toString();
            bInfo.IPAP = query.value(3).toString();
            bInfo.EPAP = query.value(4).toString();
            bInfo.PEEP = query.value(5).toString();
            bInfo.hreathModel = query.value(6).toString();     
            info.vecBreathe.append(bInfo);
        }
    }
}

void FormLogic::GetClinicalData(clinicalInformation& info)
{
    QSqlQuery query;

    if (clinicalId == 0)
        return;

    query.prepare("SELECT name,TotalVolume,MediaVolume,PumpingSpeed,PumpingVolume,Score,type FROM clinical_information_medicine WHERE pid = :id;");
  
    query.bindValue(":id", clinicalId);

    if (query.exec())
    {

        while (query.next())
        {
            medicine cInfo;
            short type = 0;
            cInfo.name = query.value(0).toString();
            cInfo.TotalVolume = query.value(1).toString();
            cInfo.MediaVolume = query.value(2).toString();
            cInfo.PumpingSpeed = query.value(3).toString();
            cInfo.PumpingVolume = query.value(4).toString();
            cInfo.Score = query.value(5).toString();
            type = query.value(6).toInt();

            switch (type)
            {
            case VASOACTIVITY:
                info.vecActivity.append(cInfo);
                break;
            case ANALGESIA:
                info.vecAnalgesia.append(cInfo);
                break;
            case CALM:
                info.vecCalm.append(cInfo);
                break;
            case MUSCLERELAXATION:
                info.vecMuscleRelaxation.append(cInfo);
                break;
            default:
                break;
            }
        }
    }
}

