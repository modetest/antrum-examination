#pragma once

#include <QtWidgets/QWidget>
#include "ui_antrum.h"

class antrum : public QWidget
{
    Q_OBJECT

public:
    antrum(QWidget *parent = nullptr);
    ~antrum();

private:
    Ui::antrumClass ui;
};
