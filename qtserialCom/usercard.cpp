#include "usercard.h"

usercard::usercard(QObject *parent)
	: QObject(parent)
{
}

void usercard::CheckSumOut(unsigned char * buf, unsigned char len)
{
	unsigned char i;
	unsigned char checksum;
	checksum = 0;
	for (i = 0; i < (len - 1); i++)
	{
		checksum ^= buf[i];
	}
	buf[len - 1] = (unsigned char)~checksum;
}

bool usercard::CheckSumIn(unsigned char * buf, unsigned char len)
{
	unsigned char i;
	unsigned char checksum;
	checksum = 0;
	for (i = 0; i < (len - 1); i++)
	{
		checksum ^= buf[i];
	}
	if (buf[len - 1] == (unsigned char)~checksum)
	{
		return true;
	}
	return false;
}

void usercard::Hex2Str(const unsigned char * sSrc, unsigned char * sDest, int nSrcLen)
{
	int  i;
	char szTmp[3];

	for (i = 0; i < nSrcLen; i++)
	{
		sprintf_s(szTmp, "%02X", (unsigned char)sSrc[i]);
		memcpy(&sDest[i * 2], szTmp, 2);
	}
	sDest[nSrcLen * 2] = '\0';
	return;
}

usercard::~usercard()
{
}
