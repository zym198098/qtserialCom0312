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
serialthread::serialthread(QObject *parent, QString portname1, QSerialPort::BaudRate BaudRate1, QSerialPort::StopBits stopBits1, QSerialPort::Parity party1, QSerialPort::DataBits databits1) : QObject(parent)
{
	serial = new QSerialPort();
	serial->setPortName(portname1);
	serial->setBaudRate(BaudRate1);
	serial->setStopBits(stopBits1);
	serial->setParity(party1);
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
	
	if (com_start)
	{
		serial_thread = std::move(std::thread(&serialthread::Execute, this));
	}
	
}

void serialthread::stop()
{
	com_start = false;
	if (serial_thread.joinable())
	{
		serial_thread.join();
	}
	serial->close();
}

void serialthread::Execute()
{
	//static QByteArray sRecvTotal;
	static QByteArray sRecvTotal;
	QByteArray data ;

	while (com_start)
	{
		data = serial->readAll();
		if (data.size() == 0)
		{
			std::chrono::milliseconds dura(1);
			std::this_thread::sleep_for(dura);
			continue;
		}
		sRecvTotal += data;
		if (Fixed_length )
		{
			if (sRecvTotal.size() <= 2)
			{
				continue;
			}
			int datalen = sRecvTotal[length_num-1];//获取数据包长度 默认协议第二位为长度  通过设置length_num 改变协议传输长度
			if (fix_rec_size != 0)  //fix_rec_size !=0 按照 fix_rec_size 大小接收；否则按照协议第二位为长度 接收
			{
				datalen= fix_rec_size;
			}
			
			if (datalen == 0)//数据包长度不能为零
			{
				printf("data erro datalenth is 0 !");
				sRecvTotal.clear();
				data.clear();
				continue;
			}
			//int r = sRecvTotal.size() / fix_rec_size;//按照固定大小接收
			int r = sRecvTotal.size() / datalen;//按照数据包第二位长度接收
			if (r)//fix_rec_size字节的正数倍发送
			{
				QByteArray send = sRecvTotal.left(datalen);
				//QByteArray shex = send.toHex();//转16进制
				emit readCom(send);
				int si = sRecvTotal.size()% datalen;
				if (si>0)
				{
					QByteArray Remaining = sRecvTotal.right(si);
					sRecvTotal = Remaining;
				}
				else
				{
					sRecvTotal.clear();
				}
			}
			else
			{

			}
			
			data.clear();
		}
		else
		{
			if (data.size() > 0)
			{
				emit readCom(data);
				data.clear();
			}
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

