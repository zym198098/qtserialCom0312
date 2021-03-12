#include "qtserialCom.h"

#pragma execution_character_set("utf-8")
qtserialCom::qtserialCom(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	helpdialog = new helpDialog(this);
	ui.btn_close->setEnabled(false);
	////���ҿ��õĴ���
	//foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	//{
	//	QSerialPort serial;
	//	serial.setPort(info);
	//	if (serial.open(QIODevice::ReadWrite))
	//	{
	//		ui.PortBox->addItem(serial.portName());
	//		serial.close();
	//	}
	//}
	initSerialPortSetting();//��ʼ��
	//���ò����������˵�Ĭ����ʾ��һ��
	ui.BaudBox->setCurrentIndex(0); // 115200
	serial_com = new serialthread(this);
	
	
	//connect(serial_com, &serial_com->readCom, this, &qtserialCom::readcom);
	connect(serial_com, SIGNAL(readCom(QByteArray)), this, SLOT(readcom(QByteArray)));
	connect(ui.btn_open, &QPushButton::clicked, this, [=]()
	{

		if (ui.btn_open->text() == "open")
		{
			serial_com->setport(ui.PortBox->currentText());
			serial_com->setBaudRate(baudRate[ui.BaudBox->currentIndex()]);
			serial_com->setDataBits(dataBits[ui.dataBits->currentIndex()]);
			serial_com->setParity(parity[ui.parity->currentIndex()]);
			serial_com->setStopBits(stopBits[ui.stopBits->currentIndex()]);
			serial_com->Fixed_length = ui.ck_fix_lenth->isChecked();
			if (serial_com->Fixed_length)
			{
				serial_com->length_num = ui.spin_size_in->value()+1;
			}
			serial_com->fix_rec_size = ui.read_bit->value();
			if (serial_com->serial == nullptr)
			{
				return;
			}
			if (serial_com->com_start)
			{
				return;
			}

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
	parity << QString("��") << QString("��żУ��") << QString("żУ��") << QString("��") << QString("���");
	ui.parity->addItems(parity);

	/* Insert choices of stop bits into QComboBox. Keep this->stopBits and stopBits in same order. */
	this->stopBits << QSerialPort::OneStop << QSerialPort::OneAndHalfStop << QSerialPort::TwoStop;
	QStringList stopBits;
	stopBits << tr("1") << tr("1.5") << tr("2");
	ui.stopBits->addItems(stopBits);
}