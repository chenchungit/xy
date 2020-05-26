#include "ServerHead.h"

CServiceObject::CServiceObject(int32_t _iType,int32_t _ServerID,TcpSocketID _TcpSocketID,int32_t _State/*,
							   const char *_ip,int32_t _Port*/)
{
	m_iType = _iType;
	m_ServerID = _ServerID;
	m_State = _State;
	m_TcpSocketID = _TcpSocketID;
	m_CurrentUser = 0;
	m_MaxUser = 1000;
	/*memcpy_s(m_ip,sizeof(m_ip),_ip,strlen(_ip));
	m_port = _Port;*/
}

/****************
*更新链接参数
******************/
void CServiceObject::UpdateParameter(int32_t _iType,int32_t _ServerID,TcpSocketID _TcpSocketID,
									 int32_t _State/*,const char *_ip,int32_t _Port*/)
{
	m_iType = _iType;
	m_ServerID = _ServerID;
	m_State = _State;
	m_TcpSocketID = _TcpSocketID;
	m_CurrentUser = 0;
	m_MaxUser = 3000;
	/*memcpy_s(m_ip,sizeof(m_ip),_ip,strlen(_ip));
	m_port = _Port;*/
}
CServiceObject::~CServiceObject()
{

}