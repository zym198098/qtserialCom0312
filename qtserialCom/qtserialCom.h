#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qtserialCom.h"

#include <thread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "serialthread.h"
#include "helpDialog.h"
using namespace std;
class qtserialCom : public QMainWindow
{
    Q_OBJECT

public:
	void initSerialPortSetting();
    qtserialCom(QWidget *parent = Q_NULLPTR);
	
	serialthread *serial_com;
	QList<QSerialPortInfo> infolist;
	QList<QSerialPort::BaudRate> baudRate;
	QList<QSerialPort::DataBits> dataBits;
	QList<QSerialPort::Parity> parity;
	QList<QSerialPort::StopBits> stopBits;
	char ConvertHexChar(char ch);

	QByteArray QString2Hex(QString str);
	

public slots:
	void readcom(QByteArray comdata);
private:
    Ui::qtserialComClass ui;
	helpDialog *helpdialog;
	bool read_by_hex = false;//是否转换为十六进制
	//QSerialPort *serial;

};
