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
	serialthread(QObject *parent, QString portname1,QSerialPort::BaudRate BaudRate1 ,  QSerialPort::StopBits stopBits1 = QSerialPort::OneStop, QSerialPort::Parity party1 = QSerialPort::NoParity, QSerialPort::DataBits databits1= QSerialPort::Data8);
	~serialthread();
	bool com_start=false;//串口打开标志
	void open();
	void stop();
	void Execute();
	//接收模式 normal 一次全部接收；fixsize 接收固定长度；length_bit 按协议接收位定义的长度接收
	enum get_type {
		normal,//一次全部接收
		fixsize,//接收固定长度
		length_bit//按协议接收位定义的长度接收
	};
	
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
	//设置是否按长度接收
	void set_get_by_length(bool Fixed_by_length);
	//设置协议接收的数据长度位，在第几位
	void set_length_bit(unsigned short  length_bit_at);
	//设置定长接收，一次接收的字节数
	void set_get_fixsize(unsigned int fix_size);
	//设置接收模式
	void set_get_type(get_type type);
	void serial_sleep(unsigned int ms);
public:
signals:
void readCom(QByteArray comdata);
private:
QString portname="com3";
QSerialPort::BaudRate BaudRate= QSerialPort::Baud115200;
QSerialPort::StopBits stopBits= QSerialPort::OneStop;
QSerialPort::Parity Parity = QSerialPort::NoParity;
QSerialPort::DataBits databits= QSerialPort::Data8;
//接收模式 默认为全部接收
get_type type = normal;
//定长接收
bool Fixed_by_length = false;//是否按位接收
unsigned short length_bit_at = 3;//默认 协议传输 数据长度在 第二位 接收长度定义的位
//固定接收长度
unsigned fix_size = 6;
int fix_rec_size = 0;//按照 fix_rec_size 大小接收；否则按照协议第二位为长度 接收
};
