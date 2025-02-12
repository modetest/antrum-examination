#pragma once

#include <qdialog.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <QPixmap>
#include <qbitmap.h>

class UMessageBox : public QDialog
{
	Q_OBJECT
public:
	UMessageBox(QString text,QWidget* parent = nullptr);
	~UMessageBox();
	
};

