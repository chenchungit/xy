#include "ServerHead.h"



// 初始化HttpServer静态类成员
mg_serve_http_opts HttpServer::s_server_option;
std::string HttpServer::s_web_dir = "./web";
std::unordered_map<std::string, ReqHandler> HttpServer::s_handler_map;
std::unordered_map<std::string, WebSocketHandler> HttpServer::s_WebSocket_map;

bool CHttpServerManager::handle_fun1(std::string url, std::string body, std::string param, mg_connection *c, OnRspCallback rsp_callback)
{
	std::cout << "handle fun1" << std::endl;
	std::cout << "url: " << url << std::endl;
	std::cout << "body: " << body << std::endl;
	

	Json::Reader  reader;
	Json::Value   value;
	if (reader.parse(body, value))
	{
		if (!value["AreaDeviceID"].isNull())
		{
			cout << value["AreaDeviceID"].asInt() << endl;
			cout << value["LogType"].asString() << endl;
		}
	}


	Json::FastWriter fast_writer;
	Json::Value root;  // 表示整个 json 对象
	root["Code"] = Json::Value(0);
	root["Message"] = "Success";

	Json::ValueType type = root.type();
	string SendStr = fast_writer.write(root);
	HttpServer::SendRsp(c, SendStr);
	return true;
}

bool CHttpServerManager::SubmitRecord(std::string url, std::string body, std::string param, mg_connection *c, OnRspCallback rsp_callback)
{
	Json::FastWriter fast_writer;
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, " HttpInterface body:%s", body.c_str());

	int szActionType = 0;
	Json::Reader  reader;
	Json::Value szRoot, szHeader, szPayLoad, szTempValue, szData;
	char szMessageID[33] = { 0 };
	bool isParse = reader.parse(body, szTempValue, false);
	if (isParse)
	{
		szHeader["Code"] = Json::Value(HTTP_ERROR_FAIL);
		if (szTempValue["actionType"].isInt())
		{
			switch (szTempValue["actionType"].asInt())
			{
			case SUBMIT_TYPE_JGPUSH:
			{
				if (g_ServerManager->g_JgPushRecordManager.SubmitData(szTempValue["regisTration"].asString().c_str(),
					szTempValue["contextTitle"].asString().c_str(),
					szTempValue["context"].asString().c_str()))
				{

					szHeader["Code"] = Json::Value(HTTP_ERROR_SUCCESS);
				}
				else
					szHeader["Code"] = Json::Value(HTTP_ERROR_FAIL);
				break;
			}
			case SUBMIT_TYPE_USERECORD:
			{
				//const char*_Mac, const char*_User, UINT _UseType, UINT _UseTime, UINT _UpTime, const char* _Remark
				//	, UINT _LogType, UINT _CarType, const char* _GwMac, UINT _AreaDeviceID
				g_ServerManager->g_UseRecordManager.SubmitData(
					static_cast<string>(szTempValue["deviceMac"].asString()),
					static_cast<string>(szTempValue["userId"].asString()),
					static_cast<UINT>(szTempValue["useType"].asUInt()),
					static_cast<UINT>(szTempValue["useTime"].asUInt()),
					static_cast<UINT>(szTempValue["upTime"].asUInt()),
					static_cast<string>(szTempValue["remark"].asString()),
					static_cast<UINT>(szTempValue["logType"].asUInt()),
					static_cast<UINT>(szTempValue["carType"].asUInt()),
					static_cast<string>(szTempValue["gwMac"].asString()),
					static_cast<UINT>(szTempValue["areaDeviceID"].asUInt())
				);
				szHeader["Code"] = Json::Value(HTTP_ERROR_SUCCESS);
				
				break;
			}
			case SUBMIT_TYPE_ALARMRECORD:
			{
				g_ServerManager->g_DeviceAlarmManager.SubmitData(
					static_cast<string>(szTempValue["deviceMac"].asString()),
					static_cast<UINT>(szTempValue["curTime"].asUInt()),
					static_cast<UINT>(szTempValue["heppenTime"].asUInt()),
					static_cast<UINT>(szTempValue["state"].asUInt()),
					static_cast<UINT>(szTempValue["devSort"].asUInt()),
					(UChar*)szTempValue["devState"].asString().c_str() , szTempValue["devState"].asString().length());

					szHeader["Code"] = Json::Value(HTTP_ERROR_SUCCESS);
				
				//	szHeader["Code"] = Json::Value(HTTP_ERROR_FAIL);
				break;
			}
			default:
				break;
			}
			
		}
		else
		{
			szHeader["Code"] = Json::Value(HTTP_ERROR_NOPARAM);

		}
	}
	else
	{
		szHeader["Code"] = Json::Value(HTTP_ERROR_NOSERIALIZE);
	}
	szRoot["header"] = szHeader;
	szRoot["payload"] = szPayLoad;

	Json::ValueType type = szRoot.type();
	string SendStr = fast_writer.write(szRoot);
	HttpServer::SendRsp(c, SendStr);
	return true;
}
bool CHttpServerManager::QueueJgPush(std::string url, std::string body, std::string param, mg_connection *c, OnRspCallback rsp_callback)
{
	Json::FastWriter fast_writer;
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, " HttpInterface body:%s", body.c_str());

	int szActionType = 0;
	Json::Reader  reader;
	Json::Value szRoot, szHeader, szPayLoad,szTempValue, szData;
	char szMessageID[33] = { 0 };
	bool isParse = reader.parse(body, szTempValue, false);
	if (isParse)
	{
		if (szTempValue["actionType"].isInt())
		{
			szHeader["name"] = "JgPush";
			szHeader["messageId"] = g_ServerManager->g_HttpServerManager.BuildGuid(szMessageID);
			szHeader["namespace"] = ___FACK___JSON__JGRECORD;
			szHeader["payLoadVersion"] = ___FACK___JSON__VWESION;
			szHeader["code"] = Json::Value(HTTP_ERROR_SUCCESS);

			g_ServerManager->g_JgPushRecordManager.QueueData(szTempValue["regisTration"].asString().c_str(),
				szPayLoad, szTempValue["page"].asInt(), szTempValue["pageSize"].asInt());
		}
		else
		{
			szHeader["code"] = Json::Value(HTTP_ERROR_NOPARAM);
			
		}
	}
	else
	{
		szHeader["code"] = Json::Value(HTTP_ERROR_NOSERIALIZE);
	}
	szRoot["header"] = szHeader;
	szRoot["payload"] = szPayLoad;

	Json::ValueType type = szRoot.type();
	string SendStr = fast_writer.write(szRoot);
	HttpServer::SendRsp(c, SendStr);
	return true;
}

