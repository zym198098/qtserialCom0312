#include "qtserialCom.h"
#include <QTimer>
#pragma execution_character_set("utf-8")
qtserialCom::qtserialCom(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	helpdialog = new helpDialog(this);
	ui.btn_close->setEnabled(false);
	initSerialPortSetting();//初始化
	//设置波特率下拉菜单默认显示第一项
	ui.BaudBox->setCurrentIndex(0); // 115200
	serial_com = new serialthread(this);
	com_type1 = com_type(ui.comboBox->currentIndex());
	connect(ui.comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [=](int index) {
		com_type1 = com_type(index);
	});
	//connect(serial_com, &serial_com->readCom, this, &qtserialCom::readcom);
	connect(serial_com, SIGNAL(readCom(QByteArray)), this, SLOT(readcom(QByteArray)));

	connect(ui.btn_open, &QPushButton::clicked, this, [=]()
	{

		if (ui.btn_open->text() == "open")
		{
			
			
			if (serial_com->serial == nullptr)
			{
				return;
			}
			if (serial_com->com_start)
			{
				return;
			}
			initcom();
			serial_com->open();
			statusBar()->clearMessage();

			if (serial_com->com_start)
			{
				statusBar()->showMessage("com open " + serial_com->serial->portName(), 3000);
				ui.btn_open->setEnabled(false);
				ui.btn_close->setEnabled(true);
			}
			else
			{
				statusBar()->showMessage("com open " + serial_com->serial->portName() + " erro", 3000);
			}

		}

		
	});
	connect(ui.btn_close, &QPushButton::clicked, this, [=]()
	{
		if (serial_com->serial == nullptr)
		{
			return;
		}
		if (!serial_com->com_start)
		{
			return;
		}
		serial_com->stop();
		statusBar()->clearMessage();
		statusBar()->showMessage("com close:"+serial_com->serial->portName(), 3000);
		ui.btn_open->setEnabled(true);
		ui.btn_close->setEnabled(false);
	});
	connect(ui.btn_send, &QPushButton::clicked, this, [=]()
	{
		if (serial_com->serial == nullptr)
		{
			return;
		}
		if (!serial_com->com_start)
		{
			return;
		}
		if (ui.textEdit_send->toPlainText().length())
		{
			QByteArray sendhex = ui.textEdit_send->toPlainText().toLatin1();	
			if (ui.cb_send_hex->isChecked())
			{
				sendhex = QString2Hex(ui.textEdit_send->toPlainText());
			}
			serial_com->writeData(sendhex);
			statusBar()->clearMessage();
			statusBar()->showMessage("com write", 200);
		}
	});
	connect(ui.pushButton_2, &QPushButton::clicked, this, [=]()
	{
		ui.textEdit_send->clear();
	});
	connect(ui.pushButton_3, &QPushButton::clicked, this, [=]()
	{
		ui.textBrowser->clear();
	});
	connect(ui.cb_read_hex, &QCheckBox::stateChanged, this, [=]() {
		read_by_hex = ui.cb_read_hex->isChecked();
	});
	connect(ui.actionhelp, &QAction::triggered, this, [=]() {
		
		helpdialog->show();
	});
	
	connect(ui.chbox_timer, &QCheckBox::stateChanged, this, [=](int state)
	{
		
		switch (state)
		{
		case 2:
			timSend->setInterval(ui.spinBox_ms->value());
			timSend->setTimerType(Qt::PreciseTimer);
			timSend->start();
			connect(timSend, &QTimer::timeout, this, [=]() {
				ui.btn_send->click();
				//timSend->start();
			});
			break;
		case 0:
			timSend->stop();
			break;
		default:
			break;
		}
	});

}

char qtserialCom::ConvertHexChar(char ch)
{
	if ((ch >= '0') && (ch <= '9'))
		return ch - 0x30;
	else if ((ch >= 'A') && (ch <= 'F'))
		return ch - 'A' + 10;
	else if ((ch >= 'a') && (ch <= 'f'))
		return ch - 'a' + 10;
	else return (-1);
}

QByteArray qtserialCom::QString2Hex(QString str)
{
	QByteArray senddata;
	int hexdata, lowhexdata;
	int hexdatalen = 0;
	int len = str.length();
	senddata.resize(len / 2);
	char lstr, hstr;
	for (int i = 0; i < len; )
	{
		hstr = str[i].toLatin1();
		if (hstr == ' ')
		{
			i++;
			continue;
		}
		i++;
		if (i >= len)
			break;
		lstr = str[i].toLatin1();
		hexdata = ConvertHexChar(hstr);
		lowhexdata = ConvertHexChar(lstr);
		if ((hexdata == 16) || (lowhexdata == 16))
			break;
		else
			hexdata = hexdata * 16 + lowhexdata;
		i++;
		senddata[hexdatalen] = (char)hexdata;
		hexdatalen++;
	}
	senddata.resize(hexdatalen);
	return senddata;
}

