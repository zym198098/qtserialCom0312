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
	static QByteArray length_bit_data;//按位接收
	QByteArray data;
	static int readnum = 0;//循环最多读取10次
	//static QByteArray length_bit_data;
	int read_length;//数据标志位后读取的数据长度
	  static int left = 0;//超出协议长度
		switch (type)
		{
		case get_type::normal://普通读取模式
			data = serial->readAll();
			if (data.size() == 0)
			{
				serial_sleep(1);
				break;
			}
			emit readCom(data);
			data.clear();
			break;
		case get_type::fixsize://固定大小读取
			
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
		case get_type::length_bit://根据数据位读取
			//while (!serial->atEnd())//数据长度大于数据标志位时开始读取
			{
			length_bit_data += serial->readAll();//首次读取
			if (length_bit_data.size() < length_bit_at)//如果没有读取到标志位
			{
				break;//继续读取
			}
			if (length_bit_data.size() == (length_bit_at + length_bit_data[length_bit_at - 1]))//如果正好一次性读取完成
			{
				emit readCom(length_bit_data);
				length_bit_data.clear();
				length_bit_data.resize(0);

			}
			else//如果读取到标志位 且数据没读完整
			{
				read_length = length_bit_data[length_bit_at - 1];//获取数据标志位之后应该读取的长度
				int totle = length_bit_data.size();//当前已读取长度
				int read_len1 = read_length + length_bit_at - totle;//剩余读取长度
				length_bit_data += serial->readAll(); //read(read_len1);//再次读取剩余长度

				if (length_bit_data.size() >=(length_bit_at + read_length))//如果达到或超过规定的数据长度
				{

					left = length_bit_data.size();
					QByteArray send;
					for (int i = 0; i < (length_bit_at + read_length); i++)//将超过长度截取,剩余部分继续保留在length_bit_data
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