bool CHttpServerManager::SubmitVersion(std::string url, std::string body, std::string param, mg_connection *c, OnRspCallback rsp_callback)
{
	//UChar* _FileData, string _DescribeFlag, UINT _Index, UINT _SumSize
	Json::FastWriter fast_writer;
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, " HttpInterface body:%s", body.c_str());

	int szActionType = 0;
	Json::Reader  reader;
	Json::Value szRoot, szHeader, szPayLoad, szTempValue, szData;
	char szMessageID[33] = { 0 };
	bool isParse = reader.parse(body, szTempValue, false);
	if (isParse)
	{

		szHeader["name"] = "Version";
		szHeader["messageId"] = g_ServerManager->g_HttpServerManager.BuildGuid(szMessageID);
		szHeader["namespace"] = ___FACK___JSON__VERSION;
		szHeader["payLoadVersion"] = ___FACK___JSON__VWESION;
		szHeader["code"] = Json::Value(HTTP_ERROR_SUCCESS);
		//先下载文件
		
		if (g_ServerManager->g_HttpClientManager.DownLoadFile(szTempValue["fileName"].asString(), static_cast<UINT>(szTempValue["fileCode"].asUInt())) > 0)
		{
			g_ServerManager->g_VersionManager.LoadVerFile(
				szTempValue["fileName"].asString(),
				static_cast<UINT>(szTempValue["fileCode"].asUInt()),
				static_cast<UINT>(szTempValue["sumSize"].asUInt())
			);
		}
			
		else
		{
			szHeader["code"] = Json::Value(HTTP_ERROR_FAIL);
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "DownLoad File Fail File name = %s", szTempValue["fileName"].asString());

		}
		

	}
	else
	{
		szHeader["code"] = Json::Value(HTTP_ERROR_NOSERIALIZE);
	}
	szRoot["header"] = szHeader;
	szRoot["payload"] = szPayLoad;

	Json::ValueType type = szRoot.type();
	string SendStr = fast_writer.write(szRoot);
	HttpServer::SendRsp(c, SendStr);
	return true;
}



