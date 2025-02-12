#include "acquireImageLogic.h"

acquireImageLogic::acquireImageLogic(cv::dnn::Net* net, QString rootPath, QObject* parent)
{
    m_net = net;
    m_SrcVideoPath = rootPath;
    isRun = false;
    if (colors.empty())
    {
        // Generate colors.
        colors.push_back(cv::Vec3b());
        for (int i = 1; i < 255; ++i)
        {
            cv::Vec3b color;
            for (int j = 0; j < 3; ++j)
                color[j] = (colors[i - 1][j] + rand() % 256) / 2;
            colors.push_back(color);
        }
    }
    colors[21] = { 0, 0, 255 };   // 胃超_胃窦  red
    colors[22] = { 0,255, 0 };    // 胃超_腹主动脉  green
    colors[23] = { 255,0, 0 };    // 胃超_肝左叶 blue

    std::string path = "./demo_pictures";

    m_pMatchRler = new matchRuler(path,this);
    unit = 0.0f;
}

acquireImageLogic::~acquireImageLogic()
{

}

void acquireImageLogic::SetDiskPath(QString& rootPath, int baseID, int clinialID)
{
    std::string result  = rootPath.toStdString() + "/resultSet";

    if (!std::filesystem::exists(result)) {
        std::filesystem::create_directory(result);
    }

    std::string BasePath  = result;

    BasePath = BasePath + "/" + std::to_string(baseID);

    if (!std::filesystem::exists(BasePath)) {
        std::filesystem::create_directory(BasePath);
    }
    
    std::string clinialPath  = BasePath;

    clinialPath = clinialPath + "/" + std::to_string(clinialID);

    if (!std::filesystem::exists(clinialPath)) {
        std::filesystem::create_directory(clinialPath);
    }

    picPath = clinialPath + "/" + "pic";
    videoPath = clinialPath + "/" + "video";

    if (!std::filesystem::exists(picPath)) {
        std::filesystem::create_directory(picPath);
    }
    if (!std::filesystem::exists(videoPath)) {
        std::filesystem::create_directory(videoPath);
    }
}

void acquireImageLogic::removeClinicalData(QString& rootPath, QString& baseID, QString& clinialID)
{
    //删除磁盘数据
    auto delPath = rootPath + "/resultSet/" + baseID + "/" + clinialID;
    QDir directory(delPath);
    bool success = directory.removeRecursively();
    if (!success)
        qDebug() << "delete dir error!";

    //删除库数据
    QSqlQuery query;
    query.prepare("DELETE FROM collection WHERE pid = :clinicalID");
    query.bindValue(":clinicalID", clinialID);
    if (!query.exec())
    {
        qDebug() << query.lastError();
    }

}

void acquireImageLogic::reAnalysisData()
{

}

void acquireImageLogic::SetIsRun(bool b)
{
    Mutex.lock();
    isRun = b;
    Mutex.unlock();
}

bool acquireImageLogic::GetIsRun()
{
    return isRun;
}

void acquireImageLogic::SetType(short t)
{
    m_sType = t;
}

void acquireImageLogic::setUnitZore()
{
    unit = 0.0;
}

cv::Mat& acquireImageLogic::GetMat()
{
    QMutexLocker locker(&m_mutex);
    return m_frame;
}

void acquireImageLogic::saveData(int ppid, collectionData& data)
{
    //用一下actime 存cliniaclID
    QSqlQuery query;
    query.prepare("SELECT AcTime FROM clinical_information WHERE id = :id");
    query.bindValue(":id", data.pid);
    if (query.exec())
    {
        while (query.next())
        {
            data.acTime = query.value(0).toString();
        }
    }

    query.prepare(" DELETE FROM collection WHERE pid = :pid");
    query.bindValue(":pid", data.pid);
    if (!query.exec())
        qDebug() << "sql error" << query.lastError();


    query.prepare("INSERT INTO collection(ppid,pid,acTime,startTime,endTime,cycleTime,count,diastoleArea,contractArea) VALUES(:ppid,:pid,:acTime,:startTime,:endTime,:cycleTime,:count,:diastoleArea,:contractArea);");
    query.bindValue(":ppid", ppid);
    query.bindValue(":pid", data.pid);
    query.bindValue(":acTime", data.acTime);
    query.bindValue(":startTime", data.startTime);
    query.bindValue(":endTime", data.endTime);
    query.bindValue(":cycleTime", data.cycleTime);
    query.bindValue(":count", data.count);
    query.bindValue(":diastoleArea", data.diastoleArea);
    query.bindValue(":contractArea", data.contractArea);
    if (!query.exec())
    {
        //打印sql语句错误信息
        qDebug() << "sql error" << query.lastError();
    }
}

void acquireImageLogic::saveImage(QString& path, QImage& mainPage, QImage& base, QImage& clinical, QImage& nu)
{
    cv::Mat mainPageMat = cv::Mat(mainPage.height(), mainPage.width(), CV_8UC4, const_cast<uchar*>(mainPage.constBits()), mainPage.bytesPerLine());
    cv::Mat imgBase = cv::Mat(base.height(), base.width(), CV_8UC4, const_cast<uchar*>(base.constBits()), base.bytesPerLine());
    cv::Mat imgClinical = cv::Mat(clinical.height(), clinical.width(), CV_8UC4, const_cast<uchar*>(clinical.constBits()), clinical.bytesPerLine());
    cv::Mat imgNu = cv::Mat(nu.height(), nu.width(), CV_8UC4, const_cast<uchar*>(nu.constBits()), nu.bytesPerLine());

    cv::resize(imgBase, imgBase, cv::Size(mainPageMat.cols, imgBase.rows), cv::INTER_LINEAR);
    cv::resize(imgNu, imgNu, cv::Size(mainPageMat.cols, imgNu.rows), cv::INTER_LINEAR);
    cv::resize(imgClinical, imgClinical, cv::Size(mainPageMat.cols, imgClinical.rows), cv::INTER_LINEAR);


    cv::vconcat(imgBase, imgClinical, imgBase);
    cv::vconcat(imgBase, imgNu, imgBase);
    cv::vconcat(imgBase, mainPageMat, imgBase);

    path = path + "/report.png";
    cv::imwrite(path.toStdString().c_str(), imgBase);
}

