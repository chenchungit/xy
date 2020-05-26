#include "ServerHead.h"


CGroupPackageEngine::CGroupPackageEngine()
{

}
CGroupPackageEngine::~CGroupPackageEngine()
{
	
}
int32_t CGroupPackageEngine::TrsportMessage(DWORD _handle,int32_t _iType,const TcpSocketID _SocketID,void * _SrcData)
{
	int32_t ret = S_FALSE;
	if (_SrcData == NULL)
	{
		return ret;
	}
	//UChar* szData = NULL;
	IAnalysisPacket*szIAnalysisPacket = g_ServerManager->getIAnalysisPacket();
	if (!szIAnalysisPacket)
	{
		return ret;
	}

	::google::protobuf::Message* szSerialize = (::google::protobuf::Message*)_SrcData;

	try
	{
		if (szSerialize->IsInitialized())
		{
			uint32_t szLen = 0;
			UChar *szData = (UChar*)m_dzConvertBuffList._malloc_boost();
			if(szData && szIAnalysisPacket->SerializeData(szData,szLen,szSerialize) == PROTOCOL_TYPE_OK)
			{
				CDataObject *dzDataObject = m_dzDataObjectList._malloc_boost();
				if(!dzDataObject)
					return false;
				*dzDataObject<<_handle;
				dzDataObject->write((char*)szData,szLen);
				ret = g_ServerManager->m_SocketService->TrsportMessage(_iType,(UChar*)dzDataObject->m_Data,
				dzDataObject->m_DataLen,_SocketID, SERVICE_SEND_SERVER);
				m_dzDataObjectList._free_boost(dzDataObject);
			}
			m_dzConvertBuffList._free_boost((LPConvertBuffer)szData);
			ret = S_OK;
		}
		else
		{
			return ret;
		}
	}
	catch (...)
	{
		return ret;
	}

	return ret;
}
/****************************
	*协议序列化发送处理
	***************************/
int32_t CGroupPackageEngine::TrsportMessage(int32_t _iType,const TcpSocketID _SocketID,void * _SrcData)
{
	int32_t ret = 0;
	if (_SrcData == NULL)
	{
		return ret;
	}
	//UChar* szData = NULL;
	IAnalysisPacket*szIAnalysisPacket = g_ServerManager->getIAnalysisPacket();
	if (!szIAnalysisPacket)
	{
		return ret;
	}

	::google::protobuf::Message* szSerialize = (::google::protobuf::Message*)_SrcData;

	try
	{
		if (szSerialize->IsInitialized())
		{
			uint32_t szLen = 0; 
			UChar *szData = (UChar*)m_dzConvertBuffList._malloc_boost();
			if(szData && szIAnalysisPacket->SerializeData(szData,szLen,szSerialize) == PROTOCOL_TYPE_OK)
				ret = g_ServerManager->m_SocketService->TrsportMessage(_iType,szData,
				     szLen,_SocketID, SERVICE_SEND_SERVER);
			m_dzConvertBuffList._free_boost((LPConvertBuffer)szData);
			
		}
		else
		{
			return ret;
		}
	}
	catch (...)
	{
		return ret;
	}

	return ret;
}
int32_t CGroupPackageEngine::TrsportMessage(int32_t iType,UChar *dzData,int32_t dzLen,const TcpSocketID dzDrcChannelID,int32_t _iType)
{
	CDataObject *dzDataObject = m_dzDataObjectList._malloc_boost();
	if(!dzDataObject)
		return 0;
	*dzDataObject << iType;
	*dzDataObject << dzLen;
	dzDataObject->write((char*)dzData,dzLen);
	int32_t dzSendLen = 0;
	if (dzDataObject->m_DataLen)
	{
		dzSendLen = g_ServerManager->m_SocketService->SendToClient(dzDataObject->m_Data,
			dzDataObject->m_DataLen,dzDrcChannelID,_iType);
	}
	m_dzDataObjectList._free_boost(dzDataObject);
	
	return dzSendLen;
	
}
/****************************
*发送数据包
***************************/
int32_t CGroupPackageEngine::TrsportTokenMessage(const TcpSocketID _SocketID,TcpSocketID _Token,int32_t iType,const char * _SrcData,int32_t _Len)
{
	CDataObject *dzDataObject = m_dzDataObjectList._malloc_boost();
	if(!dzDataObject)
		return 0;
#ifdef CODE_ACTION_SHOW
	*dzDataObject << _Token;
#else
	//*dzDataObject << _Token;
#endif // !CODE_ACTION_SHOW
	*dzDataObject << iType;
	*dzDataObject << _Len;


	dzDataObject->write((char*)_SrcData,_Len);
	int32_t dzSendLen = 0;
	if (dzDataObject->m_DataLen)
	{
		dzSendLen = TrsportMessage(_SocketID,dzDataObject->m_Data,dzDataObject->m_DataLen);
	}
	m_dzDataObjectList._free_boost(dzDataObject);

	return dzSendLen;
}
/****************************
*发送数据包
***************************/
int32_t CGroupPackageEngine::TrsportMessage(const TcpSocketID _SocketID,const char * _SrcData,int32_t _Len)
{
	int32_t dzSendLen = g_ServerManager->m_SocketService->SendToClient(_SocketID,(char*)_SrcData,
		_Len);
	return dzSendLen;
}
