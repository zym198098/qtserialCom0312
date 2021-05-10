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
	bool com_start=false;//���ڴ򿪱�־
	void open();
	void stop();
	void Execute();
	//����ģʽ normal һ��ȫ�����գ�fixsize ���չ̶����ȣ�length_bit ��Э�����λ����ĳ��Ƚ���
	enum get_type {
		normal,//һ��ȫ������
		fixsize,//���չ̶�����
		length_bit//��Э�����λ����ĳ��Ƚ���
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
	//�����Ƿ񰴳��Ƚ���
	void set_get_by_length(bool Fixed_by_length);
	//����Э����յ����ݳ���λ���ڵڼ�λ
	void set_length_bit(unsigned short  length_bit_at);
	//���ö������գ�һ�ν��յ��ֽ���
	void set_get_fixsize(unsigned int fix_size);
	//���ý���ģʽ
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
//����ģʽ Ĭ��Ϊȫ������
get_type type = normal;
//��������
bool Fixed_by_length = false;//�Ƿ�λ����
unsigned short length_bit_at = 3;//Ĭ�� Э�鴫�� ���ݳ����� �ڶ�λ ���ճ��ȶ����λ
//�̶����ճ���
unsigned fix_size = 6;
int fix_rec_size = 0;//���� fix_rec_size ��С���գ�������Э��ڶ�λΪ���� ����
};