void acquireImageLogic::saveCurveImage(QString& path, QImage& curveImage)
{
    cv::Mat cureImage = cv::Mat(curveImage.height(), curveImage.width(), CV_8UC4, const_cast<uchar*>(curveImage.constBits()), curveImage.bytesPerLine());
    auto ImgPath  = path + "/curve.png";
    cv::imwrite(ImgPath.toStdString().c_str(), cureImage);
}

void acquireImageLogic::saveImage(QString& path, QImage& mainPage, QImage& base, QImage& clinical)
{
    cv::Mat mainPageMat = cv::Mat(mainPage.height(), mainPage.width(), CV_8UC4, const_cast<uchar*>(mainPage.constBits()), mainPage.bytesPerLine());
    cv::Mat imgBase = cv::Mat(base.height(), base.width(), CV_8UC4, const_cast<uchar*>(base.constBits()), base.bytesPerLine());
    cv::Mat imgClinical = cv::Mat(clinical.height(), clinical.width(), CV_8UC4, const_cast<uchar*>(clinical.constBits()), clinical.bytesPerLine());

    cv::resize(imgBase, imgBase, cv::Size(mainPageMat.cols,imgBase.rows ),cv::INTER_LINEAR);
    cv::resize(imgClinical, imgClinical, cv::Size(mainPageMat.cols, imgClinical.rows), cv::INTER_LINEAR);


    cv::vconcat(imgBase, imgClinical, imgBase);
    cv::vconcat(imgBase, mainPageMat, imgBase);

    path = path + "/report.png";
    cv::imwrite(path.toStdString().c_str(), imgBase);
}

void acquireImageLogic::SetUnit(QList<QPoint>& u)
{
    qDebug() << "SetUnit";
    unit = sqrt(pow(u[0].x() - u[1].x(),2)+ pow(u[0].y() - u[1].y(),2));
}

void acquireImageLogic::pauseThread()
{
    qDebug() << "pauseThread";
    this->m_Qmutex_pauseLock.lock();
}

void acquireImageLogic::resumeThread()
{
    qDebug() << "resumeThread";
    this->m_Qmutex_pauseLock.unlock();
}

float acquireImageLogic::GetUnit()
{
    return unit;
}

short acquireImageLogic::GetCameraCount()
{
    return cameraCount;
}

void acquireImageLogic::SetCameraCount(short count)
{
    cameraCount = count;
}

void acquireImageLogic::SetCameraIndex(int index)
{
    cameraIndex = index;
}

