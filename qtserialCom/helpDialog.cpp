#include "helpDialog.h"

helpDialog::helpDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.textBrowser->setSource(QUrl("qrc:/qtserialCom/help.htm"));
}

helpDialog::~helpDialog()
{
}
