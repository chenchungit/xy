#include "ServerHead.h"
uint32_t g_RoomID = 0;
//添加用户
void CWechatManager::WeChatLogin(const char * _Phone, mg_connection *_Con)
{
	CWechatObject *szWechatObject = NULL;
	map<string, CWechatObject*>::iterator col = m_dzWechatData.find(_Phone);
	if (col != m_dzWechatData.end())
	{
		szWechatObject = col->second;
	}
	else
	{
		szWechatObject = m_dzWechatPool._malloc_boost();
		m_dzWechatData[_Phone] = szWechatObject;
	}
	szWechatObject->setCon(_Con);
	szWechatObject->setPhone(_Phone);
	szWechatObject->setRoomID(0);
	/*if (strcmp("18423125481", _Phone) == 0)
	{
		g_RoomID++;
		SendRoomID(g_RoomID, _Con, "13996125913");
	}*/
}
void CWechatManager::SendRoomID(uint32_t _RoomID, mg_connection *_Con,const char * _Phone)
{
	Json::FastWriter fast_writer;
	Json::Value szRoot, szHeader, szPayLoad,szData;
	char szMessageID[33] = { 0 };
	szHeader["messageId"] = g_ServerManager->g_HttpServerManager.BuildGuid(szMessageID);
	szHeader["namespace"] = "WeChat";
	szHeader["payLoadVersion"] = 1;
	szHeader["code"] = WECHAT_ERROR_CODE_SUCCESS;
	szHeader["name"] = "weChat_Video";

	szData["roomID"] = _RoomID;
	szData["phone"] = _Phone;
	szPayLoad["data"] = szData;

	szRoot["header"] = szHeader;
	szRoot["payload"] = szPayLoad;

	//if(_Con->err)
	string SendStr = fast_writer.write(szRoot);
	if (SendStr.length() > 0)
	{
		char szSendData[SendStr.length() + 1];
		RtlZeroMemory(szSendData, SendStr.length() + 1);
		RtlCopyMemory(szSendData, SendStr.c_str(), SendStr.length());
		HttpServer::sendWebSocketData(_Con, szSendData, SendStr.length());
	}
	else
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "Send Room ID data fail");
}
void CWechatManager::SendErrorCode(uint32_t _Code, mg_connection *_Con)
{
	Json::FastWriter fast_writer;
	Json::Value szRoot, szHeader, szPayLoad;
	char szMessageID[33] = { 0 };
	szHeader["messageId"] = g_ServerManager->g_HttpServerManager.BuildGuid(szMessageID);
	szHeader["namespace"] = "WeChat";
	szHeader["payLoadVersion"] = 1;
	szHeader["code"] = Json::Value(_Code);
	szHeader["name"] = "weChat_Error";

	szPayLoad["data"] = Json::arrayValue;

	szRoot["header"] = szHeader;
	szRoot["payload"] = szPayLoad;

	
	string SendStr = fast_writer.write(szRoot);

	if (SendStr.length() > 0)
	{
		char szSendData[SendStr.length() + 1];
		RtlZeroMemory(szSendData, SendStr.length() + 1);
		RtlCopyMemory(szSendData, SendStr.c_str(), SendStr.length());
		HttpServer::sendWebSocketData(_Con, szSendData, SendStr.length());
	}
	else
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "Send Error Code data fail");
	
}
//申请视频
void CWechatManager::WeChatVedio(const char * _SrcPhone, mg_connection *_SrcCon, const char * _DrcPhone)
{
	g_RoomID++;
	CWechatObject *szSrcWechatObject = NULL;
	map<string, CWechatObject*>::iterator col = m_dzWechatData.find(_SrcPhone);
	if (col == m_dzWechatData.end())
	{
		SendErrorCode(WECHAT_ERROR_CODE_NOLOGIN, _SrcCon);
		return;
	}
	else
		szSrcWechatObject = col->second;
	CWechatObject *szDrcWechatObject = NULL;
	col = m_dzWechatData.find(_DrcPhone);
	if (col == m_dzWechatData.end())
	{
		SendErrorCode(WECHAT_ERROR_CODE_NOLOGIN_TARGET, _SrcCon);
		return;
	}
	else
		szDrcWechatObject = col->second;

	SendRoomID(g_RoomID, _SrcCon, _SrcPhone);
	szSrcWechatObject->setRoomID(g_RoomID);
	SendRoomID(g_RoomID, col->second->getCon(), _SrcPhone);
}
void CWechatManager::JoinWeChatVedio(const char * _Phone, mg_connection *_Con, uint32_t _RoomID)
{
	map<string, CWechatObject*>::iterator col = m_dzWechatData.find(_Phone);
	if (col != m_dzWechatData.end())
	{
		CWechatObject *szSrcWechatObject = col->second;
		szSrcWechatObject->setRoomID(_RoomID);
	}
}