//void acquireImageLogic::writeReport(QString rootPath,int baseID,int clID)
//{
//    QString xmlPath = rootPath + "/template";
//    QString ImgPath = rootPath + "/resultSet/" + baseID + "/" + clID + "/curve.png";
//    QString savePath = rootPath + "/resultSet/" + baseID + "/" + clID + "/export.docx";
//
//    QString modifiedPath = rootPath + "/template/modified.xml";
//
//    baseInformation baseInfo;
//    clinicalInformation clinicalInfo;
//    QVector<nutritionalInformation> nuVec;
//    collectionData collectioncInfo;
//    
//    //queryData(baseID,clID, baseInfo, clinicalInfo, nuVec, collectioncInfo);
//
//    //insertImageToTemplate(xmlPath, ImgPath);
//
//    QFile file(modifiedPath);
//    if (!file.open(QIODevice::ReadOnly))
//    {
//        qDebug() << "open xml file fail. " << file.errorString();
//        return;
//    }
//    QByteArray baContent = file.readAll();
//    file.close();
//    QString strAllContent = QString::fromUtf8(baContent);
//    strAllContent.replace("$USCode", clinicalInfo.USCode);
//    strAllContent.replace("$DataTime", clinicalInfo.AcTime);
//    strAllContent.replace("$PatientID",QString::number(baseID));
//
//    strAllContent.replace("$code", "CODE");
//    strAllContent.replace("$name", baseInfo.name);
//    strAllContent.replace("$age", baseInfo.age);
//    strAllContent.replace("$sex", baseInfo.sex);
//    strAllContent.replace("$height", baseInfo.height);
//    strAllContent.replace("$weight", baseInfo.actual_weight);
//    strAllContent.replace("$ideal_weight", baseInfo.ideal_weight);
//    strAllContent.replace("$BMI", baseInfo.BMI);
//    strAllContent.replace("$APACHE2", baseInfo.APACHE2);
//
//    strAllContent.replace("$SOFA", baseInfo.SOFA);
//    strAllContent.replace("$admission_number", baseInfo.admission_number);
//    strAllContent.replace("$hospital", baseInfo.hospital);
//    strAllContent.replace("$departments", baseInfo.administrative_office);
//    strAllContent.replace("$ward", baseInfo.inpatient_ward);
//    strAllContent.replace("$bed_number", baseInfo.bed_number);
//    strAllContent.replace("$medical_team", baseInfo.medical_group);
//    strAllContent.replace("$medical_leader", baseInfo.treat_group_leader);
//    strAllContent.replace("$ysician",baseInfo.attending_doctor);
//
//    strAllContent.replace("$doctor", baseInfo.bed_doctor);
//    strAllContent.replace("$pher", baseInfo.acquisition_doctor);
//    strAllContent.replace("$admission_time", baseInfo.admission_date);
//    strAllContent.replace("$ICU_time", baseInfo.icu_date);
//
//    strAllContent.replace("$diagnosis", baseInfo.principal_diagnosis);
//
//
//    strAllContent.replace("$heart_rate", clinicalInfo.hreat_rate);
//    strAllContent.replace("$rhythm", clinicalInfo.heart_Rhythm);
//    strAllContent.replace("$sure ", clinicalInfo.SBP);
//    strAllContent.replace("$diastolic_", clinicalInfo.DBP);
//    strAllContent.replace("$MAP", clinicalInfo.MAP);
//    strAllContent.replace("$SPO", clinicalInfo.SPO2);
//    strAllContent.replace("$RR", clinicalInfo.RR);
//    strAllContent.replace("$PI", clinicalInfo.PI);
//
//    //组合字符
//    QString medicants;
//    if (clinicalInfo.vecActivity.size() > 0)
//    {
//        medicants =  clinicalInfo.vecActivity[0].name + ": " + clinicalInfo.vecActivity[0].TotalVolume + "mg, " + clinicalInfo.vecActivity[0].MediaVolume + "ml, " + clinicalInfo.vecActivity[0].PumpingSpeed + "ml/h, " + clinicalInfo.vecActivity[0].PumpingVolume + "ug/kg/min, " + u8"评分" + clinicalInfo.vecActivity[0].Score;
//    }
//
//    for (size_t i = 1; i < clinicalInfo.vecActivity.size(); i++)
//    {
//        medicants = medicants + "; "+ clinicalInfo.vecActivity[i].name + ": " + clinicalInfo.vecActivity[i].TotalVolume + "mg, " + clinicalInfo.vecActivity[i].MediaVolume + "ml, " + clinicalInfo.vecActivity[i].PumpingSpeed + "ml/h, " + clinicalInfo.vecActivity[i].PumpingVolume + "ug/kg/min, " + u8"评分" + clinicalInfo.vecActivity[i].Score;
//    }
//    QString nuStr;
//    if (nuVec.size() > 0)
//    {
//         nuStr =  nuVec[0].Formula + ", " + nuVec[0].Volume + "ml, " + nuVec[0].Kcal + "kcal, " + nuVec[0].Protein + u8"g, 开始时间：" + nuVec[0].StartTime;
//    }
//
//    for (size_t i = 1; i < clinicalInfo.vecActivity.size(); i++)
//    {
//        nuStr = nuVec[i].Formula + ", " + nuVec[0].Volume + "ml, " + nuVec[0].Kcal + "kcal, " + nuVec[0].Protein + u8"g, 开始时间：" + nuVec[0].StartTime;
//    }
//    
//    if (clinicalInfo.vecBreathe.size() > 0)
//    {
//        strAllContent.replace("$respiratory_s", clinicalInfo.vecBreathe[0].pattern);
//    }
//    else
//    {
//        strAllContent.replace("$respiratory_s",u8"无");
//    }
//    strAllContent.replace("$medicine", medicants);
//    strAllContent.replace("$nutrition", nuStr);
//
//    strAllContent.replace("$collection_time", collectioncInfo.acTime);
//    strAllContent.replace("$duration", collectioncInfo.cycleTime);
//    strAllContent.replace("$area", collectioncInfo.diastoleArea);//舒张
//    strAllContent.replace("$a", collectioncInfo.contractArea);//收缩
//    strAllContent.replace("$total_count", collectioncInfo.count);
//
//    //计算
//    //1/3(平均最大面积 - 平均最小面积) | 平均最大面积
//    auto diastole = collectioncInfo.diastoleArea.toFloat();
//    auto contract = collectioncInfo.contractArea.toFloat();
//    auto count = collectioncInfo.count.toInt();
//    
//    auto aca = (diastole - contract) / diastole;
//    float acf = (float)count / 3.0f;
//    auto MI = aca * acf;
//    strAllContent.replace("$MI", QString::number(MI, 'f', 2));
//    //Volume = 27.0 + 14.6 * CSA − 1.28 * age
//    float volume = 27.0 + 14.6 * diastole - 1.28 * baseInfo.age.toInt();
//    strAllContent.replace("$GRV", QString::number(volume, 'f', 2));
//
//    //输入
//    strAllContent.replace("$curve_rhythm", u8"没有节律");
//    strAllContent.replace("$check", u8"杨医生");
//    strAllContent.replace("$record", u8"杨医生");
//    strAllContent.replace("$auditing", u8"杨医生");
//
//    QFile newFile(savePath);
//    if (!newFile.open(QIODevice::WriteOnly))
//    {
//        qDebug() << "file open fail." << newFile.errorString();
//        return;
//    }
//
//    newFile.write(strAllContent.toUtf8());
//    newFile.close();
//}

