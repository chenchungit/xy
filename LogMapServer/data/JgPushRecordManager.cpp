#include "ServerHead.h"


void CJgPushRecordManager::Initialize()
{
	if (g_ServerManager->getIAnalysisPacket())
		m_IAnalysisPacket = g_ServerManager->getIAnalysisPacket();
	ReadDB();
	m_MaxID = 1L;
}
bool CJgPushRecordManager::ReadDB()
{
	MYSQL_RES *dzRes;
	MYSQL_ROW dzSqlrow;
	char *szRetConvert;
	char *dzSql = (char*)m_dzConvertBuffList._malloc_boost();
	snprintf(dzSql, MAX_BUFF_LOG_SIZE,
		"call JgPushRecord(0,0,0,'','','','','',0,0,%d)", DB_USE_QUEUE);

	dzRes = g_ServerManager->g_IMysqlInterFace->QuerySql(dzSql);
	m_dzConvertBuffList._free_boost((LPConvertBuffer)dzSql);
	if (!dzRes)
	{
		mysql_free_result(dzRes);
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "Database stored procedure access error");
	}
	else
	{//
		while ((dzSqlrow = mysql_fetch_row(dzRes)))
		{
			if (mysql_num_fields(dzRes) < 3)
			{
				continue;
			}
			CJgPushRecordObject *szJgPushRecordObject = m_dzJgPushRecordPool._malloc_boost();
			if (!szJgPushRecordObject) return false;
			dzSqlrow[5] ? szJgPushRecordObject->setContent(dzSqlrow[5]) : szJgPushRecordObject->setContent("");
			dzSqlrow[6] ? szJgPushRecordObject->setContentTitle(dzSqlrow[6]) : szJgPushRecordObject->setContentTitle("");
			dzSqlrow[7] ? szJgPushRecordObject->setTagAlias(dzSqlrow[7]) : szJgPushRecordObject->setTagAlias("");
		
			if(strlen(szJgPushRecordObject->getTagAlias()) > 0)
			{ 
				hash_map<string, vector<CJgPushRecordObject*>*>::iterator col = m_dzJgPushRecordData.find(szJgPushRecordObject->getTagAlias());
				if (col != m_dzJgPushRecordData.end())
				{

					vector<CJgPushRecordObject*> *szJgPushRecordObjectList = col->second;//
					szJgPushRecordObjectList->push_back(szJgPushRecordObject);
					
				}
				else
				{
					vector<CJgPushRecordObject*> *szJgPushRecordObjectList = new vector<CJgPushRecordObject *>();
					szJgPushRecordObjectList->push_back(szJgPushRecordObject);
					m_dzJgPushRecordData[szJgPushRecordObject->getTagAlias()] = szJgPushRecordObjectList;
				}
			}
			else
			{
				m_dzJgPushRecordPool._free_boost(szJgPushRecordObject);
			}	 
		}
		mysql_free_result(dzRes);
	}
	return true;
}
void CJgPushRecordManager::QueueData(const char * _TagAlias, Json::Value &_payLoad, int _Page, int _Size)
{
	hash_map<string, vector<CJgPushRecordObject*>*>::iterator col = m_dzJgPushRecordData.find(_TagAlias);
	if (col != m_dzJgPushRecordData.end())
	{
		vector<CJgPushRecordObject*>* szJgPushRecordObjectArray = col->second;
		Json::Value szData;
		int szIndex = 0;
		for (auto row: *szJgPushRecordObjectArray)
		{
			szIndex++;
			if (szIndex >= (_Page - 1) * _Size && szIndex < _Page * _Size)
			{
				Json::Value subData;
				subData["Content"] = row->getContent();
				subData["ContentTitle"] = row->getContentTitle();
				szData.append(subData);
			}
			if (szIndex > _Page * _Size)
				break;
		}
		_payLoad["Data"] = szData;
		cout << "hash size : " << m_dzJgPushRecordData.size()<<  "   RegisTration" << _TagAlias << endl;
	}
}
bool CJgPushRecordManager::SubmitData(const char * _TagAlias, const char * _ContextTitle, const char * _Context)
{
	CJgPushRecordObject *szJgPushRecordObject = m_dzJgPushRecordPool._malloc_boost();
	szJgPushRecordObject->setContent(_Context);
	szJgPushRecordObject->setContentTitle(_ContextTitle);
	szJgPushRecordObject->setTagAlias(_TagAlias);

	if (strlen(szJgPushRecordObject->getTagAlias()) > 0)
	{
		hash_map<string, vector<CJgPushRecordObject*>*>::iterator col = m_dzJgPushRecordData.find(szJgPushRecordObject->getTagAlias());
		if (col != m_dzJgPushRecordData.end())
		{

			vector<CJgPushRecordObject*> *szJgPushRecordObjectList = col->second;//
			szJgPushRecordObjectList->push_back(szJgPushRecordObject);

		}
		else
		{
			vector<CJgPushRecordObject*> *szJgPushRecordObjectList = new vector<CJgPushRecordObject *>();
			szJgPushRecordObjectList->push_back(szJgPushRecordObject);
			m_dzJgPushRecordData[szJgPushRecordObject->getTagAlias()] = szJgPushRecordObjectList;
		}
		return true;
	}
	else
	{
		m_dzJgPushRecordPool._free_boost(szJgPushRecordObject);
		return false;
	}
}
