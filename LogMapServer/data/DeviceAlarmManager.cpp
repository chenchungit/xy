#include "ServerHead.h"

bool CDeviceAlarmManager::DeviceAlarmComp( CDeviceAlarmObject *_Src,  CDeviceAlarmObject *_Drc) {
	    return _Src->getHeppenTime() > _Drc->getHeppenTime();
	
}
void CDeviceAlarmManager::Initialize()
{
	if (g_ServerManager->getIAnalysisPacket())
		m_IAnalysisPacket = g_ServerManager->getIAnalysisPacket();
	ReadDB();
	
}

bool CDeviceAlarmManager::ReadDB()
{
	MYSQL_RES *dzRes;
	MYSQL_ROW dzSqlrow;
	char *szRetConvert;
	char *dzSql = (char*)m_dzConvertBuffList._malloc_boost();
	snprintf(dzSql, MAX_BUFF_LOG_SIZE,
		"call DeviceAlarm(0,'',0,0,0,0,'',%d)", DB_USE_QUEUE);

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
			CDeviceAlarmObject *szDeviceAlarmObject = m_dzDeviceAlarmPool._malloc_boost();
			if (!szDeviceAlarmObject) return false;
			
			dzSqlrow[1] ? szDeviceAlarmObject->setMac(dzSqlrow[1]) : szDeviceAlarmObject->setMac("");
			dzSqlrow[2] ? szDeviceAlarmObject->setCurTime(atoll(dzSqlrow[2])) : szDeviceAlarmObject->setCurTime(0);
			dzSqlrow[3] ? szDeviceAlarmObject->setHeppenTime(atoll(dzSqlrow[3])) : szDeviceAlarmObject->setHeppenTime(0);
			dzSqlrow[4] ? szDeviceAlarmObject->setState(atoi(dzSqlrow[4])) : szDeviceAlarmObject->setState(0);
			dzSqlrow[5] ? szDeviceAlarmObject->setDevSort(atoi(dzSqlrow[5])) : szDeviceAlarmObject->setDevSort(0);
			dzSqlrow[6] ? szDeviceAlarmObject->setDevState(dzSqlrow[6]) : szDeviceAlarmObject->setDevState("");
			szDeviceAlarmObject->setDBState(DB_STATE_UNKOWN);

			if (strlen(szDeviceAlarmObject->getMac()) > 0)
			{
				hash_map<string, vector<CDeviceAlarmObject*>*>::iterator col = m_dzDeviceAlarmData.find(szDeviceAlarmObject->getMac());
				if (col != m_dzDeviceAlarmData.end())
				{

					vector<CDeviceAlarmObject*> *szDeviceAlarmObjectList = col->second;//
					szDeviceAlarmObjectList->push_back(szDeviceAlarmObject);

				}
				else
				{
					vector<CDeviceAlarmObject*> *szDeviceAlarmObjectList = new vector<CDeviceAlarmObject *>();
					szDeviceAlarmObjectList->push_back(szDeviceAlarmObject);
					m_dzDeviceAlarmData[szDeviceAlarmObject->getMac()] = szDeviceAlarmObjectList;
				}
			}
			else
			{
				m_dzDeviceAlarmPool._free_boost(szDeviceAlarmObject);
			}
		}
		mysql_free_result(dzRes);

	}
	return true;
}