//void acquireImageLogic::queryData(int baseID, int clID, baseInformation& baseInfo, clinicalInformation& clinInfo, QVector<nutritionalInformation>& nuInfo, collectionData& collectInfo)
//{
//    //基本信息
//    QSqlQuery query;
//    query.prepare("SELECT "
//        "serial_number, name, age, sex, height, actual_weight, ideal_weight, BSI, BMI, APACHE2, SOFA, "
//        "admission_number, hospital, administrative_office, inpatient_ward, bed_number, "
//        "treat_group_leader, attending_doctor, bed_doctor, acquisition_doctor,patients_source_type, patients_source, "
//        "admission_date, icu_date, principal_diagnosis "
//        "FROM basic_information WHERE id = :id;"
//    );
//
//    query.bindValue(":id", baseID);
//
//    if (query.exec()) {
//        while (query.next())
//        {
//            baseInfo.number = query.value(0).toString();
//            baseInfo.name = query.value(1).toString();
//            baseInfo.age = query.value(2).toString();
//            baseInfo.sex = query.value(3).toString();
//            baseInfo.height = query.value(4).toString();
//            baseInfo.actual_weight = query.value(5).toString();
//            baseInfo.ideal_weight = query.value(6).toString();
//            baseInfo.BSI = query.value(7).toString();
//            baseInfo.BMI = query.value(8).toString();
//            baseInfo.APACHE2 = query.value(9).toString();
//            baseInfo.SOFA = query.value(10).toString();
//            baseInfo.admission_number = query.value(11).toString();
//            baseInfo.hospital = query.value(12).toString();
//            baseInfo.administrative_office = query.value(13).toString();
//            baseInfo.inpatient_ward = query.value(14).toString();
//            baseInfo.bed_number = query.value(15).toString();
//            baseInfo.treat_group_leader = query.value(16).toString();
//            baseInfo.attending_doctor = query.value(17).toString();
//            baseInfo.bed_doctor = query.value(18).toString();
//            baseInfo.acquisition_doctor = query.value(19).toString();
//            baseInfo.patients_source_type = query.value(20).toString();
//            baseInfo.patients_source = query.value(21).toString();
//            baseInfo.admission_date = query.value(22).toString();
//            baseInfo.icu_date = query.value(23).toString();
//            baseInfo.principal_diagnosis = query.value(24).toString();
//        }
//    }
//    else
//    {
//        qDebug() << "sql query error: " << query.lastError();
//    }
//
//    //临床信息
//    query.prepare("SELECT heart_rate,heart_Rhythm,SBP,DBP,MAP,SPO2,PI,RR,USCode,AcTime,id FROM clinical_information WHERE id = :id ORDER BY id DESC LIMIT 1;");
//
//    query.bindValue(":id", clID);
//
//    if (query.exec())
//    {
//        while (query.next())
//        {  
//           clinInfo.hreat_rate = query.value(0).toString();
//           clinInfo.heart_Rhythm = query.value(1).toString();
//           clinInfo.SBP = query.value(2).toString();
//           clinInfo.DBP = query.value(3).toString();
//           clinInfo.MAP = query.value(4).toString();
//           clinInfo.SPO2 = query.value(5).toString();
//           clinInfo.PI = query.value(6).toString();
//           clinInfo.RR = query.value(7).toString();
//           clinInfo.USCode = query.value(8).toString();
//           clinInfo.AcTime = query.value(9).toString();
//        }
//    }
//    
//    //呼吸
//    query.prepare("SELECT pattern,LMin,FiO2,IPAP,EPAP,PEEP,hreathModel FROM breathe_information WHERE pid = :id;");
//
//    query.bindValue(":id", clID);
//
//    if (query.exec())
//    {
//
//        while (query.next())
//        {
//            breathe bInfo;
//            bInfo.pattern = query.value(0).toString();
//            bInfo.LMin = query.value(1).toString();
//            bInfo.FiO2 = query.value(2).toString();
//            bInfo.IPAP = query.value(3).toString();
//            bInfo.EPAP = query.value(4).toString();
//            bInfo.PEEP = query.value(5).toString();
//            bInfo.hreathModel = query.value(6).toString();
//            clinInfo.vecBreathe.append(bInfo);
//        }
//    }
//
//    //药物
//    query.prepare("SELECT name,TotalVolume,MediaVolume,PumpingSpeed,PumpingVolume,Score,type FROM clinical_information_medicine WHERE pid = :id;");
//
//    query.bindValue(":id", clID);
//
//    if (query.exec())
//    {
//
//        while (query.next())
//        {
//            medicine cInfo;
//            short type = 0;
//            cInfo.name = query.value(0).toString();
//            cInfo.TotalVolume = query.value(1).toString();
//            cInfo.MediaVolume = query.value(2).toString();
//            cInfo.PumpingSpeed = query.value(3).toString();
//            cInfo.PumpingVolume = query.value(4).toString();
//            cInfo.Score = query.value(5).toString();
//            type = query.value(6).toInt();
//
//            clinInfo.vecActivity.append(cInfo);
//
//            /*switch (type)
//            {
//            case VASOACTIVITY:
//                clinInfo.vecActivity.append(cInfo);
//                break;
//            case ANALGESIA:
//                clinInfo.vecAnalgesia.append(cInfo);
//                break;
//            case CALM:
//                clinInfo.vecCalm.append(cInfo);
//                break;
//            case MUSCLERELAXATION:
//                clinInfo.vecMuscleRelaxation.append(cInfo);
//                break;
//            default:
//                break;
//            }*/
//        }
//    }
//
//    //营养
//    query.prepare("SELECT name,Formula,Volume,Kcal,Protein,StartTime FROM nutrition_information WHERE pid = :id;");
//
//    query.bindValue(":id", clID);
//
//    if (query.exec())
//    {
//        while (query.next())
//        {
//            nutritionalInformation info;
//            info.name = query.value(0).toString();
//            info.Formula = query.value(1).toString();
//            info.Volume = query.value(2).toString();
//            info.Kcal = query.value(3).toString();
//            info.Protein = query.value(4).toString();
//            info.StartTime = query.value(5).toString();
//            nuInfo.append(info);
//        }
//    }
//
//    QString s = QString("SELECT pid,AcTime,cycleTime,count,diastoleArea,contractArea FROM collection WHERE pid IN (%1)").arg(clID);
//    if (query.exec(s))
//    {
//        while (query.next())
//        {
//           collectInfo.pid = query.value(0).toString();
//           collectInfo.acTime = query.value(1).toString();
//           collectInfo.cycleTime = query.value(2).toString();
//           collectInfo.count = query.value(3).toString();
//           collectInfo.diastoleArea = query.value(4).toString();
//           collectInfo.contractArea = query.value(5).toString();
//        }
//    }
//    query.finish();
//}

//void acquireImageLogic::insertImageToTemplate(QString& path, QString& ImgPath)
//{
//    // 创建XML文档对象
//    auto xmlPath = path + "/template.xml";
//    auto modifiedPath = path + "/modified.xml";
//    
//    tinyxml2::XMLDocument doc;
//    if (doc.LoadFile(xmlPath.toStdString().c_str()) == tinyxml2::XML_SUCCESS)
//    {
//        qDebug() << u8"加载XML文件成功";
//
//        // 获取根节点
//        tinyxml2::XMLElement* root = doc.RootElement();
//
//        print_child_nodes(root,ImgPath);
// 
//        if (doc.SaveFile(modifiedPath.toStdString().c_str()) != tinyxml2::XML_SUCCESS)
//        {
//            qDebug() << u8"保存失败";
//        }
//    }
//    else
//    {
//        qDebug() << u8"加载XML文件失败";
//    }
//}

