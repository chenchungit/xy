#include <iostream>
#include "ServerHead.h"

CSocketService::CSocketService():
m_pSocket(NULL),
_count(0),
m_pSocketPort(0)
{
}
CSocketService::~CSocketService()
{
	
}
void CSocketService::CloseSocket(TcpSocketID sid)
{
	m_pSocket->CloseSocket(sid,true);
}
void CSocketService::Init(TcpServerInitItem *_TcpServerInitItem)
{
	m_pSocket = CreateSocketServer();
	_TcpServerInitItem->MaxPacketsPerSeconds = 0;
	_TcpServerInitItem->pServerService = this;
	m_pSocket->init(_TcpServerInitItem);
	
}
void CSocketService::Fini()
{
	m_pSocket->fini();
}
void CSocketService::Start()
{
#ifdef CountryCode
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "%s TcpServer Start", ErrorChinaContext[2]);
#else
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "%s TcpServer Start", ErrorEnglishContext[2]);
#endif
	
	m_pSocket->start();
}
void CSocketService::Stop()
{
#ifdef CountryCode
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "%s TcpServer Stop", ErrorChinaContext[2]);
#else
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "%s TcpServer Stop", ErrorEnglishContext[2]);
#endif
	m_pSocket->stop();
}

bool CSocketService::IsOnLine(TcpSocketID sid)
{
	return m_pSocket->IsOnLine(sid);
}


int32_t  CSocketService::OnTcpServerStart()
{
#ifdef CountryCode
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "%s OnTcpServerStart Start", ErrorChinaContext[2]);
#else
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "%s OnTcpServerStart Start", ErrorEnglishContext[2]);
#endif
	return 0;
}
int32_t  CSocketService::OnTcpServerStop()
{
#ifdef CountryCode
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "%s TcpServer Stop", ErrorChinaContext[2]);
#else
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "%s TcpServer Stop", ErrorEnglishContext[2]);
#endif
	return 0;
}
int32_t  CSocketService:: OnTcpSocketConnected(TcpSocketID id, DWORD dwIP)
{
	IPV4toIP(dwIP, m_dzComvert);
#ifdef CountryCode
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "%s Network Connection Address and Token IP = %s Channel = %u Handler = %u",
		ErrorChinaContext[2], m_dzComvert, id.ChannelID, id.HandlerID);
#else
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "%s Network Connection Address and Token IP = %s Channel = %u Handler = %u", 
		ErrorEnglishContext[2], m_dzComvert, id.ChannelID, id.HandlerID);
#endif
	
	return 0;
}
int32_t  CSocketService::OnTcpSocketClose(TcpSocketID id, DWORD dwIP)
{
	memset(m_dzComvert, 0,MAX_PATH);
	IPV4toIP(dwIP, m_dzComvert);
#ifdef CountryCode
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "%s Disconnect Address:IP : %s Channel = %u Handler = %u",
		ErrorChinaContext[2], m_dzComvert, id.ChannelID, id.HandlerID);
#else
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "%s Disconnect Address:IP : %s Channel = %u Handler = %u",
		ErrorEnglishContext[2], m_dzComvert, id.ChannelID, id.HandlerID);
#endif
	g_ServerManager->UnSocketClose(id);
	return 0;
}


int32_t  CSocketService::OnTcpSocketRecived(TcpSocketID id, CMD_Head* head, const char* pData, int32_t DataLen)
{
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "OnTcpSocketRecived");
	g_ServerManager->g_AnalsysNetData->InsertDataQueue(id,(UChar*)pData,DataLen);
	return 0;
}
int32_t CSocketService::OnTcpServerErrorMessage(TcpSocketID id,char* _EMessage)
{
#ifdef CountryCode
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "%s Tcp NetWork Information Channel = %u  MSG = %s",
		ErrorChinaContext[2], m_dzComvert, id.ChannelID, id.HandlerID);
#else
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "%s Tcp NetWork Information Channel = %u  MSG = %s",
		ErrorEnglishContext[2], id.ChannelID, _EMessage);
#endif
	return 0;
}
int32_t CSocketService::OnTcpServerHeart(TcpSocketID id)
{
	g_ServerManager->SocketHeart(id);
	//g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM,"测试心跳信息 [%u][%u]",id.ChannelID,id.HandlerID);
    return 0;
}
int32_t CSocketService::SendToClient(const char* pData, int32_t DataLen,TcpSocketID id,int32_t _iType)
{
	if (m_pSocket)
	{
		CMD_Head mdzHead;
		memset(&mdzHead,0,sizeof(CMD_Head));
		switch (_iType)
		{
		case SERVICE_SEND_APP:
			{
				*((char*)pData + DataLen) = 0xFF;
				*((char*)pData + DataLen + 1) = 0xFF;
				return m_pSocket->Send(id,(char*)pData,DataLen+2);
				break;
			}	
		case SERVICE_SEND_SERVER:
			{
              //g_LogWriteFileManager->InsertWaitingToWriteQueue(LOG_WRITE_IO,"GAME_S2O_SERVER 长度[%d]",DataLen);
				return m_pSocket->Send(id,mdzHead,(char*)pData,DataLen);
				break;
			}
		}
	}
	return 0;
}
int32_t CSocketService::SendToClient(TcpSocketID id,const char* pData, int32_t DataLen)
{
	//g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "~~~~~~~~~~~~~~~~~~  [%d]  ~~~~~~~~~~~~~~~~~~~~~~~~~~~", DataLen);
	if (m_pSocket)
	{
		CMD_Head mdzHead;
		memset(&mdzHead,0,sizeof(CMD_Head));
		return m_pSocket->Send(id,mdzHead,(char*)pData,DataLen);
	}
	return -1;
}




