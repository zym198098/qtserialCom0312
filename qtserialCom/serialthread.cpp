#include "serialthread.h"

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
	if (serial == nullptr)
	{
		delete serial;
		serial = nullptr;
	}
}

void serialthread::open()
{
	com_start=serial->open(QSerialPort::ReadWrite);
		serial_thread = std::move(std::thread(&serialthread::Execute, this));

	
}

void serialthread::stop()
{
	com_start = false;
	
		serial_thread.join();
	
	serial->close();
}

void serialthread::Execute()
{
	//static QByteArray sRecvTotal;
	static QByteArray sRecvTotal;
	QByteArray data ;

	while (com_start)
	{

		switch (type)
		{
		case get_type::normal:
			data = serial->readAll();
			if (data.size() == 0)
			{
				serial_sleep(1);
				break;
			}
			emit readCom(data);
			data.clear();
			break;
		case get_type::fixsize:
			data = serial->read(fix_size);
			if (data.size() == 0)
			{
				serial_sleep(1);
				break;
			}
			sRecvTotal = sRecvTotal + data;
			if (sRecvTotal.length() == fix_size)
			{
				emit readCom(sRecvTotal);
				data.clear();
				sRecvTotal.clear();
			}
			
			break;
		case get_type::length_bit:
			data = serial->read(length_bit_at);
			if (data.size() == 0)
			{
				serial_sleep(1);
				break;
			}
			if (data.size() == length_bit_at)
			{
				int sum = data[length_bit_at-1];
				//接收长度不固定
				sRecvTotal = data + serial->read(sum);
				if (sRecvTotal.length() == (sum + length_bit_at))
				{
					emit readCom(sRecvTotal);
					sRecvTotal.clear();
					data.clear();
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

void serialthread::writeData(const QByteArray &data)
{
	if (serial == nullptr)
	{
		return;

	}
	serial->write(data);
}

void serialthread::writeData(const char * data)
{
	if (serial == nullptr)
	{
		return;

	}
	serial->write(data);
	
}

void serialthread::writeData(const char * data, int length)
{
	if (serial == nullptr)
	{
		return;

	}
	serial->write(data,length);
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