//void acquireImageLogic::print_child_nodes(tinyxml2::XMLNode* root, QString& imgPath)
//{
//    for (tinyxml2::XMLNode* child = root->FirstChild(); child != nullptr; child = child->NextSibling()) {
//        // 根据需要处理不同的节点类型
//       if (child->ToElement()) {
//            // 元素节点
//            tinyxml2::XMLElement* element = child->ToElement();
//            const char* name = element->Name();
//            std::string tempStr(name);
//            if (tempStr == "pkg:binaryData")
//            {
//                element->SetText(ImagToBase64(imgPath).toStdString().c_str());
//                return;
//            }
//
//            // 递归遍历子元素
//            print_child_nodes(child, imgPath);
//        }
//    }
//}

//QString acquireImageLogic::ImagToBase64(QString& strImagePath)
//{
//    QImage image(strImagePath);
//    QByteArray ba;
//    QBuffer buf(&ba);
//    image.save(&buf, "png");
//    QString str = ba.toBase64();
//    return str;
//}

void acquireImageLogic::run()
{
    if (m_sType == 100)
    {
        if (cameraCount == 0)
        {
            emit ViewImage(false);
            return;
        }

        cv::VideoCapture cap(cameraIndex);
        auto fps = cap.get(cv::CAP_PROP_FPS);
      
        cap.set(cv::CAP_PROP_FRAME_WIDTH,1280);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT,760);

        srcFrameWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
        srcFrameHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
        
        if (!cap.isOpened())
        {
            qDebug() << "open camere error!";
            emit ViewImage(false);
            return;
        }

        cv::Mat frame;
        int frame_cnt = 0;

        //自动
        while (true)
        {
            cap >> frame;

            if (frame_cnt % 5 == 0)
            {
                if (frame.empty())
                {
                    cv::waitKey();
                    break;
                }
                cv::resize(frame, frame, cv::Size(800, 600), cv::INTER_LINEAR);
                Mutex.lock();
                m_frame = frame.clone();
                Mutex.unlock();

                emit ViewImage(true);
            }
            frame_cnt++;

            if (isRun)
                break;
        }


        if (isRun)
        {
            m_Qmutex_pauseLock.lock();
         
            float scale = 1 / 255.0;
            cv::Scalar mean = cv::Scalar(0.485, 0.456, 0.406) * 255.0; //减去平均值
            bool swapRB = true;//交换RGB BGR 
            int inpWidth = 800;
            int inpHeight = 600;

            std::string writerSegPath = videoPath + "/seg.mp4";
            std::string writerSrcPath = videoPath + "/src.mp4";

            auto videoSegWriter = cv::VideoWriter(writerSegPath, cv::VideoWriter::fourcc('X', '2', '6', '4'), 5, cv::Size(srcFrameWidth, srcFrameHeight), 0);

            if (!videoSegWriter.isOpened()) {
                std::cout << "无法打开视频写入器！" << std::endl;
                return;
            }

            auto videoSrcWriter = cv::VideoWriter(writerSrcPath, cv::VideoWriter::fourcc('X', '2', '6', '4'), 5, cv::Size(srcFrameWidth, srcFrameHeight), 0);

            if (!videoSrcWriter.isOpened()) {
                std::cout << "无法打开视频写入器！" << std::endl;
                return;
            }

            // Process frames.
            cv::Mat frame, blob;

            unsigned int count = 1;
            unsigned int frame_cnt = 0;


            cap >> frame;

            if (!m_pMatchRler->AutoMatchRuler(frame, unit))
            {
                Mutex.lock();
                m_frame = frame.clone();
                Mutex.unlock();
                emit NotMatch();
                this->pauseThread();
            }

            qDebug() << "unit:" << unit;

            while (true)
            {
                cap >> frame;

                if (frame_cnt % 2 == 0)
                {
                    if (frame.empty())
                    {
                        cv::waitKey();
                        break;
                    }

                    videoSrcWriter.write(frame);

                    cv::dnn::blobFromImage(frame, blob, scale, cv::Size(inpWidth, inpHeight), mean, swapRB, false);

                    cv::Scalar std = cv::Scalar(0.229, 0.224, 0.225);

                    if (std.val[0] != 0.0 && std.val[1] != 0.0 && std.val[2] != 0.0)
                    {
                        // Divide blob by std.
                        divide(blob, std, blob);
                    }

                    m_net->setInput(blob);
                    cv::Mat score = m_net->forward();
                    cv::Mat segm;
                    cv::Mat binary = cv::Mat::zeros(score.size[2], score.size[3], CV_8UC1);


                    float thres = 0.60;//知情度

                    //const int chns = score.size[1];//chanl

                    char picName[10];

                   sprintf_s(picName, "%05d.jpg", count);

                    std::string tempPicPath = picPath + "/" + picName;

                    cv::imwrite(tempPicPath, frame);

                    auto area = colorizeSegmentation(score, segm, colors, binary, thres, tempPicPath);
                    //resize(segm, segm, frame.size(), 0, 0, cv::INTER_NEAREST);
                    cv::resize(frame, frame, cv::Size(inpWidth, inpHeight), 0, 0, cv::INTER_CUBIC);

                    addWeighted(frame, 0.5, segm, 0.5, 0.0, frame);

                    std::string label = cv::format("number:%s,area:%f", picName, area);

                    putText(frame, label, cv::Point(0, 15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));

                    Mutex.lock();
                    m_frame = frame.clone();
                    Mutex.unlock();

                    emit doneMat(area);

                    resize(frame, frame, cv::Size(srcFrameWidth, srcFrameHeight), 0, 0, cv::INTER_NEAREST);

                    videoSegWriter.write(frame);

                    if (!isRun)
                    {
                        break;
                    }

                    count++;
                }
                frame_cnt++;
            }
            m_Qmutex_pauseLock.unlock();
            cap.release();
            videoSegWriter.release();
            videoSrcWriter.release();
        }     
    }
    else
    {
        m_Qmutex_pauseLock.lock();

        float scale = 1 / 255.0;
        cv::Scalar mean = cv::Scalar(0.485, 0.456, 0.406) * 255.0; //减去平均值
        bool swapRB = true;//交换RGB BGR 
        int inpWidth = 800;
        int inpHeight = 600;

        // Create a window
        cv::VideoCapture cap;
        cap.open(m_SrcVideoPath.toStdString().c_str());
        srcFrameWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
        srcFrameHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

        std::string writerSegPath = videoPath + "/seg.mp4";
        std::string writerSrcPath = videoPath + "/src.mp4";

        auto videoSegWriter = cv::VideoWriter(writerSegPath, cv::VideoWriter::fourcc('X', '2', '6', '4'), 5, cv::Size(srcFrameWidth, srcFrameHeight), 0);

        if (!videoSegWriter.isOpened()) {
            std::cout << "无法打开视频写入器！" << std::endl;
            return;
        }

        auto videoSrcWriter = cv::VideoWriter(writerSrcPath, cv::VideoWriter::fourcc('X', '2', '6', '4'), 5, cv::Size(srcFrameWidth, srcFrameHeight), 0);

        if (!videoSrcWriter.isOpened()) {
            std::cout << "无法打开视频写入器！" << std::endl;
            return;
        }

        // Process frames.
        cv::Mat frame, blob;

        unsigned int count = 1;
        unsigned int frame_cnt = 0;

        cap >> frame;

        if (!m_pMatchRler->AutoMatchRuler(frame, unit))
        {
            Mutex.lock();
            m_frame = frame.clone();
            Mutex.unlock();
            emit NotMatch();
            this->pauseThread();
        }
        emit Match();
        qDebug() << "unit:" << unit;

        while (true)
        {
            cap >> frame;

            if (frame_cnt % 5 == 0)
            {
                if (frame.empty())
                {
                    cv::waitKey();
                    break;
                }

                videoSrcWriter.write(frame);

                cv::dnn::blobFromImage(frame, blob, scale, cv::Size(inpWidth, inpHeight), mean, swapRB, false);

                cv::Scalar std = cv::Scalar(0.229, 0.224, 0.225);

                if (std.val[0] != 0.0 && std.val[1] != 0.0 && std.val[2] != 0.0)
                {
                    // Divide blob by std.
                    divide(blob, std, blob);
                }

                m_net->setInput(blob);
                cv::Mat score = m_net->forward();
                cv::Mat segm;
                cv::Mat binary = cv::Mat::zeros(score.size[2], score.size[3], CV_8UC1);
          
                float thres = 0.60;//知情度

                //const int chns = score.size[1];//chanl

                char picName[10];

                sprintf_s(picName, "%05d.jpg", count);

                std::string tempPicPath = picPath + "/" + picName;

                cv::imwrite(tempPicPath, frame);
              
                auto area = colorizeSegmentation(score, segm, colors, binary, thres, tempPicPath);
               
                //resize(segm, segm, frame.size(), 0, 0, cv::INTER_NEAREST);
                cv::resize(frame, frame, cv::Size(inpWidth, inpHeight), 0, 0, cv::INTER_CUBIC);

                addWeighted(frame, 0.5, segm, 0.5, 0.0, frame);

                std::string label = cv::format("number:%s,area:%f", picName,area);

                putText(frame, label, cv::Point(0, 15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));

                Mutex.lock();
                m_frame = frame.clone();
                Mutex.unlock();

                emit doneMat(area);

                resize(frame, frame, cv::Size(srcFrameWidth, srcFrameHeight), 0, 0, cv::INTER_NEAREST);

                videoSegWriter.write(frame);

                if (!isRun)         
                    break;

                count++;
            }
            frame_cnt++;
        }
        cap.release();
        videoSegWriter.release();
        videoSrcWriter.release();
        m_Qmutex_pauseLock.unlock();
    }
}

