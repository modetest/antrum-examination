#include "infotabwidget.h"
#include "ui_infotabwidget.h"

InfoTabWidget::InfoTabWidget(int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoTabWidget)
{
    ui->setupUi(this);
    InfoTabLogic* m_Logic = new InfoTabLogic(this);
    //this->setWindowFlags(Qt::Widget);
    baseInformation info;
    m_Logic->GetBaseInfo(id, info);

    ui->numberLineEdit->setText(info.number);
    ui->nameLineEdit->setText(info.name);
    ui->ageLineEdit->setText(info.age);
    ui->sexLineEdit->setText(info.sex);
    ui->heightLineEdit->setText(info.height);
    ui->weightLineEdit->setText(info.actual_weight);
    ui->iHeightLineEdit->setText(info.ideal_weight);
    ui->BSILineEdit->setText(info.BSI);
    ui->BMILineEdit->setText(info.BMI);
    ui->APLineEdit->setText(info.APACHE2);
    ui->SOFALineEdit->setText(info.SOFA);
    ui->hTimelineEdit->setText(info.admission_date);
    ui->iTimelineEdit->setText(info.icu_date);
    ui->textEdit->setText(info.principal_diagnosis);
}

InfoTabWidget::~InfoTabWidget()
{
    delete ui;
}


//Logic
InfoTabLogic::InfoTabLogic(QObject* parent):QObject(parent)
{
    //m_sql = sqliteManage::getInstance();
}

InfoTabLogic::~InfoTabLogic()
{
}

void InfoTabLogic::GetBaseInfo(int id, baseInformation& info)
{
    QSqlQuery query;

    query.prepare("SELECT * FROM basic_information WHERE id = :id;");

    query.bindValue(":id", id);

    if (query.exec())
    {

        while (query.next())
        {
            info.number = query.value(1).toString();
            info.name = query.value(2).toString();
            info.age = query.value(3).toString();
            info.sex = query.value(4).toString();
            info.height = query.value(5).toString();
            info.actual_weight = query.value(6).toString();
            info.ideal_weight = query.value(7).toString();
            info.BSI = query.value(8).toString();
            info.BMI = query.value(9).toString();
            info.APACHE2 = query.value(10).toString();
            info.SOFA = query.value(11).toString();
            info.admission_number = query.value(12).toString();
            info.hospital = query.value(13).toString();
            info.administrative_office = query.value(14).toString();
            info.inpatient_ward = query.value(15).toString();
            info.bed_number = query.value(16).toString();
            info.treat_group_leader = query.value(17).toString();
            info.attending_doctor = query.value(18).toString();
            info.bed_doctor = query.value(19).toString();
            info.acquisition_doctor = query.value(20).toString();
            info.patients_source_type = query.value(21).toString();
            info.patients_source = query.value(22).toString();
            info.admission_date = query.value(23).toString();
            info.icu_date = query.value(24).toString();
            info.principal_diagnosis = query.value(25).toString();

           /* info.number = query.value(0).toString();
            info.name = query.value(1).toString();
            info.age = query.value(2).toString();
            info.sex = query.value(3).toString();
            info.height = query.value(4).toString();
            info.actual_weight = query.value(5).toString();
            info.ideal_weight = query.value(6).toString();
            info.BSI = query.value(7).toString();
            info.BMI = query.value(8).toString();
            info.APACHE2 = query.value(9).toString();
            info.SOFA = query.value(10).toString();
            info.admission_number = query.value(11).toString();
            info.hospital = query.value(12).toString();
            info.administrative_office = query.value(13).toString();
            info.inpatient_ward = query.value(14).toString();
            info.bed_number = query.value(15).toString();
            info.medical_group = query.value(16).toString();
            info.treat_group_leader = query.value(17).toString();
            info.attending_doctor = query.value(18).toString();
            info.bed_doctor = query.value(19).toString();
            info.acquisition_doctor = query.value(20).toString();
            info.patients_source = query.value(21).toString();
            info.admission_date = query.value(22).toString();
            info.icu_date = query.value(23).toString();
            info.principal_diagnosis = query.value(24).toString();*/
        }
    }
}

