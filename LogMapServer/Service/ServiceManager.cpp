#include "ServerHead.h"

CServiceManager::CServiceManager()
{
	
}
CServiceManager::~CServiceManager()
{

}
/****************
*初始化
******************/
void CServiceManager::InitService()
{
	RegisterFuncation();
	//CDataFuntionMap::RegisterFuncation();
}

/*************************Parameter
*查找服务器对象
*************************/
CServiceObject * CServiceManager::FindServerObject(int32_t _ServiceType)
{
	Guard guard(&m_SocketMutex, true);
	CServiceObject * szServiceObject = NULL;
	if (!m_ServiceLink.empty())
	{
		for(map<DWORD,CServiceObject*>::iterator col = m_ServiceLink.begin();
			col != m_ServiceLink.end();
			col++)
		{
			if (col->second && col->second->m_iType == _ServiceType)
			{
				szServiceObject = col->second;
				//RtlCopyMemory(szConnectObject,col->second,sizeof(CConnectObject));
				break;
			}
		}
	}
	return szServiceObject;
}

/****************
*查找空闲链接
******************/
CServiceObject* CServiceManager::FindFreeConnect(int32_t _ServerID)
{
	Guard guard(&m_SocketMutex, true);
	for (map<DWORD,CServiceObject*>::iterator col = m_ServiceLink.begin();
		col != m_ServiceLink.end();col++)
	{
		if (col->second && col->second->m_ServerID == _ServerID)
		{
			CServiceObject *szServiceObject = col->second;
			m_ServiceLink.erase(col);
			return szServiceObject;
		}
	}
	if (!m_ServiceLink.empty())
	{
		for (map<DWORD,CServiceObject*>::iterator col = m_ServiceLink.begin();
			col != m_ServiceLink.end();col++)
		{
			if (col->second && col->second->m_State == YD_SERVICE_STATE_UNKNOW)
			{
				CServiceObject *szServiceObject = col->second;
				m_ServiceLink.erase(col);
				return col->second;
			}
		}
	}
	return NULL;
}

/****************
*处理断开的连接
******************/
void CServiceManager::UnSocketClose(TcpSocketID id)
{
	Guard guard(&m_SocketMutex, true);
	map<DWORD,CServiceObject*>::iterator szSocket = m_ServiceLink.find(id.HandlerID);
	if (szSocket != m_ServiceLink.end())
	{
		szSocket->second->m_State = YD_SERVICE_STATE_UNKNOW;
	}
}
 /****************
*查找当前最优的逻辑服务器分配
******************/
DWORD CServiceManager::FindOptimalConnect()
{
	Guard guard(&m_SocketMutex, true);
	CServiceObject* szServiceObject = NULL;

	if (m_ServiceLink.empty())
	{
		return 0;
	}
	map<DWORD,CServiceObject*>::iterator col = m_ServiceLink.begin();

	for (;col != m_ServiceLink.end();col++)
	{
		if(!szServiceObject && col->second->m_State == YD_SERVICE_STATE_ON )
		{
			szServiceObject = col->second;
		}
		else
			if(col->second->m_State == YD_SERVICE_STATE_ON 
				&& col->second->m_CurrentUser < col->second->m_MaxUser 
				&& szServiceObject->m_CurrentUser > col->second->m_CurrentUser)
			{
				szServiceObject = col->second;
			}
	}
	return szServiceObject ? szServiceObject->m_TcpSocketID.HandlerID : 0;
}
void CServiceManager::InsertServiceObject(CServiceObject *_ServiceObject)
{
	Guard guard(&m_SocketMutex, true);
	m_ServiceLink[_ServiceObject->m_TcpSocketID.HandlerID] = _ServiceObject;
}

/****************
*发送数据到相关的逻辑服务器
******************/
bool CServiceManager::SendDataToLogicService(DWORD _Handle,int32_t _iType,UChar* _Data,int32_t _len)
{
	Guard guard(&m_SocketMutex, true);
	if (!m_ServiceLink.empty())
	{
		map<DWORD,CServiceObject*>::iterator szSocket = m_ServiceLink.find(_Handle);
		if (szSocket != m_ServiceLink.end())
		{
			/*return g_ServerManager->m_SocketService->TrsportMessage(_iType,(UChar*)_Data,
				_len,m_ServiceLink[_Handle]->m_TcpSocketID,GAME_S2O_SERVER) > 0 ? true : false;*/
		} 
		
	}
	return false;
}
/****************
*发送数据到相关的逻辑服务器
******************/
bool CServiceManager::SendDataToLogicService(int32_t _iType,UChar* _Data,int32_t _len)
{
	Guard guard(&m_SocketMutex, true);
	if (!m_ServiceLink.empty())
	{
		map<DWORD,CServiceObject*>::iterator szSocket = m_ServiceLink.begin();
		for (;szSocket != m_ServiceLink.end();szSocket++)
		{
			/*g_ServerManager->m_SocketService->TrsportMessage(_iType,(UChar*)_Data,
				_len,szSocket->second->m_TcpSocketID,GAME_S2O_SERVER);*/
		}
		return true;
	}
	return false;
}

void CServiceManager::SynchronousInitData(TcpSocketID id, int ServiceType)
{
	
	

	SystemCode szSystemCode;
	szSystemCode.set_state(1);
	SendProtocolData(id, szSystemCode, SERVER_INIT_END, "Synchronized data completion");
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "Synchronized data completion");
}

void CServiceManager::SynchronousProtocolData(TcpSocketID id, ::google::protobuf::Message& _Protocol, int32_t _Cmd, const char * _Context)
{
	Guard guard(&m_SocketMutex, true);
	CServiceObject * szServiceObject = NULL;
	if (!m_ServiceLink.empty())
	{
		for (map<DWORD, CServiceObject*>::iterator col = m_ServiceLink.begin();
			col != m_ServiceLink.end();
			col++)
		{
			if (col->second->m_TcpSocketID != id)
			{
				SendProtocolData(col->second->m_TcpSocketID, _Protocol, _Cmd, _Context);
			}
		}
	}
}
void  CServiceManager::UpdateSynchronous()
{
	Guard guard(&m_SocketMutex, true);
	CServiceObject * szServiceObject = NULL;
	if (!m_ServiceLink.empty())
	{
		for (map<DWORD, CServiceObject*>::iterator col = m_ServiceLink.begin();
			col != m_ServiceLink.end();
			col++)
		{
			//g_ServerManager->g_DeviceDescribeinfoManager.SynchronousData(col->second->m_TcpSocketID);
			/*g_ServerManager->g_DevTypeCribeManager.SynchronousData(col->second->m_TcpSocketID);
			g_ServerManager->g_EleSendRecordManager.SynchronousData(col->second->m_TcpSocketID);
			g_ServerManager->g_UserInfoManager.SynchronousData(col->second->m_TcpSocketID);
			g_ServerManager->g_ItemManager.SynchronousData(col->second->m_TcpSocketID);*/
		}
	}
}