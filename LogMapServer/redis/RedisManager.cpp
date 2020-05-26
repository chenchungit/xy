#include "ServerHead.h"



CRedisManager::CRedisManager()
{
	
}



CRedisManager::~CRedisManager()
{
}

bool CRedisManager::CheckStatus(redisContext *ctx)
{
	redisReply *reply = (redisReply*)redisCommand(ctx, "ping");
	if (reply == NULL) return false;

	boost::shared_ptr<redisReply> autoFree(reply, freeReplyObject);

	if (reply->type != REDIS_REPLY_STATUS) return false;
	if (strcasecmp(reply->str, "PONG") != 0) return false;

	return true;
}
//void CRedisManager::ReleaseContext(redisContext *ctx, bool active)
//{
//	if (ctx == NULL) return;
//	if (!active) { redisFree(ctx); return; }
//
//	lock_guard<mutex> lock(m_mutex);
//	m_clients.push(ctx);
//}
void* CRedisManager::RedisThreadFuntion(void* para)
{
	CRedisManager *pWnd = (CRedisManager*)para;

	std::stringstream szTableStream, szKeyStream;

	
	while (g_ThreadRun)
	{
		//Sleep(10000);
		for (auto col : pWnd->m_dzRedisConn)
		{
			CRedisObject *szRedisObject = col.second;

			szTableStream.str("");
			szKeyStream.str("");

			szTableStream << ____FACK_PRODUCT_MAMAGER_REDIS_TABLE_INDEX;
			szKeyStream << ____FACK_PRODUCT_MAMAGER_REDIS_KEY;


			szTableStream << szRedisObject->getDBIndex();
			szKeyStream << szRedisObject->getKey();
			redisReply * reply = pWnd->getString(szRedisObject->getRedisID(), szTableStream.str(), szKeyStream.str());//!pWnd->isError(szRedisObject->getRedisID(), reply)
			if (reply)
			{
				if (reply->type == REDIS_REPLY_ERROR || (reply->type == REDIS_REPLY_STATUS && strcasecmp(reply->str, "OK") != 0))
				{
					g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, " Failed to execute %s", __FUNCTION__);
					/*std::cout << reply->str << std::endl;
					std::cout << "Failed to execute " << __FUNCTION__ << std::endl;*/
					break;
				}
				else
					if (REDIS_REPLY_NIL == reply->type)
					{
						//g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, " redis not data");
						sleep(1);
					}
					else
					{
						pWnd->JsonProtocol(reply->str);
						sleep(2);
						//std::cout << "Data:" << reply->str << std::endl;
					}

				pWnd->freeReply(reply);

			}
			else
				if (pWnd->isError(szRedisObject->getRedisID(), reply))
				{
					g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, " redis reconnect success");
				}
				else
				{
					g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, " redis reconnect Fail");
				}
		}
	}
}

