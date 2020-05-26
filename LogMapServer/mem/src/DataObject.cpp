#include "ServerHead.h"


CDataObject::CDataObject()
{
	m_DataLen = 0;
	memset(m_Data,0,MAX_BUFF_NET_SIZE);
}
CDataObject::~CDataObject()
{

}
int32_t CDataObject::write(char* szData)
{
	if(NULL == szData)
		return 0;
	int32_t Len = (int32_t)strlen(szData);
	if(m_DataLen == MAX_BUFF_NET_SIZE)
		return 0;
	if((Len + m_DataLen + sizeof(int32_t)) > MAX_BUFF_NET_SIZE)
		return 0;
	//memcpy_s(m_Data + m_DataLen,MAX_BUFF_NET_SIZE - m_DataLen,szData,Len);
	return write((char*)szData, (int32_t)Len);;
}
int32_t CDataObject::read(char* szBuf)
{
	/*if(NULL == szBuf)
		return 0;*/
	if(m_DataLen == MAX_BUFF_NET_SIZE)
		return 0;
	read((char*)szBuf, (int32_t)m_DataLen);
	szBuf[m_DataLen] = 0;
	return m_DataLen;
}


int32_t CDataObject::write(const char* pData, int32_t Len)
{
	if(m_DataLen == MAX_BUFF_NET_SIZE)
		return 0;
	if((Len + m_DataLen) > MAX_BUFF_NET_SIZE)
		Len = MAX_BUFF_NET_SIZE - m_DataLen;
	memcpy((char*)(m_Data + m_DataLen), pData, Len);
	m_DataLen += Len;
	return m_DataLen;
}

int32_t CDataObject::read(char* pBuff, int32_t Len)
{
	if(m_DataLen == MAX_BUFF_NET_SIZE)
		return 0;
	if(Len > MAX_BUFF_NET_SIZE)
		return 0;
	//pBuff = m_Data;
	memcpy(pBuff, (char*)m_Data, Len);
	return m_DataLen;
}