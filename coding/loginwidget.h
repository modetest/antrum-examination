#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QStyleOption>
#include <QPainter>

#include <QKeyEvent>

#include "loginLogic.h"
#include "UltrasoundArtificialIntelligence.h"


namespace Ui {
class loginWidget;
}

class loginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit loginWidget(QWidget *parent = nullptr);
    ~loginWidget();
protected:
    void paintEvent(QPaintEvent*);//–ﬁ∏ƒ±≥æ∞


public slots:
    void OnEyePassword();
    void OnLoginBtn();

    void keyPressEvent(QKeyEvent* event);

signals:
    void JumpMainWidget();

private:
    Ui::loginWidget *ui;
    loginLogic m_logic;
    bool m_pwd;//øÿ÷∆√‹¬Îœ‘ æ
};

#endif // LOGINWIDGET_H