float acquireImageLogic::colorizeSegmentation(const cv::Mat& score, cv::Mat& segm, std::vector<cv::Vec3b> colors, cv::Mat& binary, float thres, std::string& path)
{

    const int rows = score.size[2]; // height
    const int cols = score.size[3]; // width
    const int chns = score.size[1]; // 49

    cv::Mat maxCl = cv::Mat::zeros(rows, cols, CV_8UC1); //储存选定的通道（49分之1）

    cv::Mat scoreMap = cv::Mat::zeros(rows, cols, CV_32FC1);//存label的key

    cv::Mat maxVal(rows, cols, CV_32FC1, score.data);//概率[0,1]

    //建立查找表 49通道类别 0背景，600*800像素
    for (int ch = 1; ch < chns; ch++)//一共49个通道代表49个通道在每个下像素的
    {
        for (int row = 0; row < rows; row++)//600
        {
            const float* ptrScore = score.ptr<float>(0, ch, row); //指向每个通道的第一个位置，存贮的是49个label
            uint8_t* ptrMaxCl = maxCl.ptr<uint8_t>(row);
            float* ptrMaxVal = maxVal.ptr<float>(row);//temp
            float* ptrFinalScore = scoreMap.ptr<float>(row);
            uint8_t* ptrBinary = binary.ptr<uint8_t>(row);

            for (int col = 0; col < cols; col++)//800
            {
                if (ptrScore[col] > ptrMaxVal[col])//49通道的同一个位置取最大值
                {
                    ptrMaxVal[col] = ptrScore[col];
                    ptrMaxCl[col] = (uchar)ch;//最终选择通道
                    ptrFinalScore[col] = ptrScore[col];
                    ptrBinary[col] = (uchar)ch;
                    cls_ids.push_back((uchar)ch);
                }
            }
        }
    }

    //去重
    std::set<uchar>s(cls_ids.begin(), cls_ids.end());
    cls_ids.assign(s.begin(), s.end());

    //上色
    segm.create(rows, cols, CV_8UC3);
    for (int row = 0; row < rows; row++)
    {
        const uchar* ptrMaxCl = maxCl.ptr<uchar>(row);
        cv::Vec3b* ptrSegm = segm.ptr<cv::Vec3b>(row);

        for (int col = 0; col < cols; col++)
        {
            ptrSegm[col] = colors[ptrMaxCl[col]];
        }
    }
     
    //labelme文件
    LabelmeJsonData gastricAntrum;
    LabelmeJsonData leftLobeLiver;
    LabelmeJsonData abdominalAorta;
    gastricAntrum.label = "gastricAntrum";
    leftLobeLiver.label = "leftLobeLiver";
    abdominalAorta.label = "abdominalAorta";

    float areaGastricAntrum = 0;

    resize(binary, binary, cv::Size(srcFrameWidth, srcFrameHeight), 0, 0, cv::INTER_NEAREST);
    resize(scoreMap, scoreMap, cv::Size(srcFrameWidth, srcFrameHeight), 0, 0, cv::INTER_NEAREST);

    for (int lbl : cls_ids) {

        cv::Mat mask = cv::Mat::zeros(binary.size(), CV_8UC1); // 创建一个与pred segmentation大小相同的零矩阵

        compare(binary, lbl, mask, cv::CMP_EQ); // 将mask中lbl与binary相等的位置设置为255 

        cv::Mat score; // 每个分割类别的得分区域二维图

        // 计算掩膜部分数据的总和  
        cv::Scalar s = sum(mask);
        mask.convertTo(mask, CV_32FC1);
        score = scoreMap.mul(mask);
        cv::Scalar logit = sum(score);
        mask.convertTo(mask, CV_8UC1);
        float avg = logit[0] / s[0];

        if (avg < thres) {
            // 小于阈值的分割类别区域自动从结果中去除掉
            //std::cout << "filter remove class id=" << lbl << "predit=" << avg << "\n";
            binary.setTo(cv::Scalar(0), mask);
        }

        // 先通过连通域判断，只保留最大面积的区域
        std::vector<std::vector<cv::Point> > contours;//轮廓点集合
        std::vector<cv::Vec4i> hierarchy;//<int,4> [后，前，子，父]索引编号
        std::vector<cv::Point> point;
        contours.push_back(point);

        cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
        if (contours[0].size() > 0)
        {
            if (lbl == 21)
            {
                gastricAntrum.pointArray = contours[0];
                //gastricAntrum.pointArray.push_back(temp);
                //qDebug() << "contourArea:" << contourArea(contours[0]);
                areaGastricAntrum = contourArea(contours[0]) * pow(1 / unit, 2);
                //qDebug() << "areaGastricAntrum:" << areaGastricAntrum;

            }
            if (lbl == 22)
            {
                leftLobeLiver.pointArray = contours[0];
                //leftLobeLiver.pointArray.push_back(temp);
            }
            if (lbl == 23)
            {
                abdominalAorta.pointArray = contours[0];
               //abdominalAorta.pointArray.push_back(temp);
            }

            labelmeJson(path, &gastricAntrum, &leftLobeLiver, &abdominalAorta, srcFrameHeight, srcFrameWidth);
        }

    }

    //int result = connected_component(binary);

    return areaGastricAntrum;
}

