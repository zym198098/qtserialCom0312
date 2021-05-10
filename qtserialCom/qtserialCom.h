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
	//����ģʽ normal һ��ȫ�����գ�fixsize ���չ̶����ȣ�length_bit ��Э�����λ����ĳ��Ƚ���
	enum com_type {
		normal,//һ��ȫ������
		fixsize,//���չ̶�����
		length_bit//��Э�����λ����ĳ��Ƚ���
	};
	com_type com_type1= com_type::normal;
	// ��ʱ����-��ʱ��
	QTimer *timSend=nullptr;
public slots:
	void readcom(QByteArray comdata);
private:
    Ui::qtserialComClass ui;
	helpDialog *helpdialog;
	bool read_by_hex = false;//�Ƿ�ת��Ϊʮ������
	//QSerialPort *serial;

};
