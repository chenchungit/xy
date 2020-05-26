/*            _________           _________
*          / /________/        / /________/
*        / /                 / /
*      / /                 / /
*    / /                 / /
*	/ /                  / /
*	/ /                  / /
*	 / /                  / /
*	  / / _________         / / _________
*	   / /________/          / /________/
*
* mairycal@163.com
*/

#pragma once
#ifndef _XCODE_H_
#define _XCODE_H_

class XCode
{
public:
	XCode() {};
	~XCode() {};
	string Replase(string &s1, const string &s2, const string &s3);
	void IPV4toIP(DWORD dwIP, char *strIP);
	int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen,
		char *outbuf, size_t outlen);
	int u2g(char *inbuf, size_t inlen, char *outbuf, size_t outlen);
	int g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen);
	void HexStrToByte(const char* source, unsigned char* dest, int sourceLen);
	void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen);
	void XSplit(string _Str, const char* _Flag, vector<string> & _SubArray);
	char* BuildGuid(char *_Buffer);
	void Crc16CcittUpdate(unsigned char ucData, unsigned short  *pwCrc);
	unsigned short GetCrc16Ccitt(unsigned short  wBase, unsigned char* pucBuff, DWORD dwLen);
	uint64_t ntohll(uint64_t _val);
	uint64_t htonll(uint64_t _val);

	uint16_t getDeviceVersion(string _Ver);
private:

};
#endif