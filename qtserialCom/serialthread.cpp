#include "serialthread.h"
#include <thread>
#include<QTimer>
using namespace std;
serialthread::serialthread(QObject *parent)
	: QObject(parent)
{
	serial = new QSerialPort();
	serial->setPortName(portname);
	serial->setBaudRate(BaudRate);
	serial->setStopBits(stopBits);
	serial->setParity(Parity);
	
}
serialthread::serialthread(QObject *parent, QString portname1, QSerialPort::BaudRate BaudRate1, QSerialPort::StopBits stopBits1, QSerialPort::Parity Parity1, QSerialPort::DataBits databits1) : QObject(parent)
{
	serial = new QSerialPort();
	serial->setPortName(portname1);
	serial->setBaudRate(BaudRate1);
	serial->setStopBits(stopBits1);
	serial->setParity(Parity1);
	
}
serialthread::~serialthread()
{
	if (com_start)
	{
		stop();
	}
	if (serial != nullptr)
	{
		delete serial;
		serial = nullptr;
	}
}

void serialthread::open()
{
	
	com_start=serial->open(QSerialPort::ReadWrite);
	if (com_start)
	{
		connect(serial, &QSerialPort::readyRead, this, &serialthread::read_data);

	}
		

	
}

void serialthread::stop()
{
	if (com_start)
	{
		com_start = false;
		serial->close();
	}
	
}


void serialthread::setport(QString portname1)
{
	serial->setPortName(portname1);
}

void serialthread::setBaudRate(QSerialPort::BaudRate BaudRate1)
{
	serial->setBaudRate(BaudRate1);
}

void serialthread::setStopBits(QSerialPort::StopBits stopBits1)
{
	serial->setStopBits(stopBits1);
}

void serialthread::setParity(QSerialPort::Parity Parity1)
{
	serial->setParity(Parity1);
}

void serialthread::setDataBits(QSerialPort::DataBits databits1)
{
	serial->setDataBits(databits1);
}

qint64 serialthread::writeData(const QByteArray &data)
{
	if (serial == nullptr)
	{
		return 0;

	}
	return serial->write(data);
}

void serialthread::writeData(const char * data)
{
	if (serial == nullptr)
	{
		return;

	}
	serial->write(data);
	
}

qint64 serialthread::writeData(const char * data, int length)
{
	if (serial == nullptr)
	{
		return 0;

	}
	return serial->write(data,length);
}

void serialthread::set_get_by_length(bool Fixed_by_length)
{
	this->Fixed_by_length = Fixed_by_length;
}

void serialthread::set_length_bit(unsigned short length_bit_at)
{
	this->length_bit_at = length_bit_at;
}

void serialthread::set_get_fixsize(unsigned int fix_size)
{
	this->fix_size = fix_size;
	//serial->setReadBufferSize(fix_size);
}

void serialthread::set_get_type(get_type type)
{
	this->type = type;
}

void serialthread::serial_sleep(unsigned int ms)
{
	std::chrono::milliseconds dura(ms);
	std::this_thread::sleep_for(dura);
}

void serialthread::read_data()
{

	static QByteArray sRecvTotal;
	static QByteArray length_bit_data;//��λ����
	QByteArray data;
	static int readnum = 0;//ѭ������ȡ10��
	//static QByteArray length_bit_data;
	int read_length;//���ݱ�־λ���ȡ�����ݳ���
	  static int left = 0;//����Э�鳤��
		switch (type)
		{
		case get_type::normal://��ͨ��ȡģʽ
			data = serial->readAll();
			if (data.size() == 0)
			{
				serial_sleep(1);
				break;
			}
			emit readCom(data);
			data.clear();
			break;
		case get_type::fixsize://�̶���С��ȡ
			
		{    int canread = serial->bytesAvailable();
				QByteArray data1 = serial->read(canread);
				sRecvTotal += data1;
				left = sRecvTotal.size();
				if (sRecvTotal.size() >= fix_size)
				{
					
					QByteArray send;
					for (int i=0;i<fix_size;i++)
					{
						send.append(sRecvTotal[0]);
						sRecvTotal.remove(0, 1);
						left = sRecvTotal.size();
					}
					sRecvTotal.resize(left);
					emit readCom(send);
					//sRecvTotal.clear();
					break;

				}
				else
				{

				}

			}
		
			break;
		case get_type::length_bit://��������λ��ȡ
			//while (!serial->atEnd())//���ݳ��ȴ������ݱ�־λʱ��ʼ��ȡ
			{
			length_bit_data += serial->readAll();//�״ζ�ȡ
			if (length_bit_data.size() < length_bit_at)//���û�ж�ȡ����־λ
			{
				break;//������ȡ
			}
			if (length_bit_data.size() == (length_bit_at + length_bit_data[length_bit_at - 1]))//�������һ���Զ�ȡ���
			{
				emit readCom(length_bit_data);
				length_bit_data.clear();
				length_bit_data.resize(0);

			}
			else//�����ȡ����־λ ������û������
			{
				read_length = length_bit_data[length_bit_at - 1];//��ȡ���ݱ�־λ֮��Ӧ�ö�ȡ�ĳ���
				int totle = length_bit_data.size();//��ǰ�Ѷ�ȡ����
				int read_len1 = read_length + length_bit_at - totle;//ʣ���ȡ����
				length_bit_data += serial->readAll(); //read(read_len1);//�ٴζ�ȡʣ�೤��

				if (length_bit_data.size() >=(length_bit_at + read_length))//����ﵽ�򳬹��涨�����ݳ���
				{

					left = length_bit_data.size();
					QByteArray send;
					for (int i = 0; i < (length_bit_at + read_length); i++)//���������Ƚ�ȡ,ʣ�ಿ�ּ���������length_bit_data
					{
						send.append(length_bit_data[0]);
						length_bit_data.remove(0, 1);
						left = length_bit_data.size();

					}
					length_bit_data.resize(left);
					emit readCom(send);

					//length_bit_data.resize(left);
					break;
				}

			}
				
			}
		
			break;
		default:
			data = serial->readAll();
			if (data.size() == 0)
			{
				serial_sleep(1);
				break;
			}
			emit readCom(data);
			data.clear();
			break;
		}




	
}

