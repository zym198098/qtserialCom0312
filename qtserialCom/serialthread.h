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
	bool com_start=false;//���ڴ򿪱�־
	void open();
	void stop();
	void Execute();
	//��������
	bool Fixed_length = false;//�Ƿ�λ����
	int length_num = 2;//Ĭ�� Э�鴫�� ���ݳ����� �ڶ�λ
	int fix_rec_size = 0;//���� fix_rec_size ��С���գ�������Э��ڶ�λΪ���� ����
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