void CDeviceAlarmManager::SubmitData(string _Mac, UINT _CurTime, UINT _HeppenTime, UINT _State, UINT _DevSort, UChar* _DevState, UINT _DevStateLen)
{
	Guard guard(&m_DataMutex, true);
	CDeviceAlarmObject *szDeviceAlarmObject = NULL;
	szDeviceAlarmObject = m_dzDeviceAlarmPool._malloc_boost();
	szDeviceAlarmObject->setMac(_Mac.c_str());
	szDeviceAlarmObject->setCurTime(_CurTime);
	szDeviceAlarmObject->setHeppenTime(_HeppenTime);
	szDeviceAlarmObject->setState(_State);
	szDeviceAlarmObject->setDevSort(_DevSort);
	szDeviceAlarmObject->setDevState((char*)_DevState, _DevStateLen);
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "Recv Device Alarm Info");
	szDeviceAlarmObject->setDBState(DB_STATE_ADD);
	if (strlen(szDeviceAlarmObject->getMac()) > 0)
	{
		hash_map<string, vector<CDeviceAlarmObject*>*>::iterator col = m_dzDeviceAlarmData.find(szDeviceAlarmObject->getMac());
		if (col != m_dzDeviceAlarmData.end())
		{

			vector<CDeviceAlarmObject*> *szDeviceAlarmObjectList = col->second;//
			szDeviceAlarmObjectList->push_back(szDeviceAlarmObject);
			sort(szDeviceAlarmObjectList->begin(), szDeviceAlarmObjectList->end(), DeviceAlarmComp);
		}
		else
		{
			vector<CDeviceAlarmObject*> *szDeviceAlarmObjectList = new vector<CDeviceAlarmObject *>();
			szDeviceAlarmObjectList->push_back(szDeviceAlarmObject);
			m_dzDeviceAlarmData[szDeviceAlarmObject->getMac()] = szDeviceAlarmObjectList;
		}
	}
	else
	{
		m_dzDeviceAlarmPool._free_boost(szDeviceAlarmObject);
	}
	saveDB(szDeviceAlarmObject);
	Json::Value szPlayload;
    jgGroupPackage(_Mac, _DevState, szPlayload);
	if(JgPush(szPlayload))
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "Device Alarm JgPush Json::Value Success");
}
bool CDeviceAlarmManager::saveDB(CDeviceAlarmObject * _DeviceAlarmObject)
{
	std::stringstream szCharacters;
	szCharacters << "INSERT INTO xy_devicealarm(da_Mac,da_CurTime,da_HappenTime,da_State,da_DevSort, da_DevState) VALUES('";
	szCharacters << _DeviceAlarmObject->getMac() << "',";
	szCharacters << _DeviceAlarmObject->getCurTime() << ",";
	szCharacters << _DeviceAlarmObject->getHeppenTime() << ",";
	szCharacters << _DeviceAlarmObject->getState() << ",";
	szCharacters << _DeviceAlarmObject->getDevSort() << ",'";
	szCharacters << _DeviceAlarmObject->getDevState() << "');";
	MYSQL_RES *dzRes = g_ServerManager->g_IMysqlInterFace->ExcelSql(szCharacters.str());
	MYSQL_ROW dzSqlrow;
	if (dzRes)
	{
		while ((dzSqlrow = mysql_fetch_row(dzRes)))
		{
			g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_ERROR, "insert device alarm success[%s]", SysTemContext[atoi(dzSqlrow[0])]);
		}
		mysql_free_result(dzRes);
		return true;
	}
	return false;
}
void CDeviceAlarmManager::QueueData(string _Mac, Json::Value &_payLoad, int _Page, int _Size)
{
	Guard guard(&m_DataMutex, true);
	hash_map<string, vector<CDeviceAlarmObject*>*>::iterator col = m_dzDeviceAlarmData.find(_Mac);
	if (col != m_dzDeviceAlarmData.end())
	{
		vector<CDeviceAlarmObject*>* szDeviceAlarmObjectArray = col->second;
		Json::Value szData;
		int szIndex = 0;
		for (auto row : *szDeviceAlarmObjectArray)
		{
			szIndex++;
			if (szIndex >= (_Page - 1) * _Size && szIndex < _Page * _Size)
			{
				Json::Value subData;
				subData["mac"] = row->getMac();
				subData["curTime"] = (uint32_t)row->getCurTime();
				subData["happenTime"] = (uint32_t)row->getHeppenTime();
				subData["state"] = row->getState();

				UChar szDeviceType = 0;
				g_ServerManager->g_ServiceManager.HexStrToByte(row->getDevState(), &szDeviceType, 2);

				subData["featureType"] = szDeviceType;
				switch (szDeviceType)
				{
				case IFT_HUMAN_CNT:
				{
					uint16_t szIn, szOut, szSum;
					szIn = szOut = szSum = 0;
					uint8_t szTrig = 0;
					
					g_ServerManager->g_ServiceManager.HexStrToByte((const char*)row->getDevState() + 2, (unsigned char*)&szTrig, 2);
					g_ServerManager->g_ServiceManager.HexStrToByte((const char*)row->getDevState() + 4, (unsigned char*)&szSum, 4);
					g_ServerManager->g_ServiceManager.HexStrToByte((const char*)row->getDevState() + 8, (unsigned char*)&szIn, 4);
					g_ServerManager->g_ServiceManager.HexStrToByte((const char*)row->getDevState() + 12, (unsigned char*)&szOut, 4);
					
					subData["dir"] = ((szTrig & 0x10) >> 4);//标志
					subData["status"] = szTrig & 0x01;//状态
					subData["humanSum"] = szSum;//总人数
					subData["humanIn"] = szIn;//进
					subData["humanOut"] = szOut;//出
					/*g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "IFT_HUMAN_CNT Device  Human Cnt[%d][%d][%d][%d][%d]",
						(int32_t)szSum, (int32_t)szIn, (int32_t)szOut, (int32_t)((szTrig & 0x10) >> 4), (int32_t)(szTrig & 0x01));*/
					break;
				}
				case IFT_LOCK:
				{
					//subData["featureType"] = IFT_LOCK;
					unsigned char szCardID[64] = { 0 };
					g_ServerManager->g_ServiceManager.HexStrToByte((char*)row->getDevState() + 4, szCardID, 20);
					if (szCardID[0] == 0xfd && szCardID[1] == 0xfd)
					{
						unsigned int iState = (szCardID[3] << 8) + szCardID[2];
						if (iState > 0)//D0 锁舌，D1天地锁, D2门未关, D3钥匙，  D4门内开关，  D5保护开关 ，D6 反锁开关，D7键盘指纹输入锁定，D8门铃
						{
							if (iState & 0x02)
							{
								if (iState & 0x04)
									subData["lockPrized"] = true;//门被撬
								else
									subData["lockNotLocked"] = true;//门未上锁

							}
							if (iState & 0x04 && !(iState & 0x02))
							{
								subData["lockNotClose"] = true;//门未关
							}
							if (iState & 0x80)
							{
								subData["lockKeyboardInputLock"] = true;//键盘输入锁定
							}
							if (iState & 0x100)
							{
								subData["lockDoorbell"] = true;//门铃
							}
							if (iState & 0x200)
							{
								subData["lockLowElectricity"] = true;//门锁电量过低
							}
						}
					}
					
					break;
				}
				case IFT_DOOR_ALARM:
				case IFT_SMOKE_ALARM:
				case IFT_ALARM:
				case IFT_GAS_SENSOR:
				{
					UChar szVoltage = 0;
					g_ServerManager->g_ServiceManager.HexStrToByte(row->getDevState() + 2, &szVoltage, 2);
					subData["voltage"] = szVoltage;
					//subData["featureType"] = szDeviceType;
					break;
				}
				}
				
				subData["devSort"] = row->getDevSort();
				szData.append(subData);
			}
			/*if (szIndex > _Page * _Size)
				break;*/
		}
		_payLoad["data"] = szData;
		_payLoad["total"] = szIndex;
		cout << "hash size : " << m_dzDeviceAlarmData.size() << "   Mac" << _Mac << endl;
	}
	else
	{
		//Json::arrayValue szData;
		/*Json::Value subData("");
		szData.append(subData);*/
		_payLoad["data"] = Json::arrayValue;
	}
}