void CRedisManager::JsonProtocol(string _Body)
{
	//MessageBox(NULL, _Body.c_str(),"",MB_OK);
	int szActionType = 0;
	Json::Reader  reader;
	Json::Value   szRoot,szData;
	bool isParse = reader.parse(_Body, szRoot, false);
	//{"actionType":1,"data":{"name":"wangkun1","mobile":"19922824005"}}
	if (isParse && !szRoot["actionType"].isNull())
	{
		/*if (szRoot["actionType"].asInt())
		{*/
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "HttpInterface type =%d ", szRoot["actionType"].asInt());
			//获取类型
			switch (szRoot["actionType"].asInt())
			{
			case ACTION_TYPE_MECH_ADD:
			case ACTION_TYPE_MECH_UPDATE:
			case ACTION_TYPE_MECH_DEL:
			case ACTION_TYPE_ITEM_ADD:
			case ACTION_TYPE_ITEM_UPDATE:
			case ACTION_TYPE_ITEM_DEL:
			case ACTION_TYPE_AREA_ADD:
			case ACTION_TYPE_AREA_UPDATE:
			case ACTION_TYPE_AREA_DEL:
			case ACTION_TYPE_PUBLIC_ELE:
			case ACTION_TYPE_TYPECRIBINFO:
			case ACTION_TYPE_ROLEINFO:
			case ACTION_TYPE_USERINFO:
			case ACTION_TYPE_CARDSRECORD:
			case ACTION_TYPE_JGPUSH:
			{
				szData = szRoot["data"];
				if (!szData["DBID"].isNull())
				{
					H2SRequeseData szH2SRequeseData;
					szH2SRequeseData.set_actiontype(szRoot["actionType"].asInt());
					szH2SRequeseData.set_dbid(szData["DBID"].asUInt());
					g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "HttpInterface type =%d DBID = %llu", szH2SRequeseData.actiontype(), szH2SRequeseData.dbid());
					CConnectObject *szConnectObject = g_ServerManager->FindService(SERVER_DB);
					if (szConnectObject)
					{
						SendTokenProtocolData(szConnectObject->m_TcpSocketID, szConnectObject->m_TcpSocketID, szH2SRequeseData, SERVER_H2S_REQUESEDATA, "synchronization Redis data");
					}
				}
				
				break;
			}
			case ACTION_TYPE_FACEDISCERN:
			{
                //UINT64 _UserID, char *_FaceCondition, char *_Mac, UINT64 _BeginTime, UINT64 _EndTime, UINT64 _ActionID
				szData = szRoot["data"];
				if (!szData["userId"].isNull()
					&& !szData["faceFeature"].isNull()
					&& !szData["deviceMac"].isNull()
					&& !szData["startTime"].isNull()
					&& !szData["endTime"].isNull()
					&& !szData["actionId"].isNull())
				{
					g_ServerManager->g_UserInfoManager.UpHMCondition(szData["userId"].asUInt(), 
						(char*)szData["faceFeature"].asString().c_str(),
						(char*)szData["deviceMac"].asString().c_str(),
						szData["startTime"].asUInt(),
						szData["endTime"].asUInt(),
						szData["actionId"].asUInt()
					    );
				}
				break;
			}
			case ACTION_TYPE_CONFIG:
			{
				szData = szRoot["data"];
				if (!szData["userId"].isNull()
					&& !szData["configType"].isNull()
					&& !szData["faceFeature"].isNull()
					&& !szData["deviceMac"].isNull()
					&& !szData["startTime"].isNull()
					&& !szData["endTime"].isNull()
					&& !szData["actionId"].isNull()
					&& !szData["command"].isNull()
					&& !szData["gwMac"].isNull()
					&& !szData["externData"].isNull())
				{
					g_ServerManager->g_DeviceBaseInfoManager.HttpSendGateWayCode(szData["userId"].asUInt(), 
						szData["configType"].asUInt(), szData["faceFeature"].asUInt(),
						szData["deviceMac"].asString().c_str(), szData["startTime"].asUInt(),
						szData["endTime"].asUInt(), szData["actionId"].asUInt(),
						szData["command"].asUInt(), szData["gwMac"].asString().c_str(),
						szData["externData"].asString().c_str(),4);
				}
				break;
			}
			case ACTION_TYPE_WBLIST:
			{
				szData = szRoot["data"];
				//{"actionType":13, "data" : {"FetIdx":65526, "EndTime" : 1882172071, "CardType" : 162,
				//"ListType" : 2, "FunType" : 160, "StartTime" : 1566489600, "DeviceMac" : "DC2C2688A4DE", 
				//	"CardId" : "000AEC9E0000FF00", "RecordId" : 191048, "GWMac" : "BC14EF004ACC"}}
				if (!szData["DeviceMac"].isNull()
					&& !szData["GWMac"].isNull()
					&& !szData["CardId"].isNull()
					&& !szData["FunType"].isNull()
					&& !szData["StartTime"].isNull()
					&& !szData["EndTime"].isNull()
					&& !szData["CardType"].isNull()
					&& !szData["ListType"].isNull())
				{
					string szTempData = "";
					int szTempIntData = 0;
					uint64_t szTempTimeData = 0;
					WhiteBlackList szWhiteBlackList;

					unsigned char szDeviceMac[32] = { 0 };
					szTempData = szData["DeviceMac"].asString();
					g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "==============Dev = %s=============", szTempData.c_str());
					g_ServerManager->HexStrToByte(szTempData.c_str(), szDeviceMac, szTempData.length() >= DEVICE_MAC_LEN ? DEVICE_MAC_LEN : 0);
					szWhiteBlackList.set_devicemac((char*)szDeviceMac);


					szTempData = szData["GWMac"].asString();
					g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "==============GW = %s=============", szTempData.c_str());
					szWhiteBlackList.set_gwmac(szTempData.c_str());

					szTempData = szData["CardId"].asString();
					RtlZeroMemory(szDeviceMac, 32);
					g_ServerManager->HexStrToByte(szTempData.c_str(), szDeviceMac, szTempData.length() >= 16 ? 16 : 0);

					szTempIntData = szData["FunType"].asInt();
					szWhiteBlackList.set_funtype(szTempIntData);
					//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "****************************1");
					szTempTimeData = szData["StartTime"].asInt();
					szWhiteBlackList.set_starttime(szTempTimeData);
					szTempTimeData = szData["EndTime"].asInt();
					szWhiteBlackList.set_endtime(szTempTimeData);
					//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "****************************2");

					szTempIntData = szData["CardType"].asInt();
					szWhiteBlackList.set_cardtype(szTempIntData);

					szTempIntData = szData["ListType"].asInt();
					szWhiteBlackList.set_listtype(szTempIntData);

					szTempIntData = szData["RecordId"].asInt();
					szWhiteBlackList.set_recordid(szTempIntData);

					szTempIntData = szData["FetIdx"].asInt();
					szWhiteBlackList.set_fetidx(szTempIntData);
					szWhiteBlackList.set_data("123\0");

					

					if (szWhiteBlackList.funtype() == WB_LIST_TYPE_PSW)
					{
						
						if (szWhiteBlackList.listtype() == 2 || szWhiteBlackList.listtype() == 0)
							szDeviceMac[5] = 0x00;
						else
							szDeviceMac[5] = 0x01;
						szWhiteBlackList.set_listtype(0x02);
					}
				/*	if (szWhiteBlackList.funtype() != WB_LIST_TYPE_PSW)
					{
						if (szWhiteBlackList.listtype() == 0x00)
						{
							szWhiteBlackList.set_listtype(0x01);
						}
						if (szWhiteBlackList.listtype() == 0x02)
						{
							szWhiteBlackList.set_listtype(0x03);
						}
					}*/
					/*if (szWhiteBlackList.funtype() == WB_LIST_TYPE_PSW2)
					{
						if (szWhiteBlackList.listtype() == 2 || szWhiteBlackList.listtype() == 0)
							szDeviceMac[5] = 0x00;
						else
							szDeviceMac[5] = 0x01;
					}*/
					szWhiteBlackList.set_cardid(szDeviceMac, 8);

					//g_ServerManager->g_RedisManager.SendMQTTProtocolData(S2D_DOWN_DATACMD, szWhiteBlackList, MQTT_TOPIC_HEARD + szWhiteBlackList.gwmac(), "commit http White Black data");
					g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "****************************send Cards **************************");



					H2SRequeseData szH2SRequeseData;
					szH2SRequeseData.set_actiontype(ACTION_TYPE_WBLIST);
					szH2SRequeseData.set_dbid(szData["RecordId"].asUInt());
					g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "HttpInterface type =%d DBID = %llu", szH2SRequeseData.actiontype(), szH2SRequeseData.dbid());
					CConnectObject *szConnectObject = g_ServerManager->FindService(SERVER_DB);
					if (szConnectObject)
					{
						SendTokenProtocolData(szConnectObject->m_TcpSocketID, szConnectObject->m_TcpSocketID, szH2SRequeseData, SERVER_H2S_REQUESEDATA, "synchronization Redis data");
					}
				}

				break;
			}
			case ACTION_TYPE_HARDWAREVERSION:
			{
				szData = szRoot["data"];
				if (!szData["verType"].isNull())
				{
					VersionManager szVersionManager;

					szVersionManager.set_vertype(szData["verType"].asInt());
					szVersionManager.set_interfacename("NULL");
					string szTotic = "/Version";
					if (_____FUCK____ARE__YOU___V1__________)
					{
						SendMQTTProtocolData(S2D_VERSION_MANAGER, szVersionManager,
							static_cast<string>(SERVER_TOPIC_DOWN) + static_cast<string>(TOPIC_SUB_VERSION), "commit http Version data");

					}
					else
						SendMQTTProtocolData(S2D_VERSION_MANAGER, szVersionManager, MQTT_TOPIC_HARDWARE + szTotic, "commit http Version data");
				}
				
				break;
			}
			case ACTION_TYPE_DELDEVICE:
			{
				g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "=========================================");
				szData = szRoot["data"];
				if (!szData["DBID"].isNull())
				{
					g_ServerManager->g_GWDevContrastManager.DelGwBindingDevice(szData["DBID"].asUInt());
					
				}
				break;
			}
			default:
				break;
			}
		//}
		
	}
}