int acquireImageLogic::connected_component(cv::Mat& image)
{
    cv::Mat labels;
    //printf("image size: %d, %d \n", image.size[0], image.size[1]);
    cv::Mat centroids, stats;
    //int num = 0;
    CV_Assert(image.type() == CV_8UC1);
    int num = connectedComponentsWithStats(image, labels, stats, centroids, 8, 4);
    //printf("total labels: %d\n", (num - 1));
    //printf("label size: height=%d, width=%d \n", labels.size[0], labels.size[1]);

    return num;
}
void acquireImageLogic::labelmeJson(std::string& path, LabelmeJsonData* gastricAntrum, LabelmeJsonData* leftLobeLiver, LabelmeJsonData* abdominalAorta, int width, int height)
{
    QJsonObject jsonObject;
    QJsonObject jsonObjectFlags;

    jsonObject.insert("version", QJsonValue("5.4.1"));
    jsonObject.insert("flags", jsonObjectFlags);

    QJsonArray shabelArray;//shapes

    for (size_t i = 0; i < 3; i++)
    {
        QJsonObject jsonObjectNode;//数组对象{}
        QJsonObject jsonObjectFlagsNode;//flags
        QJsonArray pointsArray;//poins 大数组

        if (i == 0)
        {
            for (size_t i = 0; i < gastricAntrum->pointArray.size(); i++)
            {
                QJsonArray point;// x y 坐标数组
                point.append(gastricAntrum->pointArray[i].x);
                point.append(gastricAntrum->pointArray[i].y);
                pointsArray.append(point);
            }
            jsonObjectNode.insert("label", u8"gastricAntrum");
        }

        if (i == 1)
        {
            for (size_t i = 0; i < leftLobeLiver->pointArray.size(); i++)
            {
                QJsonArray point;// x y 坐标数组
                point.append(leftLobeLiver->pointArray[i].x);
                point.append(leftLobeLiver->pointArray[i].y);
                pointsArray.append(point);
            }
            jsonObjectNode.insert("label", u8"leftLobeLiver");
        }

        if (i == 2)
        {

            for (size_t i = 0; i < abdominalAorta->pointArray.size(); i++)
            {
                QJsonArray point;// x y 坐标数组
                point.append(abdominalAorta->pointArray[i].x);
                point.append(abdominalAorta->pointArray[i].y);
                pointsArray.append(point);
            }
            jsonObjectNode.insert("label", u8"abdominalAorta");
        }

        jsonObjectNode.insert("points", pointsArray);
        jsonObjectNode.insert("group_id", QJsonValue());
        jsonObjectNode.insert("shape_type", "polygon");
        jsonObjectNode.insert("flags", jsonObjectFlagsNode);


        shabelArray.append(jsonObjectNode);
    }

    jsonObject.insert("shapes", shabelArray);

    //处理path
    std::string jpgName = path.substr(path.find_last_of("/") + 1, 20);
    std::string jsonName = jpgName.substr(0, 6) + "json";
    std::string jsonPath = path.substr(0, path.size() - 9) + jsonName;

    jsonObject.insert("imagePath", jpgName.c_str());
    jsonObject.insert("imageData", QJsonValue());
    jsonObject.insert("imageHeight", QJsonValue(height));
    jsonObject.insert("imageWidth", QJsonValue(width));

    QJsonDocument jsonDoc(jsonObject);

    QFile file(jsonPath.c_str());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "can't open error!";
        return;
    }
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << jsonDoc.toJson();
    
    file.close();
}