void CDeviceAlarmManager::jgGroupPackage(string _Mac, UChar* _DevState, Json::Value &_Root)
{
	Json::Value subData,szData;
	_Root["msgType"] = __FACK_REDIS_JG_ALARM__;
	szData["mac"] = _Mac;
	szData["time"] = (uint32_t)time(NULL);
	
	UChar szDeviceType = 0;
	g_ServerManager->g_ServiceManager.HexStrToByte((char*)_DevState, &szDeviceType, 2);

	szData["featureType"] = szDeviceType;
	switch (szDeviceType)
	{
	case IFT_HUMAN_CNT:
	{
		uint16_t szIn, szOut, szSum;
		szIn = szOut = szSum = 0;
		uint8_t szTrig = 0;

		g_ServerManager->g_ServiceManager.HexStrToByte((const char*)_DevState + 2, (unsigned char*)&szTrig, 2);
		g_ServerManager->g_ServiceManager.HexStrToByte((const char*)_DevState + 4, (unsigned char*)&szSum, 4);
		g_ServerManager->g_ServiceManager.HexStrToByte((const char*)_DevState + 8, (unsigned char*)&szIn, 4);
		g_ServerManager->g_ServiceManager.HexStrToByte((const char*)_DevState + 12, (unsigned char*)&szOut, 4);

		subData["dir"] = ((szTrig & 0x10) >> 4);//标志
		subData["status"] = szTrig & 0x01;//状态
		subData["humanSum"] = szSum;//总人数
		subData["humanIn"] = szIn;//进
		subData["humanOut"] = szOut;//出
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "IFT_HUMAN_CNT Device  Human Cnt[%d][%d][%d][%d][%d][%d]",
			(int32_t)szSum, (int32_t)szIn, (int32_t)szOut, (int32_t)((szTrig & 0x10) >> 4), (int32_t)(szTrig & 0x01), (int32_t)szTrig);
		break;
	}
	case IFT_LOCK:
	{
		//_Root["featureType"] = IFT_LOCK;
		unsigned char szCardID[64] = { 0 };
		g_ServerManager->g_ServiceManager.HexStrToByte((char*)_DevState + 4, szCardID, 20);
		if (szCardID[0] == 0xfd && szCardID[1] == 0xfd)
		{
			unsigned int iState = (szCardID[3] << 8) + szCardID[2];
			if (iState > 0)//D0 锁舌，D1天地锁, D2门未关, D3钥匙，  D4门内开关，  D5保护开关 ，D6 反锁开关，D7键盘指纹输入锁定，D8门铃
			{
				if (iState & 0x02)
				{
					if (iState & 0x04)
						subData["lockPrized"] = true;//门被撬
					else
						subData["lockNotLocked"] = true;//门未上锁

				}
				if (iState & 0x04 && !(iState & 0x02))
				{
					subData["lockNotClose"] = true;//门未关
				}
				if (iState & 0x80)
				{
					subData["lockKeyboardInputLock"] = true;//键盘输入锁定
				}
				if (iState & 0x100)
				{
					subData["lockDoorbell"] = true;//门铃
				}
				if (iState & 0x200)
				{
					subData["lockLowElectricity"] = true;//门锁电量过低
				}
			}
		}

		break;
	}
	case IFT_ALARM:
	case IFT_DOOR_ALARM:
	case IFT_SMOKE_ALARM:
	case IFT_GAS_SENSOR:
	{
		UChar szVoltage = 0;
		g_ServerManager->g_ServiceManager.HexStrToByte((char*)_DevState + 2, &szVoltage, 2);
		subData["voltage"] = szVoltage;
		subData["alarm"] = true;
		
		break;
	}
	}
	szData["data"] = subData;
	_Root["msgData"] = szData;
}
bool CDeviceAlarmManager::JgPush(Json::Value &_Root)
{ 
	if (!_Root.empty())
	{
		Json::FastWriter fast_writer;
		Json::ValueType type = _Root.type();
		string SendStr = fast_writer.write(_Root);
		std::stringstream szJson(SendStr);

		//g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_ERROR, "[%s]", szJson.str().c_str());

		return g_ServerManager->g_RedisClientManager.channel(SERVER_REDIS_KEY, szJson);
	}
	else
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_ERROR, "JgPush Json::Value is null");
		return false;
	}
	
}


