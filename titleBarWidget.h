#ifndef TITLEBARWIDGET_H
#define TITLEBARWIDGET_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class titleBarWidget;
}

class titleBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit titleBarWidget(QWidget *parent = nullptr);
    ~titleBarWidget();

public slots:
    void OnSetPushButton();
private:
    static titleBarWidget* instance;
    Ui::titleBarWidget *ui;
    QPushButton* setPushButton;
};

#endif // TITLEBARWIDGET_H