AnalysisLogic::AnalysisLogic(QString picPath, QObject* parent)
{
    isRun = false;
    this->picPath = picPath + "/pic";
    this->writerSegPath = picPath.toStdString() + "/video/seg.mp4";
    this->unit = unit;
}

AnalysisLogic::~AnalysisLogic()
{
}

void AnalysisLogic::SetIsRun(bool b)
{
    isRun = b;
}

bool AnalysisLogic::GetIsRun()
{
    return isRun;
}

void AnalysisLogic::run()
{
    if (!std::filesystem::exists(picPath.toStdString()))
        return;

    QVector<std::string> jsonFiles;
    //读取文件
    for (const auto& entry : std::filesystem::recursive_directory_iterator(picPath.toStdString())) {
         auto filePath = entry.path().string();
         auto filenameStr = entry.path().filename().string();
         auto extension = entry.path().extension();
       if (entry.is_regular_file()) {
            
            if (extension == ".json")
            {
                jsonFiles.append(filePath);
            }
      
        }
    }
    cv::Mat matSize;
    std::string path;
    if (jsonFiles.size() > 0)
    {
        path = jsonFiles[0].substr(0, jsonFiles[0].find_last_of("\\") + 7) + "jpg";
    }
    else
    {
        emit Finish();
        qDebug() << "not have json files!";
        return;
    }
    

    matSize = cv::imread(path);
    qDebug() << QString::fromStdString(path);
    auto videoSegWriter = cv::VideoWriter(writerSegPath, cv::VideoWriter::fourcc('X', '2', '6', '4'), 5, cv::Size(matSize.size()), 0);

    if (!videoSegWriter.isOpened()) {
        std::cout << "无法打开视频写入器！" << std::endl;
        return;
    }

    for (size_t i = 0; i < jsonFiles.size(); i++)
    {
        //解析JSON 重画图像 计算面积 保存截图和excel
        auto tempStr = jsonFiles[i].substr(0, jsonFiles[i].find_last_of("\\") + 7) + "jpg";
        qDebug() << QString::fromStdString(tempStr);

        cv::Mat mat = cv::imread(tempStr);
        if (mat.empty())
            continue;

        float area = 0.0f;

        std::vector<std::vector<cv::Point>> contours;

        std::vector<cv::Point> vecPoints;

        analysisJson(jsonFiles[i], vecPoints);

        qDebug() << "tempStr"<< QString::number( vecPoints.size());

        if (vecPoints.empty())
        {
            continue;
        }

        contours.push_back(vecPoints);

        area = cv::contourArea(contours[0]) * pow(1 / unit, 2);
      
        cv::Scalar color( 0,0, 255);

        //cv::drawContours(mat, contours, 0, color, CV_FILLED);

        cv::drawContours(mat, contours, 0, color, CV_FILLED);

        videoSegWriter.write(mat);

        mutex.lock();
        cv::resize(mat, m_frame, cv::Size(800, 600), CV_INTER_CUBIC);
        //m_frame = mat.clone();
        mutex.unlock();

        emit AnalysisMat(area);

       /* cv::imshow("123",mat);

        cv::waitKey(1);*/
    }

    videoSegWriter.release();

    //完成后 解决计算信号
    emit Finish();
}

void AnalysisLogic::analysisJson(std::string jsonPath, std::vector<cv::Point>& vecPoints)
{
    QFile file(jsonPath.c_str());
    if (file.open(QFile::ReadOnly)) 
    {
        QByteArray all = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(all);
        if (doc.isObject())
        {
            QJsonObject obj = doc.object();
            QJsonValue shapes =  obj.value("shapes");
            if (shapes.isArray())
            {
                QJsonArray shapesArray = shapes.toArray();
                for (size_t i = 0; i < shapesArray.size(); i++)
                {
                    QJsonObject subObj = shapesArray[i].toObject();
                    if (subObj.value("label").toString() == "gastricAntrum")
                    {
                        QJsonArray pointsArray = subObj.value("points").toArray();
                        for (size_t j = 0; j < pointsArray.size(); j++)
                        {
                            QJsonArray points = pointsArray[j].toArray();
                            if(points.size() >= 2)
                            {
                                cv::Point tempPoint;
                                tempPoint.x = points[0].toDouble();
                                tempPoint.y = points[1].toDouble();
                                vecPoints.push_back(tempPoint);
                            }
                        }
                    }
                }
            }
        }
    }
    file.close();
}

cv::Mat& AnalysisLogic::GetMat()
{
    QMutexLocker locker(&mutex);
    return m_frame;
}

void AnalysisLogic::SetUnit(float u)
{
    unit = u;
}
