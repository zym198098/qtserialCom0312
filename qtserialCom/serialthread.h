#pragma once

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <thread>
using namespace std;
class serialthread : public QObject
{
	Q_OBJECT

public:
	serialthread(QObject *parent);
	serialthread(QObject *parent, QString portname1,QSerialPort::BaudRate BaudRate1,  QSerialPort::StopBits stopBits1, QSerialPort::Parity party1, QSerialPort::DataBits databits1);
	~serialthread();
	bool com_start=false;//串口打开标志
	void open();
	void stop();
	void Execute();
	//定长接收
	bool Fixed_length = false;//是否按位接收
	int length_num = 2;//默认 协议传输 数据长度在 第二位
	int fix_rec_size = 0;//按照 fix_rec_size 大小接收；否则按照协议第二位为长度 接收
	std::thread serial_thread;
	QSerialPort *serial=nullptr;
	void setport(QString portname1);
	void setBaudRate(QSerialPort::BaudRate BaudRate1);
	void setStopBits(QSerialPort::StopBits stopBits1);
	void setParity(QSerialPort::Parity Parity1);
	void setDataBits(QSerialPort::DataBits databits1);
	void writeData(const QByteArray &data);
	void writeData(const char * data);
	void writeData(const char * data,int length);
public:
signals:
void readCom(QByteArray comdata);
private:
QString portname="com5";
QSerialPort::BaudRate BaudRate= QSerialPort::Baud115200;
QSerialPort::StopBits stopBits= QSerialPort::OneStop;
QSerialPort::Parity Parity = QSerialPort::NoParity;
QSerialPort::DataBits databits= QSerialPort::Data8;

};
