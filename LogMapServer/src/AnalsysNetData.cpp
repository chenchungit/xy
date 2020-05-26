#include "ServerHead.h"

CAnalsysNetData::CAnalsysNetData()
{
	m_ThreadID = 0L;
	pthread_create(&m_ThreadID, NULL, AnsysThreadFuntion, this);
}
CAnalsysNetData::~CAnalsysNetData()
{

}
bool CAnalsysNetData::InsertDataQueue(TcpSocketID id, UChar * _data, const int _len)
{
	//g_ServerManager->SystemCharacter("OnTcpSocketRecived=====>");
	Guard guard(&PoolMutex_AnsysFromClient, true);
	LPClient dzNode = m_dzNetNotAnalsysDataList._malloc_boost();
	if (!dzNode)
		return false;
	memcpy(dzNode->dzPSocketBuff, _data, _len);
	dzNode->dzlen = _len;
	dzNode->dzSocketID = id;
	m_dzNotAnalSysData.push(dzNode);

	return true;
}
int CAnalsysNetData::PopAnalSysDataQueue(list<LPClient> &_DataList)
{
	Guard guard(&PoolMutex_AnsysFromClient, true);
	/*if(!m_dzNotAnalSysData.empty())
	cout<<"»º³å´óÐ¡£º"<<m_dzNotAnalSysData.size()<<endl;*/
	while (!m_dzNotAnalSysData.empty())
	{
		LPClient dzNode = m_dzNotAnalSysData.front();
		m_dzNotAnalSysData.pop();
		_DataList.push_back(dzNode);
	}
	return _DataList.size();
}

bool CAnalsysNetData::InsertMQTTDataQueue(string _Topic, UChar * _data, const int _len)
{
	Guard guard(&PoolMutex_AnsysFromMQTT, true);
	LPClient dzNode = m_dzMQTTAnalsysDataList._malloc_boost();
	if (!dzNode)
		return false;
	memcpy(dzNode->dzPSocketBuff, _data, _len);
	dzNode->dzTopic = _Topic;
	dzNode->dzlen = _len;
	m_dzMQTTAnalSysData.push(dzNode);

	return true;
}
int CAnalsysNetData::PopMQTTAnalSysDataQueue(list<LPClient> &_DataList)
{
	Guard guard(&PoolMutex_AnsysFromMQTT, true);
	while (!m_dzMQTTAnalSysData.empty())
	{
		LPClient dzNode = m_dzMQTTAnalSysData.front();
		m_dzMQTTAnalSysData.pop();
		_DataList.push_back(dzNode);
	}
	return _DataList.size();
}


bool CAnalsysNetData::_memcpy_Array(void * _drc, void *_src, int _len)
{
	bool _ret = false;
	memcpy(_drc, _src, _len);
	return _ret;
}

void* CAnalsysNetData::AnsysThreadFuntion(void* para)
{
	CAnalsysNetData *pWnd = (CAnalsysNetData*)para;
	list<LPClient> szClientDataList, szMQTTDataList;
	IAnalysisPacket *szIAnalysisPacket = g_ServerManager->getIAnalysisPacket();
	int szClientDataListSize = 0, szMQTTDataListSize = 0;
	while (g_ThreadRun)
	{
		szClientDataListSize = 0;
		szMQTTDataListSize = 0;
		szClientDataListSize = pWnd->PopAnalSysDataQueue(szClientDataList);
		szMQTTDataListSize = pWnd->PopMQTTAnalSysDataQueue(szMQTTDataList);

		//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL,"Socket Data[%d]Mqtt[%d]", szClientDataListSize, szMQTTDataListSize);
		if ((szClientDataListSize || szMQTTDataListSize) && szIAnalysisPacket)
		{
			if (szClientDataListSize)
			{
				for (list<LPClient>::iterator col = szClientDataList.begin(); col != szClientDataList.end();)
				{
					LPClient szNode = *col;
					szIAnalysisPacket->DecryptTokenData(szNode->dzSocketID,
						szNode->dzPSocketBuff, szNode->dzlen);
					pWnd->m_dzNetNotAnalsysDataList._free_boost(szNode);
					col = szClientDataList.erase(col);
				}
			}
			if (szMQTTDataListSize)
			{
				g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "=============>szMQTTDataListSize[%d]", szMQTTDataListSize);
				for (list<LPClient>::iterator row = szMQTTDataList.begin(); row != szMQTTDataList.end();)
				{
					LPClient szNode = *row;
					szIAnalysisPacket->DecryptTopicData(szNode->dzTopic,szNode->dzPSocketBuff, szNode->dzlen,VERSION_2);
					pWnd->m_dzMQTTAnalsysDataList._free_boost(szNode);
					row = szClientDataList.erase(row);
				}
			}

			szClientDataList.clear();
			szMQTTDataList.clear();
		}
		else
		{
			usleep(100000);
		}

	}
}
