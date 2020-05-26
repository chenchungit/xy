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
#ifndef _DATAOBJECT_H_
#define _DATAOBJECT_H_
#pragma once

class CDataObject
{
public:
	CDataObject();
	~CDataObject();
public:
	int32_t				write(const char* pData, int32_t Len);
	int32_t				read(char* pBuff, int32_t Len);
	/*
	字符串流读写:
	输入/输出格式:0结尾的ANSI字符串.
	储存格式:  1.首先写入字符串长度,然后紧跟字符串,读出时进行逆操作.
	*/
	int32_t				write(char* szData);
	int32_t				read(char* szBuf);
	template<class T> inline int32_t write(T val)
	{
		return write((char *)&val,sizeof(T));
	}
	template<class T> inline int32_t read(T &val)
	{
		return read((char *)&val,sizeof(T));
	}
	template<class T> CDataObject& operator << (T &val)
	{
		write((char*)&val, sizeof(val));
		return *this;
	}
	template<class T> CDataObject& operator >> (T &val)
	{
		read((char*)&val, sizeof(val));
		return *this;
	}



	CDataObject& operator << (char* szData){
		
		write(szData);
		
		return *this;
	}
	CDataObject& operator >> (char* szBuff){read(szBuff); return *this;}
public:
	char m_Data[MAX_BUFF_NET_SIZE];
	int32_t m_DataLen;

};


#endif