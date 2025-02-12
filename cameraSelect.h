#pragma once

#include <QDialog>
#include <QBoxLayout>
#include <Qlabel>
#include <QCamera>
#include <QCameraInfo>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>

class cameraSelect  : public QDialog
{
	Q_OBJECT

public:
	cameraSelect(QWidget *parent);
	~cameraSelect();

public slots:
	void OnOKBtn();
private:
	QButtonGroup* m_group;

};
