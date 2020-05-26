#pragma once
#ifndef _CRC32_H_
#define _CRC32_H_

class CCrc32
{
public:
	//创建余式表的函数
	void InitCrc32Table(uint32_t* pdwCrc32Tbl);
	//采用常量余式表的CRC32校验函数
	uint32_t GetCrc32(unsigned char* pucBuff, uint32_t dwLen);
	//CRC32更新函数
	void GetCrc32Updata(uint32_t* pdwCrc32, unsigned char* pucBuff, uint32_t dwLen);
	//采用变量余式表的CRC32校验函数
	uint32_t GetCrc32Tbl(uint32_t* pdwCrc32Tbl, unsigned char* pucBuff, uint32_t dwLen);


	bool MakeDevicePackBufferOutCrc(uint32_t _InitCrc, uint32_t _SrcCrc, unsigned char* pucBuff, uint32_t dwLen);

};
#endif