bool CRedisManager::HashSetInner(DWORD _RedisID, std::stringstream& _Stream)
{
	std::string szData;
	getline(_Stream, szData);
	for (auto col : m_dzRedisConn)
	{
		CRedisObject *szRedisObject = col.second;
		if (szRedisObject->getRedisID() == _RedisID)
		{
			
			redisReply * szReply = (redisReply*)::redisCommand(szRedisObject->getRedisContext(), szData.c_str());
			if (szReply->type == REDIS_REPLY_ERROR ||
				(szReply->type == REDIS_REPLY_STATUS && strcasecmp(szReply->str, "OK") != 0))
			{
				if (szReply->str != nullptr) {
					std::cout << szReply->str << std::endl;
				}
				std::cout << "Failed to execute " << __FUNCTION__ << std::endl << std::endl;
				return false;
			}
			else
			   return true;
		}	}
	return false;
	
}

template<class T, class... Args>
bool CRedisManager::HashSetInner(DWORD _RedisID, std::stringstream& _Stream, T head, Args... rest)
{
	_Stream << head << " ";
	return HashSetInner(_RedisID, _Stream, rest...);
}

bool CRedisManager::Set(DWORD _RedisID, std::string _Data)
{
	for (auto col : m_dzRedisConn)
	{
		CRedisObject *szRedisObject = col.second;
		if (szRedisObject->getRedisID() == _RedisID)
		{
			
			redisReply * szReply = (redisReply*)::redisCommand(szRedisObject->getRedisContext(), _Data.c_str());
			if (szReply->type == REDIS_REPLY_ERROR ||
				(szReply->type == REDIS_REPLY_STATUS && strcasecmp(szReply->str, "OK") != 0))
			{
				if (szReply->str != nullptr) {
					std::cout << szReply->str << std::endl;
				}
				std::cout << "Failed to execute " << __FUNCTION__ << std::endl << std::endl;
				return false;
			}
			else
				return true;
		}
	}
	return false;
}
redisReply * CRedisManager::getString(DWORD _RedisID,  const string & _key)
{

	for (auto col : m_dzRedisConn)
	{
		CRedisObject *szRedisObject = col.second;
		if (szRedisObject->getRedisID() == _RedisID)
		{
			
			redisReply * szReply = (redisReply*)::redisCommand(szRedisObject->getRedisContext(), _key.c_str());
			if (szReply->type == REDIS_REPLY_ERROR ||
				(szReply->type == REDIS_REPLY_STATUS && strcasecmp(szReply->str, "OK") != 0))
			{
				if (szReply->str != nullptr) {
					std::cout << szReply->str << std::endl;
				}
				std::cout << "Failed to execute " << __FUNCTION__ << std::endl << std::endl;
				return NULL;
			}
			else
				return szReply;
		}
	}
	return NULL;
}
redisReply * CRedisManager::getString(DWORD _RedisID, const string & _Tableindex, const string & _key)
{
	for (auto col : m_dzRedisConn)
	{
		CRedisObject *szRedisObject = col.second;
		if (szRedisObject->getRedisID() == _RedisID)
		{
			redisContext* szRedisContext = szRedisObject->getRedisContext();
			if (szRedisContext && CheckStatus(szRedisContext))
			{
				redisReply *szReply = (redisReply*)::redisCommand(szRedisContext, _Tableindex.c_str());
				if (szReply)
				{
					if (szReply->type == REDIS_REPLY_ERROR ||
						(szReply->type == REDIS_REPLY_STATUS && strcasecmp(szReply->str, "OK") != 0))
					{
						std::cout << "Failed Table to execute " << __FUNCTION__ << std::endl << std::endl;
						return nullptr;
					}
					else
					{
						if(szReply)
							freeReply(szReply);
						szReply = (redisReply*)::redisCommand(szRedisObject->getRedisContext(), _key.c_str());
						if (szReply)
						{
							if (szReply->type == REDIS_REPLY_ERROR ||
								(szReply->type == REDIS_REPLY_STATUS && strcasecmp(szReply->str, "OK") != 0))
							{
								if (szReply->str != nullptr) {
									std::cout << szReply->str << std::endl;
								}
								std::cout << "Failed to execute " << __FUNCTION__ << std::endl << std::endl;
								return nullptr;
							}
							else
								return szReply;
						}
						
					}
				}
				
			}
			
		}
	}
	return nullptr;
}
void CRedisManager::getString(DWORD _RedisID, const string & _key, string & _value)
{
	redisReply *szReply = getString(_RedisID, _key);
	if (!isError(_RedisID,szReply) && szReply->type == REDIS_REPLY_STRING)
	{
		_value = szReply->str;
		freeReply(szReply);
	}
}
void CRedisManager::getString(DWORD _RedisID, const string & _key, int & _value)
{
	redisReply *szReply = getString(_RedisID, _key);
	if (!isError(_RedisID, szReply) && szReply->type == REDIS_REPLY_STRING)
	{
		_value = atoi(szReply->str);
		freeReply(szReply);
	}
}
void CRedisManager::getString(DWORD _RedisID, const string & _key, float & _value)
{
	redisReply *szReply = getString(_RedisID, _key);
	if (!isError(_RedisID, szReply) && szReply->type == REDIS_REPLY_STRING)
	{
		_value = atof(szReply->str);
		freeReply(szReply);
	}
}
bool CRedisManager::isError(DWORD _RedisID, redisReply *_Reply)
{
	if (nullptr == _Reply)
	{
		freeReply(_Reply);
		map<DWORD, CRedisObject*>::iterator col = m_dzRedisConn.find(_RedisID);
		if (col != m_dzRedisConn.end())
		{
			col->second->disConnect();
			col->second->InitRedis(1);
			return true;
		}
	}
	return false;
}
void CRedisManager::freeReply(redisReply *_Reply)
{
	if (nullptr != _Reply)
	{
		::freeReplyObject(_Reply);
		_Reply = nullptr;
	}
}

redisContext* CRedisManager::CreateRedis(char* _RedisIP, int _Port, int _Seconds, DWORD _RedisID,int _DBIndex, char* _Key)
{
	CRedisObject *szRedisObject = new CRedisObject(_RedisIP,_Port,_Seconds,_RedisID, _DBIndex, _Key);
	if (szRedisObject)
	{
		if (szRedisObject->InitRedis(_Seconds))
		{
			m_dzRedisConn[_RedisID] = szRedisObject;

			m_ThreadID = 0L;
			pthread_create(&m_ThreadID, NULL, RedisThreadFuntion, this);
			/*m_CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
			if (m_CompletionPort != INVALID_HANDLE_VALUE)
			{
				m_ThreadIO = (void*)CreateThread(NULL,
					0,
					(LPTHREAD_START_ROUTINE)RedisThreadFuntion,
					this,
					0,
					NULL);
			}*/

			return szRedisObject->getRedisContext();
		}
		else
		{
			delete szRedisObject;
			szRedisObject = NULL;
		}
	}
	return NULL;
}