bool CWechatManager::WeChatLoginFace(std::string _data, mg_connection *_Con)
{
	Json::Reader  reader;
	Json::Value szPalyloadValue;
	bool isParse = reader.parse(_data, szPalyloadValue, false);
	if (isParse)
	{
		g_ServerManager->g_WechatManager.WeChatLogin(szPalyloadValue["phone"].asString().c_str(), _Con);
	}
	else
	{
		g_ServerManager->g_WechatManager.SendErrorCode(HTTP_ERROR_NOSERIALIZE, _Con);
	}
	return true;
}
bool CWechatManager::WeChatVedioFace(std::string _data, mg_connection *_Con)
{

	Json::Reader  reader;
	Json::Value szPalyloadValue;
	bool isParse = reader.parse(_data, szPalyloadValue, false);
	if (isParse)
	{
		g_ServerManager->g_WechatManager.WeChatVedio(szPalyloadValue["srcPhone"].asString().c_str(), _Con, szPalyloadValue["drcPhone"].asString().c_str());
	}
	else
	{
		g_ServerManager->g_WechatManager.SendErrorCode(HTTP_ERROR_NOSERIALIZE, _Con);
	}
	return true;
}
bool CWechatManager::JoinWeChatVedioFace(std::string _data, mg_connection *_Con)
{
	Json::Reader  reader;
	Json::Value szPalyloadValue;
	bool isParse = reader.parse(_data, szPalyloadValue, false);
	if (isParse)
	{
		g_ServerManager->g_WechatManager.JoinWeChatVedio(szPalyloadValue["phone"].asString().c_str(), _Con, szPalyloadValue["roomID"].asUInt());
	}
	else
	{
		g_ServerManager->g_WechatManager.SendErrorCode(HTTP_ERROR_NOSERIALIZE, _Con);
	}
	return true;
}


bool CWechatManager::WeChatOauthLockFace(std::string _data, mg_connection *_Con)
{
	Json::Reader  reader;
	Json::Value szPalyloadValue;
	bool isParse = reader.parse(_data, szPalyloadValue, false);
	if (isParse)
	{
		g_ServerManager->g_WechatManager.WeChatOauthLock(szPalyloadValue["phone"].asString().c_str(),
			szPalyloadValue["key"].asString().c_str(), szPalyloadValue["deviceName"].asString().c_str(), _Con);
	}
	else
	{
		g_ServerManager->g_WechatManager.SendErrorCode(HTTP_ERROR_NOSERIALIZE, _Con);
	}
	return true;
}
void CWechatManager::WeChatOauthLock(const char * _Phone, const char * _LockKey, const char * _DeviceName, mg_connection *_Con)
{
	uint32_t szRoomID = 0;
	map<string, CWechatObject*>::iterator col = m_dzWechatData.find(_Phone);
	if (col != m_dzWechatData.end())
	{
		CWechatObject *szSrcWechatObject = col->second;
		szRoomID = szSrcWechatObject->getRoomID();
	}
	else
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "not find user");
	}
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "count [%d]phone[%s] room id[%d]", m_dzWechatData.size(), _Phone, szRoomID);
	if (szRoomID > 0)
	{
		col = m_dzWechatData.begin();
		for (; col != m_dzWechatData.end();col++)
		{
			if (col->second->getRoomID() == szRoomID)
			{
				g_ServerManager->g_WechatManager.SendLockKey(_Phone,_LockKey, _DeviceName, col->second->getCon());
			}
		}
	}
	else
	{
		g_ServerManager->g_WechatManager.SendErrorCode(WECHAT_ERROR_CODE_NOUSERORROOM, _Con);
	}
}