void qtserialCom::initcom()
{
	if (timSend == nullptr)
	{
		timSend = new QTimer();
	}
	serial_com->setport(ui.PortBox->currentText());
	serial_com->setBaudRate(baudRate[ui.BaudBox->currentIndex()]);
	serial_com->setDataBits(dataBits[ui.dataBits->currentIndex()]);
	serial_com->setParity(parity[ui.parity->currentIndex()]);
	serial_com->setStopBits(stopBits[ui.stopBits->currentIndex()]);
	switch (com_type1)
	{
	case qtserialCom::normal:
		serial_com->set_get_type(serialthread::get_type::normal);
		break;
	case qtserialCom::fixsize:
		serial_com->set_get_type(serialthread::get_type::fixsize);
		serial_com->set_get_fixsize(ui.read_bit->value());
		break;
	case qtserialCom::length_bit:
		serial_com->set_get_type(serialthread::get_type::length_bit);
		serial_com->set_length_bit(ui.spin_size_in->value());
		break;
	default:
		serial_com->set_get_type(serialthread::get_type::normal);
		break;
	}
}

void qtserialCom::readcom(QByteArray comdata)
{ 
	unsigned char userid[6] = { 0 };
	for (int i = 0; i < comdata.size(); i++)
	{
		if (i == 12)
		{
			break;
		}
		if (i > 4 && i < 11) {
			userid[i-5] = (unsigned char)comdata.at(i);
		}
		
	}
	QByteArray dataHex = comdata;
	if (read_by_hex)
	{
		 dataHex = comdata.toHex();
	}
	
	QString data;
	int s = dataHex.size();
	int s1 = comdata.size();
	for (int i = 0; i < dataHex.size() ; i++)
	{
		data += dataHex.at(i);
	}
	data += "\n";
	ui.textBrowser->insertPlainText(data);
	data.clear();
	statusBar()->clearMessage();
	statusBar()->showMessage("com read", 200);
	//ui.textBrowser->setText(data);

}
void qtserialCom::initSerialPortSetting()
{
	
	/* Insert the available serial ports into QComboBox. Keep infolist and ui->serialPortNumber in same order.*/
	infolist = QSerialPortInfo::availablePorts();
	foreach(const QSerialPortInfo& info, infolist)
	{
		ui.PortBox->addItem(info.portName());
	}
	if (ui.PortBox->count() == 0)
	{
		ui.PortBox->addItem(tr("NULL"));
		ui.btn_open ->setEnabled(false);
	}

	/* Insert choices of baud rate into QComboBox. Keep this->baudRate and baudRate in same order. */
	this->baudRate << QSerialPort::Baud115200 << QSerialPort::Baud57600 << QSerialPort::Baud38400
		<< QSerialPort::Baud19200 << QSerialPort::Baud9600 << QSerialPort::Baud4800
		<< QSerialPort::Baud2400 << QSerialPort::Baud1200;
	QStringList baudRate;
	baudRate << "115200" << "57600" << "38400"
		<< "19200" << "9600" << "4800"
		<< "2400" << "1200";
	ui.BaudBox->addItems(baudRate);

	/* Insert choices of data bits into QComboBox. Keep this->dataBits and dataBits in same order. */
	this->dataBits << QSerialPort::Data8 << QSerialPort::Data7
		<< QSerialPort::Data6 << QSerialPort::Data5;
	QStringList dataBits;
	dataBits << tr("8") << tr("7") << tr("6") << tr("5");
	ui.dataBits->addItems(dataBits);

	/* Insert choices of parity into QComboBox. Keep this->parity and parity in same order. */
	this->parity << QSerialPort::NoParity << QSerialPort::OddParity << QSerialPort::EvenParity
		<< QSerialPort::SpaceParity << QSerialPort::MarkParity;
	QStringList parity;
	parity << QString("无") << QString("奇偶校验") << QString("偶校验") << QString("空") << QString("标记");
	ui.parity->addItems(parity);

	/* Insert choices of stop bits into QComboBox. Keep this->stopBits and stopBits in same order. */
	this->stopBits << QSerialPort::OneStop << QSerialPort::OneAndHalfStop << QSerialPort::TwoStop;
	QStringList stopBits;
	stopBits << tr("1") << tr("1.5") << tr("2");
	ui.stopBits->addItems(stopBits);
}