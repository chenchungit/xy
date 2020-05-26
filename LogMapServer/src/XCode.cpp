
#include "ServerHead.h"



string XCode::Replase(string &s1, const string &s2, const string &s3)
{
	string::size_type pos = 0;
	string::size_type a = s2.size();
	string::size_type b = s3.size();
	while ((pos = s1.find(s2, pos))
		!= string::npos)
	{
		s1.replace(pos, a, s3);
		pos += b;
	}
	return s1;
}
void XCode::IPV4toIP(DWORD dwIP, char *strIP)
{
	//char strIP[MAX_PATH] = {0};
	short add1, add2, add3, add4;

	add1 = (short)(dwIP & 255);
	add2 = (short)((dwIP >> 8) & 255);
	add3 = (short)((dwIP >> 16) & 255);
	add4 = (short)((dwIP >> 24) & 255);
	sprintf(strIP, "%d.%d.%d.%d", add1, add2, add3, add4);

}
//mbstowcs
int XCode::code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen,
	char *outbuf, size_t outlen) {
	iconv_t cd;
	char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset, from_charset);
	if (cd == 0)
		return -1;
	memset(outbuf, 0, outlen);
	if (iconv(cd, pin, &inlen, pout, &outlen) == -1)
		return -1;
	iconv_close(cd);
	*pout = (char*)'\0';

	return 0;
}

int XCode::u2g(char *inbuf, size_t inlen, char *outbuf, size_t outlen) {
	return code_convert("utf-8", "gb2312", inbuf, inlen, outbuf, outlen);
}

int XCode::g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen) {
	return code_convert("gb2312", "utf-8", inbuf, inlen, outbuf, outlen);
}

void XCode::HexStrToByte(const char* source, unsigned char* dest, int sourceLen)
{
	short i;
	unsigned char highByte, lowByte;

	for (i = 0; i < sourceLen; i += 2)
	{
		highByte = toupper(source[i]);
		lowByte = toupper(source[i + 1]);

		if (highByte > 0x39)
			highByte -= 0x37;
		else
			highByte -= 0x30;

		if (lowByte > 0x39)
			lowByte -= 0x37;
		else
			lowByte -= 0x30;

		dest[i / 2] = (highByte << 4) | lowByte;
	}
	return;
}
void XCode::ByteToHexStr(const unsigned char* source, char* dest, int sourceLen)
{
	int i;
	unsigned char highByte, lowByte;

	for (i = 0; i < sourceLen; i++)
	{
		highByte = source[i] >> 4;
		lowByte = source[i] & 0x0f;

		highByte += 0x30;

		if (highByte > 0x39)
			dest[i * 2] = highByte + 0x07;
		else
			dest[i * 2] = highByte;

		lowByte += 0x30;
		if (lowByte > 0x39)
			dest[i * 2 + 1] = lowByte + 0x07;
		else
			dest[i * 2 + 1] = lowByte;
	}
	return;
}

void XCode::XSplit(string _Str, const char* _Flag, vector<string> & _SubArray)
{
	boost::split(_SubArray, _Str, boost::is_any_of(_Flag), boost::token_compress_on);
}
char* XCode::BuildGuid(char *_Buffer)
{
	uuid_t szGuid;
	uuid_generate(szGuid);
	sprintf(_Buffer, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
		szGuid[0], szGuid[1], szGuid[2], szGuid[3], szGuid[4], szGuid[5], szGuid[6], szGuid[7], szGuid[8], szGuid[9],
		szGuid[10], szGuid[11], szGuid[12], szGuid[13], szGuid[14], szGuid[15]);
	return _Buffer;
}


void XCode::Crc16CcittUpdate(unsigned char ucData, unsigned short  *pwCrc) {
	unsigned short  x = *pwCrc;
	x = (unsigned short)((unsigned char)(x >> 8) | (x << 8));
	x ^= (unsigned char)ucData;
	x ^= (unsigned char)(x & 0xFF) >> 4;
	x ^= (x << 8) << 4;
	x ^= ((x & 0xFF) << 4) << 1;
	*pwCrc = x;
}

unsigned short  XCode::GetCrc16Ccitt(unsigned short  wBase, unsigned char* pucBuff, DWORD dwLen) {
	unsigned short  wRet = wBase;
	DWORD dwCnt = 0;
	for (; dwCnt < dwLen; dwCnt++) {
		Crc16CcittUpdate(pucBuff[dwCnt], &wRet);
	}
	return wRet;
}
uint64_t XCode::htonll(uint64_t _val)
{
	return (((uint64_t)htonl(_val)) << 32) + htonl(_val >> 32);
}
uint64_t XCode::ntohll(uint64_t _val)
{
	return (((uint64_t)ntohl(_val)) << 32) + ntohl(_val >> 32);
}

uint16_t XCode::getDeviceVersion(string _Ver)
{
	replace(_Ver.begin(), _Ver.end(), '.', ' ');
	istringstream istrStream(_Ver);
	int szVer[3] = {0};
	istrStream >> szVer[0] >> szVer[1] >> szVer[2];
	return ((szVer[0] & 0x1F) << 11 |
		(szVer[1] & 0x1F) << 6 |
		(szVer[2] & 0x3F));
}