void CWechatManager::SendLockKey(const char * _Phone, const char * _LockKey, const char * _DeviceName, mg_connection *_Con)
{
	Json::FastWriter fast_writer;
	Json::Value szRoot, szHeader, szPayLoad, szData;
	char szMessageID[33] = { 0 };
	szHeader["messageId"] = g_ServerManager->g_HttpServerManager.BuildGuid(szMessageID);
	szHeader["namespace"] = "WeChat";
	szHeader["payLoadVersion"] = 1;
	szHeader["code"] = WECHAT_ERROR_CODE_SUCCESS;
	szHeader["name"] = "weChat_OpenLock";

	szData["key"] = _LockKey;
	szData["deviceName"] = _DeviceName;
	szData["phone"] = _Phone;
	szPayLoad["data"] = szData;

	szRoot["header"] = szHeader;
	szRoot["payload"] = szPayLoad;

	
	string SendStr = fast_writer.write(szRoot);
	if (SendStr.length() > 0)
	{
		char szSendData[SendStr.length() + 1];
		RtlZeroMemory(szSendData, SendStr.length() + 1);
		RtlCopyMemory(szSendData, SendStr.c_str(), SendStr.length());
		HttpServer::sendWebSocketData(_Con, szSendData, SendStr.length());
	}
	else
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "Send Lock Key data fail");
	//HttpServer::sendWebSocketData(_Con, (char*)SendStr.c_str(), SendStr.length());
}



bool CWechatManager::WeChatLogoutFace(std::string _data, mg_connection *_Con)
{
	Json::Reader  reader;
	Json::Value szPalyloadValue;
	bool isParse = reader.parse(_data, szPalyloadValue, false);
	if (isParse)
	{
		g_ServerManager->g_WechatManager.WeChatLogout(szPalyloadValue["phone"].asString().c_str());
	}
	else
	{
		g_ServerManager->g_WechatManager.SendErrorCode(HTTP_ERROR_NOSERIALIZE, _Con);
	}
	return true;
}
void CWechatManager::WeChatLogout(const char * _Phone)
{
	CWechatObject *szSrcWechatObject = 0;
	uint32_t szRoomID = 0;
	map<string, CWechatObject*>::iterator col = m_dzWechatData.find(_Phone);
	if (col != m_dzWechatData.end())
	{
		szSrcWechatObject = col->second;
		szRoomID = szSrcWechatObject->getRoomID();
		szSrcWechatObject->setRoomID(0);
	}
	else
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "not find user");
	}
	if (szRoomID > 0)
	{
		col = m_dzWechatData.begin();
		for (; col != m_dzWechatData.end(); col++)
		{
			if (col->second->getRoomID() == szRoomID)
			{
				g_ServerManager->g_WechatManager.SendLogout(col->second->getCon());
			}
		}
	}
}
void CWechatManager::SendLogout(mg_connection *_Con)
{
	Json::FastWriter fast_writer;
	Json::Value szRoot, szHeader, szPayLoad;
	char szMessageID[33] = { 0 };
	szHeader["messageId"] = g_ServerManager->g_HttpServerManager.BuildGuid(szMessageID);
	szHeader["namespace"] = "WeChat";
	szHeader["payLoadVersion"] = 1;
	szHeader["code"] = WECHAT_ERROR_CODE_SUCCESS;
	szHeader["name"] = "weChat_LogOut";

	szPayLoad["data"] = Json::arrayValue;

	szRoot["header"] = szHeader;
	szRoot["payload"] = szPayLoad;

	string SendStr = fast_writer.write(szRoot);
	if (SendStr.length() > 0)
	{
		char szSendData[SendStr.length() + 1];
		RtlZeroMemory(szSendData, SendStr.length() + 1);
		RtlCopyMemory(szSendData, SendStr.c_str(), SendStr.length());
		HttpServer::sendWebSocketData(_Con, szSendData, SendStr.length());
	}
	else
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "Send Room ID data fail");

	
//	HttpServer::sendWebSocketData(_Con, (char*)SendStr.c_str(), SendStr.length());
}
void CWechatManager::Init()
{

	/*FILE *fp = fopen("./Mongoose.txt", "ab+");
	if (fp)
		cs_log_set_file(fp);
	else
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "setting mongoose log fail");*/
	//webSock
	g_ServerManager->g_HttpServerManager.m_HttpServer->AddWebSocketCallBack("weChat_Login", WeChatLoginFace);
	g_ServerManager->g_HttpServerManager.m_HttpServer->AddWebSocketCallBack("weChat_Join", JoinWeChatVedioFace);
	g_ServerManager->g_HttpServerManager.m_HttpServer->AddWebSocketCallBack("weChat_Vedio", WeChatVedioFace);
	g_ServerManager->g_HttpServerManager.m_HttpServer->AddWebSocketCallBack("weChat_OpenLock", WeChatOauthLockFace);
	g_ServerManager->g_HttpServerManager.m_HttpServer->AddWebSocketCallBack("weChat_LogOut", WeChatLogoutFace);
}