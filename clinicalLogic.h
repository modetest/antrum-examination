#pragma once
#include <qobject.h>
#include <qmap.h>

#include "Data.h"

class clinicalLogic :
    public QObject
{
    Q_OBJECT
public:
    clinicalLogic(QObject* parent = nullptr);
    ~clinicalLogic();

    void insert(const QString& title, breathe& data);
    void insert(const QString& title, medicine& data,const BreatheOrMedicType type);
    void remove(const QString& title, const BreatheOrMedicType type);
    bool isHave(const QString& title, const BreatheOrMedicType type);
    int size(const BreatheOrMedicType type);

    void GetHreatheData(QVector<breathe>& vec);
    
    void GetVasoactivityData(QVector<medicine>& vec);
    void GetAnalgeiaData(QVector<medicine>& vec);
    void GetCalmData(QVector<medicine>& vec);
    void GetMuscleRelaxationData(QVector<medicine>& vec);

    void checkBreathZero(breathe& data);
    void checkMediantZero(medicine& data);

private:
    QMap<QString, breathe> m_MaphreathData;
    QMap<QString, medicine> m_MapVasoactivityData;
    QMap<QString, medicine> m_MapAnalgeiaData;
    QMap<QString, medicine> m_MapCalmData;
    QMap<QString, medicine> m_MapMuscleRelaxationData;
};

