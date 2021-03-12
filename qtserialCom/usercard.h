#pragma once

#include <QObject>

class usercard : public QObject
{
	Q_OBJECT

public:
	usercard(QObject *parent);
	unsigned char  CmdReadId[8] = { 0x01, 0x08, 0xA1, 0x20, 0x00, 0x01, 0x00, 0x76 };//读卡号命令
	//读写块数据命令
	unsigned char   Cmd[23] = { 0x01, 0x17, 0xA4, 0x20, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	void CheckSumOut(unsigned char *buf, unsigned char len);//校验和
	

	bool CheckSumIn(unsigned char *buf, unsigned char len);//输入校验 结果校验

	//字节流转换为十六进制字符串的另一种实现方式  
	void Hex2Str(const unsigned char *sSrc, unsigned char *sDest, int nSrcLen);
	
	
	~usercard();
};