bool CHttpServerManager::QueueAlarmRecord(std::string url, std::string body, std::string param, mg_connection *c, OnRspCallback rsp_callback)
{
	Json::FastWriter fast_writer;
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, " HttpInterface body:%s", body.c_str());

	int szActionType = 0;
	Json::Reader  reader;
	Json::Value szRoot, szHeader, szPayLoad, szTempValue, szData;
	char szMessageID[33] = { 0 };
	bool isParse = reader.parse(body, szTempValue, false);
	if (isParse)
	{
		    char szMessageID[33] = { 0 };
			szHeader["name"] = "Alarm";
			szHeader["messageId"] = g_ServerManager->g_HttpServerManager.BuildGuid(szMessageID);
			szHeader["namespace"] = ___FACK___JSON__ALARM;
			szHeader["payLoadVersion"] = ___FACK___JSON__VWESION;
			szHeader["code"] = Json::Value(HTTP_ERROR_SUCCESS);

			g_ServerManager->g_DeviceAlarmManager.QueueData(szTempValue["mac"].asString().c_str(),
				szPayLoad, szTempValue["page"].asInt(), szTempValue["pageSize"].asInt());
	
	}
	else
	{
		szHeader["code"] = Json::Value(HTTP_ERROR_NOSERIALIZE);
	}
	szRoot["header"] = szHeader;
	szRoot["payload"] = szPayLoad;

	Json::ValueType type = szRoot.type();
	string SendStr = fast_writer.write(szRoot);
	HttpServer::SendRsp(c, SendStr);
	return true;
}


bool CHttpServerManager::TmallCtl(std::string url, std::string body, std::string param, mg_connection *c, OnRspCallback rsp_callback)
{
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, " HttpInterface param:%s", param.c_str());
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, " HttpInterface body:%s", body.c_str());
	Json::FastWriter fast_writer;
	Json::Value szRoot, szHeader, szPayLoad, szTempValue, szData;
	char szMessageID[33] = { 0 };
	szHeader["messageId"] = g_ServerManager->g_HttpServerManager.BuildGuid(szMessageID);
	szHeader["namespace"] = "test";
	szHeader["payLoadVersion"] = 1;
	szHeader["code"] = Json::Value(HTTP_ERROR_SUCCESS);

	
	Json::Reader  reader;
	bool isParse = reader.parse(body, szTempValue, false);
	if (isParse)
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, " ControlType:%s", szTempValue["ControlType"].asString().c_str());
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, " DeviceID:%u", szTempValue["DeviceID"].asUInt());
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, " DeviceMac:%s", szTempValue["DeviceMac"].asString().c_str());
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, " DeviceType:%u", szTempValue["DeviceType"].asUInt());
		szHeader["name"] = g_ServerManager->g_TmallManager.PublishTmallCmd(szTempValue["ControlType"].asString(),
			szTempValue["DeviceID"].asUInt(),
			szTempValue["DeviceMac"].asString(),
			szTempValue["DeviceType"].asUInt());
	}
	else
	{
		szHeader["name"] = "INVALIDATE_PARAMS";
	}
	
	
	szRoot["header"] = szHeader;
	szRoot["payload"] = szPayLoad;

	
	string SendStr = fast_writer.write(szRoot);
	HttpServer::SendRsp(c, SendStr);
	return true;
}
bool CHttpServerManager::InitHttp()
{
	
	std::string port = "8087";
	m_HttpServer = std::shared_ptr<HttpServer>(new HttpServer);

	
	m_HttpServer->Init(port);
	m_HttpServer->AddHandler("/api/QueueJgPush", QueueJgPush);
	m_HttpServer->AddHandler("/api/SubmitRecord", SubmitRecord);
	m_HttpServer->AddHandler("/api/SubmitVersion", SubmitVersion);
	m_HttpServer->AddHandler("/api/QueueAlarmRecord", QueueAlarmRecord);
	m_HttpServer->AddHandler("/api/TmallCtl", TmallCtl);

	g_ServerManager->g_WechatManager.Init();
	
	m_HttpServer->Start();

}