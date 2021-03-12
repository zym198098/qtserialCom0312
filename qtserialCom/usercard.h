#pragma once

#include <QObject>

class usercard : public QObject
{
	Q_OBJECT

public:
	usercard(QObject *parent);
	unsigned char  CmdReadId[8] = { 0x01, 0x08, 0xA1, 0x20, 0x00, 0x01, 0x00, 0x76 };//����������
	//��д����������
	unsigned char   Cmd[23] = { 0x01, 0x17, 0xA4, 0x20, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	void CheckSumOut(unsigned char *buf, unsigned char len);//У���
	

	bool CheckSumIn(unsigned char *buf, unsigned char len);//����У�� ���У��

	//�ֽ���ת��Ϊʮ�������ַ�������һ��ʵ�ַ�ʽ  
	void Hex2Str(const unsigned char *sSrc, unsigned char *sDest, int nSrcLen);
	
	
	~usercard();
};
