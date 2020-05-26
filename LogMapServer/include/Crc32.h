#pragma once
#ifndef _CRC32_H_
#define _CRC32_H_

class CCrc32
{
public:
	//������ʽ��ĺ���
	void InitCrc32Table(uint32_t* pdwCrc32Tbl);
	//���ó�����ʽ���CRC32У�麯��
	uint32_t GetCrc32(unsigned char* pucBuff, uint32_t dwLen);
	//CRC32���º���
	void GetCrc32Updata(uint32_t* pdwCrc32, unsigned char* pucBuff, uint32_t dwLen);
	//���ñ�����ʽ���CRC32У�麯��
	uint32_t GetCrc32Tbl(uint32_t* pdwCrc32Tbl, unsigned char* pucBuff, uint32_t dwLen);


	bool MakeDevicePackBufferOutCrc(uint32_t _InitCrc, uint32_t _SrcCrc, unsigned char* pucBuff, uint32_t dwLen);

};
#endif