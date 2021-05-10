#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qtserialCom.h"

#include <thread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "serialthread.h"
#include "helpDialog.h"
#include <QTimer>
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
	void initcom();
	//接收模式 normal 一次全部接收；fixsize 接收固定长度；length_bit 按协议接收位定义的长度接收
	enum com_type {
		normal,//一次全部接收
		fixsize,//接收固定长度
		length_bit//按协议接收位定义的长度接收
	};
	com_type com_type1= com_type::normal;
	// 定时发送-定时器
	QTimer *timSend=nullptr;
public slots:
	void readcom(QByteArray comdata);
private:
    Ui::qtserialComClass ui;
	helpDialog *helpdialog;
	bool read_by_hex = false;//是否转换为十六进制
	//QSerialPort *serial;

};
