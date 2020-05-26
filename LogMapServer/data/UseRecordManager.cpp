#include "ServerHead.h"


void CUseRecordManager::Initialize()
{
	if (g_ServerManager->getIAnalysisPacket())
		m_IAnalysisPacket = g_ServerManager->getIAnalysisPacket();
	ReadDB();
	
}

bool CUseRecordManager::ReadDB()
{
	MYSQL_RES *dzRes;
	MYSQL_ROW dzSqlrow;
	char *szRetConvert;
	char *dzSql = (char*)m_dzConvertBuffList._malloc_boost();
	snprintf(dzSql, MAX_BUFF_LOG_SIZE,
		"call UseRecord(0,'','',0,0,0,'',0,0,'',0,%d)", DB_USE_QUEUE);

	dzRes = g_ServerManager->g_IMysqlInterFace->QuerySql(dzSql);
	m_dzConvertBuffList._free_boost((LPConvertBuffer)dzSql);
	if (!dzRes)
	{
		mysql_free_result(dzRes);
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "Database stored procedure access error");
	}
	else
	{

		while ((dzSqlrow = mysql_fetch_row(dzRes)))
		{

			if (mysql_num_fields(dzRes) < 3)
			{
				continue;
			}
			CUseRecordObject *szUseRecordObject = m_dzUseRecordPool._malloc_boost();
			if (!szUseRecordObject) return false;
			
			dzSqlrow[1] ? szUseRecordObject->setUser(dzSqlrow[1]) : szUseRecordObject->setUser("");
			dzSqlrow[2] ? szUseRecordObject->setMac(dzSqlrow[2]) : szUseRecordObject->setMac("");
			dzSqlrow[3] ? szUseRecordObject->setUseType(atoi(dzSqlrow[3])) : szUseRecordObject->setUseType(0);
			dzSqlrow[4] ? szUseRecordObject->setUseTime(atoll(dzSqlrow[4])) : szUseRecordObject->setUseTime(0);
			dzSqlrow[5] ? szUseRecordObject->setUpTime(atoll(dzSqlrow[5])) : szUseRecordObject->setUpTime(0);
			dzSqlrow[6] ? szUseRecordObject->setLogType(atoi(dzSqlrow[6])) : szUseRecordObject->setLogType(0);
			dzSqlrow[7] ? szUseRecordObject->setCarType(atoi(dzSqlrow[7])) : szUseRecordObject->setCarType(0);
			dzSqlrow[8] ? szUseRecordObject->setGwMac(dzSqlrow[8]) : szUseRecordObject->setGwMac("");
			dzSqlrow[9] ? szUseRecordObject->setAreaDeviceID(strtoull(dzSqlrow[9], &szRetConvert, 10)) : szUseRecordObject->setAreaDeviceID(0L);
			dzSqlrow[10] ? szUseRecordObject->setRemark(dzSqlrow[10]) : szUseRecordObject->setRemark("");

			szUseRecordObject->setDBState(DB_STATE_UNKOWN);
			szUseRecordObject->setLastTime(time(NULL));
			
			//////////////////////////////////////////////////////////////////////////
			if (strlen(szUseRecordObject->getMac()) > 0)
			{
				hash_map<string, vector<CUseRecordObject*>*>::iterator col = m_dzUseRecordData.find(szUseRecordObject->getMac());
				if (col != m_dzUseRecordData.end())
				{

					vector<CUseRecordObject*> *szUseRecordObjectList = col->second;//
					szUseRecordObjectList->push_back(szUseRecordObject);

				}
				else
				{
					vector<CUseRecordObject*> *szUseRecordObjectList = new vector<CUseRecordObject *>();
					szUseRecordObjectList->push_back(szUseRecordObject);
					m_dzUseRecordData[szUseRecordObject->getMac()] = szUseRecordObjectList;
				}
			}
			else
			{
				m_dzUseRecordPool._free_boost(szUseRecordObject);
			}
			//////////////////////////////////////////////////////////////////////////
			if (strlen(szUseRecordObject->getUser()) > 0)
			{
				hash_map<string, vector<CUseRecordObject*>*>::iterator col = m_dzCardsUseRecordData.find(szUseRecordObject->getUser());
				if (col != m_dzCardsUseRecordData.end())
				{

					vector<CUseRecordObject*> *szUseRecordObjectList = col->second;//
					szUseRecordObjectList->push_back(szUseRecordObject);

				}
				else
				{
					vector<CUseRecordObject*> *szUseRecordObjectList = new vector<CUseRecordObject *>();
					szUseRecordObjectList->push_back(szUseRecordObject);
					m_dzCardsUseRecordData[szUseRecordObject->getUser()] = szUseRecordObjectList;
				}
			}
		}
		mysql_free_result(dzRes);

	}
	return true;
}
void CUseRecordManager::SubmitData(string _Mac, string _User, UINT _UseType, UINT _UseTime, UINT _UpTime, string _Remark
	, UINT _LogType, UINT _CarType, string _GwMac, UINT _AreaDeviceID)
{
	//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "UseRecord SubmitData");
	Guard guard(&m_DataMutex, true);
	CUseRecordObject *szUseRecordObject = NULL;

	szUseRecordObject = m_dzUseRecordPool._malloc_boost();
	szUseRecordObject->setMac(_Mac);
	szUseRecordObject->setUser(_User);
	szUseRecordObject->setUseType(_UseType);
	szUseRecordObject->setUseTime(_UseTime);
	szUseRecordObject->setUpTime(_UpTime);
	szUseRecordObject->setRemark(_Remark);
	szUseRecordObject->setLogType(_LogType);
	szUseRecordObject->setCarType(_CarType);
	szUseRecordObject->setGwMac(_GwMac);
	szUseRecordObject->setAreaDeviceID(_AreaDeviceID);

	szUseRecordObject->setDBState(DB_STATE_ADD);
	if (strlen(szUseRecordObject->getMac()) > 0)
	{
		hash_map<string, vector<CUseRecordObject*>*>::iterator col = m_dzUseRecordData.find(szUseRecordObject->getMac());
		if (col != m_dzUseRecordData.end())
		{

			vector<CUseRecordObject*> *szUseRecordObjectList = col->second;//
			szUseRecordObjectList->push_back(szUseRecordObject);

		}
		else
		{
			vector<CUseRecordObject*> *szUseRecordObjectList = new vector<CUseRecordObject *>();
			szUseRecordObjectList->push_back(szUseRecordObject);
			m_dzUseRecordData[szUseRecordObject->getMac()] = szUseRecordObjectList;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (strlen(szUseRecordObject->getUser()) > 0)
	{
		hash_map<string, vector<CUseRecordObject*>*>::iterator col = m_dzCardsUseRecordData.find(szUseRecordObject->getUser());
		if (col != m_dzCardsUseRecordData.end())
		{
			vector<CUseRecordObject*> *szUseRecordObjectList = col->second;//
			szUseRecordObjectList->push_back(szUseRecordObject);
		}
		else
		{
			vector<CUseRecordObject*> *szUseRecordObjectList = new vector<CUseRecordObject *>();
			szUseRecordObjectList->push_back(szUseRecordObject);
			m_dzCardsUseRecordData[szUseRecordObject->getUser()] = szUseRecordObjectList;
		}
	}
	//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "UseRecord SubmitData 2222222222222222222222");
	saveDB(szUseRecordObject);
	//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "UseRecord SubmitData 111111111111111111111");
	if (DEVICE_LOG_TYPE_0 == _LogType)
	{
		Json::Value szPlayload;
		jgGroupPackage(_Mac, _LogType, _CarType, szPlayload);
		if (JgPush(szPlayload))
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "UseRecord JgPush Json::Value Success");
		/*else
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "UseRecord SubmitData JgPush Fuil");*/
	}
	/*else
	  g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "UseRecord SubmitData no DEVICE_LOG_TYPE_0");*/
	//Json::Value & szPlayload = jgGroupPackage(_Mac, _LogType,_CarType);
	//JgPush(szPlayload);
}
void CUseRecordManager::jgGroupPackage(string _Mac, int _LogType, int _CarType, Json::Value &_Root)
{
	//////////////////////////////////////////////////////////////////////////
	//Json::Value  szData;
	Json::Value subData, szData;
	_Root["msgType"] = __FACK_REDIS_JG_NOMAL__;

	szData["mac"] = _Mac;
	szData["time"] = (uint32_t)time(NULL);
	szData["featureType"] = IFT_LOCK;

	if (_LogType == DEVICE_LOG_TYPE_0)
	{
		if (_CarType >= 48 && _CarType <= 50)
			subData["switch"] = true;//使用开关开门
		else
			if (_CarType == 51)
				subData["key"] = true;//使用钥匙开门
			else
				if (_CarType >= 1 && _CarType <= 7)
					subData["doorCard"] = true;//使用门卡开门
				else
					if (_CarType >= 80 && _CarType <= 81)
						subData["bluetooth"] = true;//使用蓝牙开门
					else
						if (_CarType == 82)
							subData["remote"] = true;//使用远程开门
						else
							if ((_CarType >= 32 && _CarType <= 39) || (_CarType >= 160 && _CarType <= 164))
								subData["passWord"] = true;//使用密码开门
							else
								if (_CarType == 64)
									subData["fingerPrint"] = true;//使用指纹开门
								else
									if (_CarType == 65)
										subData["face"] = true;//使用人脸开门
									else
										if (_CarType == 66)
											subData["iris"] = true;//使用虹膜开门
										else
											subData["other"] = true;//使用其它开门
	}
	szData["data"] = subData;
	_Root["msgData"] = szData;
}
bool CUseRecordManager::saveDB(CUseRecordObject * _UseRecordObject)
{
	std::stringstream szCharacters;
	szCharacters << "INSERT INTO xy_userecord (ur_User,ur_Mac,ur_usetype,ur_usertime,";
	szCharacters << "ur_uptime, ur_remark, ur_LogType, ur_CarType, ur_GwMac, ur_AreaDeviceID)VALUES('";
	szCharacters << _UseRecordObject->getUser()<<"','";
	szCharacters << _UseRecordObject->getMac() << "',";
	szCharacters << _UseRecordObject->getUseType() << ",";
	szCharacters << _UseRecordObject->getUseTime() << ",";
	szCharacters << _UseRecordObject->getUpTime() << ",'";
	szCharacters << _UseRecordObject->getRemark() << "',";
	szCharacters << _UseRecordObject->getLogType() << ",";
	szCharacters << _UseRecordObject->getCarType() << ",'";
	szCharacters << _UseRecordObject->getGwMac() << "',";
	szCharacters << _UseRecordObject->getAreaDeviceID() << ");";

	//g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "save DB[%s]", szCharacters.str());
	MYSQL_RES *dzRes = g_ServerManager->g_IMysqlInterFace->ExcelSql(szCharacters.str());
	MYSQL_ROW dzSqlrow;
	if (dzRes)
	{
		while ((dzSqlrow = mysql_fetch_row(dzRes)))
		{
			g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_ERROR, "insert device Log success[%s]", SysTemContext[atoi(dzSqlrow[0])]);
		}
		mysql_free_result(dzRes);
		return true;
	}
	return false;
}
void CUseRecordManager::QueueData(Json::Value &_payLoad, int _Page, int _Size,
	string _Mac, string _UserID, uint64_t _BeginTime, uint64_t _EndTime)
{
	Guard guard(&m_DataMutex, true);
	if (_Mac.empty() && _UserID.empty() && _BeginTime == 0 && _EndTime == 0)
	{
		getAllUseRecord(_payLoad, _Page, _Size);
	}
	if (!_Mac.empty() && _UserID.empty() && _BeginTime != 0 && _EndTime != 0)
		getMacTimeUseRecord(_payLoad, _Page, _Size, _Mac, _BeginTime, _EndTime);
	if (!_Mac.empty() && _UserID.empty() && _BeginTime == 0 && _EndTime == 0)
		getMacUseRecord(_payLoad, _Page, _Size, _Mac);
	if (_Mac.empty() && !_UserID.empty() && _BeginTime == 0 && _EndTime == 0)
		getCardsUseRecord(_payLoad, _Page, _Size, _UserID);
	if (_Mac.empty() && _UserID.empty() && _BeginTime != 0 && _EndTime != 0)
		getTimeUseRecord(_payLoad, _Page, _Size, _BeginTime, _EndTime);
}
void CUseRecordManager::getMacTimeUseRecord(Json::Value &_payLoad, int _Page, int _Size, string _Mac, uint64_t _BeginTime, uint64_t _EndTime)
{
	hash_map<string, vector<CUseRecordObject*>*>::iterator col = m_dzUseRecordData.find(_Mac.c_str());
	if (col != m_dzUseRecordData.end())
	{
		vector<CUseRecordObject*>* szUseRecordObjectArray = col->second;
		Json::Value szData;
		int szIndex = 0;
		for (auto row : *szUseRecordObjectArray)
		{
			if (row->getUseTime() >= _BeginTime && row->getUseTime() <= _EndTime)
			{
				szIndex++;
				if (szIndex >= (_Page - 1) * _Size && szIndex < _Page * _Size)
				{
					Json::Value subData;
					subData["User"] = row->getUser();
					subData["usetype"] = row->getUseType();
					subData["Mac"] = row->getMac();
					subData["Usertime"] = (uint32_t)row->getUseTime();
					subData["Uptime"] = (uint32_t)row->getUpTime();
					subData["LogType"] = row->getLogType();
					subData["CarType"] = row->getCarType();
					subData["GwMac"] = row->getGwMac();
					subData["AreaDeviceID"] = (uint32_t)row->getAreaDeviceID();
					szData.append(subData);
				}
			}

			/*if (szIndex > _Page * _Size)
				break;*/
		}
		_payLoad["Data"] = szData;
		_payLoad["total"] = szIndex;
		cout << "hash size : " << m_dzUseRecordData.size() << "   Mac" << _Mac << endl;
	}
}
void CUseRecordManager::getTimeUseRecord(Json::Value &_payLoad, int _Page, int _Size, uint64_t _BeginTime, uint64_t _EndTime)
{
	int szIndex = 0;
	hash_map<string, vector<CUseRecordObject*>*>::iterator col = m_dzUseRecordData.begin();
	for (;col != m_dzUseRecordData.end();col++)
	{
		vector<CUseRecordObject*>* szUseRecordObjectArray = col->second;
		Json::Value szData;
		
		for (auto row : *szUseRecordObjectArray)
		{
			if (row->getUseTime() >= _BeginTime && row->getUseTime() <= _EndTime)
			{
				szIndex++;
				if (szIndex >= (_Page - 1) * _Size && szIndex < _Page * _Size)
				{

					Json::Value subData;
					subData["User"] = row->getUser();
					subData["usetype"] = row->getUseType();
					subData["Mac"] = row->getMac();
					subData["Usertime"] = (uint32_t)row->getUseTime();
					subData["Uptime"] = (uint32_t)row->getUpTime();
					subData["LogType"] = row->getLogType();
					subData["CarType"] = row->getCarType();
					subData["GwMac"] = row->getGwMac();
					subData["AreaDeviceID"] = (uint32_t)row->getAreaDeviceID();
					szData.append(subData);
				}
			}
		/*	if (szIndex > _Page * _Size)
				break;*/
		}
		_payLoad["Data"] = szData;
		_payLoad["total"] = szIndex;
		cout << "hash size : " << m_dzUseRecordData.size()  << endl;
	}
}
void CUseRecordManager::getCardsUseRecord(Json::Value &_payLoad, int _Page, int _Size, string _UserID)
{
	hash_map<string, vector<CUseRecordObject*>*>::iterator col = m_dzCardsUseRecordData.find(_UserID.c_str());
	if (col != m_dzCardsUseRecordData.end())
	{
		vector<CUseRecordObject*>* szUseRecordObjectArray = col->second;
		Json::Value szData;
		int szIndex = 0;
		for (auto row : *szUseRecordObjectArray)
		{
			szIndex++;
			if (szIndex >= (_Page - 1) * _Size && szIndex < _Page * _Size)
			{
				Json::Value subData;
				subData["User"] = row->getUser();
				subData["usetype"] = row->getUseType();
				subData["Mac"] = row->getMac();
				subData["Usertime"] = (uint32_t)row->getUseTime();
				subData["Uptime"] = (uint32_t)row->getUpTime();
				subData["LogType"] = row->getLogType();
				subData["CarType"] = row->getCarType();
				subData["GwMac"] = row->getGwMac();
				subData["AreaDeviceID"] = (uint32_t)row->getAreaDeviceID();
				szData.append(subData);
			}
			/*if (szIndex > _Page * _Size)
				break;*/
		}
		_payLoad["Data"] = szData;
		_payLoad["total"] = szIndex;
		cout << "hash size : " << m_dzUseRecordData.size() <<  endl;
	}
}
void CUseRecordManager::getMacUseRecord(Json::Value &_payLoad, int _Page, int _Size, string _Mac)
{

	hash_map<string, vector<CUseRecordObject*>*>::iterator col = m_dzUseRecordData.find(_Mac.c_str());
	if (col != m_dzUseRecordData.end())
	{
		vector<CUseRecordObject*>* szUseRecordObjectArray = col->second;
		Json::Value szData;
		int szIndex = 0;
		for (auto row : *szUseRecordObjectArray)
		{
			szIndex++;
			if (szIndex >= (_Page - 1) * _Size && szIndex < _Page * _Size)
			{
				Json::Value subData;
				subData["User"] = row->getUser();
				subData["usetype"] = row->getUseType();
				subData["Mac"] = row->getMac();
				subData["Usertime"] = (uint32_t)row->getUseTime();
				subData["Uptime"] = (uint32_t)row->getUpTime();
				subData["LogType"] = row->getLogType();
				subData["CarType"] = row->getCarType();
				subData["GwMac"] = row->getGwMac();
				subData["AreaDeviceID"] = (uint32_t)row->getAreaDeviceID();
				szData.append(subData);
			}
			/*if (szIndex > _Page * _Size)
				break;*/
		}
		_payLoad["Data"] = szData;
		_payLoad["total"] = szIndex;
		cout << "hash size : " << m_dzUseRecordData.size() << "   Mac" << _Mac << endl;
	}
}
void CUseRecordManager::getAllUseRecord(Json::Value &_payLoad, int _Page, int _Size)
{
	int szIndex = 0;
	hash_map<string, vector<CUseRecordObject*>*>::iterator col = m_dzUseRecordData.begin();
	for (;col != m_dzUseRecordData.end();col++)
	{
		vector<CUseRecordObject*>* szUseRecordObjectArray = col->second;
		Json::Value szData;
		
		for (auto row : *szUseRecordObjectArray)
		{
			szIndex++;
			if (szIndex >= (_Page - 1) * _Size && szIndex < _Page * _Size)
			{
				Json::Value subData;
				subData["User"] = row->getUser();
				subData["usetype"] = row->getUseType();
				subData["Mac"] = row->getMac();
				subData["Usertime"] = (uint32_t)row->getUseTime();
				subData["Uptime"] = (uint32_t)row->getUpTime();
				subData["LogType"] = row->getLogType();
				subData["CarType"] = row->getCarType();
				subData["GwMac"] = row->getGwMac();
				subData["AreaDeviceID"] = (uint32_t)row->getAreaDeviceID();
				szData.append(subData);
			}
			/*if (szIndex > _Page * _Size)
				break;*/
		}
		_payLoad["Data"] = szData;
		_payLoad["total"] = szIndex;
		cout << "hash size : " << m_dzUseRecordData.size() << endl;
	}
}

bool CUseRecordManager::JgPush(Json::Value &_Root)
{
	if (!_Root.empty())
	{
		Json::FastWriter fast_writer;
		Json::ValueType type = _Root.type();
		string SendStr = fast_writer.write(_Root);
		std::stringstream szJson(SendStr);
		return g_ServerManager->g_RedisClientManager.channel(SERVER_REDIS_KEY, szJson);
	}
	else
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "JgPush Json::Value is null");
		return false;
	}
	
}
