#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qtserialCom.h"

#include <thread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "serialthread.h"
#include "helpDialog.h"
#include <QTimer>
#include <queue>
#include<vector>
using namespace std;
class qtserialCom : public QMainWindow
{
    Q_OBJECT

public:
	void initSerialPortSetting();
    qtserialCom(QWidget *parent = Q_NULLPTR);
	~qtserialCom();
	serialthread *serial_com;
	std::thread t1;
	
	void t1_Execute();
	bool write_com = false;
	bool t1_quit = false;
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
	unsigned long line_num = 0;
public slots:
	void readcom(QByteArray comdata);
signals:
	void writeCom_ser(QByteArray comdata);
private:
    Ui::qtserialComClass ui;
	helpDialog *helpdialog;
	bool read_by_hex = false;//�Ƿ�ת��Ϊʮ������
	//QSerialPort *serial;

};
