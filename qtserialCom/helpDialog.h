#pragma once

#include <QObject>
#include <QDialog>
#include "ui_Dialoghelp.h"
class helpDialog :public  QDialog
{
	Q_OBJECT

public:
	helpDialog(QWidget *parent);
	~helpDialog();
private:
	Ui::Dialog ui;
};
