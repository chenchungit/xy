
#pragma once
#ifndef _SERVERMANAGER_H_
#define _SERVERMANAGER_H_
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include "SystemMessage.h"
#include "LConfigManager.h"
#include "DataBaseObject.h"
#include "ServerParamObject.h"
#include "Crc32.h"

//����
#include "SocketService.h"
#include "TcpSocketServer.h"

//
#include "RedisObject.h"
#include "RedisClientManager.h"

#include "ServiceObject.h"
#include "DataFuntionMap.h"
#include "ServiceManager.h"

#include "HttpClientManager.h"
#include "HttpServer.h"
#include "HttpServerManager.h"

#include "DeviceUuidObject.h"
#include "DeviceUuidManager.h"

#include "AnalsysNetData.h"
//#include "PasswordManager.h"





#include "JgPushRecordObject.h"
#include "JgPushRecordManager.h"
//#include "MqttObject.h"
//#include "MqttManager.h"

#include "UseRecordObject.h"
#include "UseRecordManager.h"

#include "DeviceAlarmObject.h"
#include "DeviceAlarmManager.h"

#include "MqttObject.h"
#include "MqttManager.h"
#include "VersionObject.h"
#include "VersionManager.h"

#include "TmallObject.h"
#include "TmallManager.h"


#include "WechatObject.h"
#include "WechatRoomObject.h"
#include "WechatManager.h"

class CServerManager :public CSystemMessage, public IMysqlInterFace, public IAnalysisSystemMessgae,public CServerParamObject
{
public:
	CServerManager();
	virtual ~CServerManager() {};

	void InitServer();

	//db
public:
	void InitMysql();
	void InitRedis();
	/*************************Parameter
	*��ʼ������
	*************************/
	void InitDataBase();
	virtual bool OnConnected() { return true; };
	virtual int32_t  OnDBErrorMessage(const char* _EMessage);
	//////////////////////////////////////////////////////////////////////////
	static void OnTimer(int _single);
	//////////////////////////////////////////////////////////////////////////
	void InitAnalysisPacket();
	//////////////////////////////////////////////////////////////////////////
	void InitSocket();
	bool ReadConfig();
	virtual int32_t  OnProtocolErrorMessage(TcpSocketID id, char* _EMessage);

	/*************************Parameter
	*��ȡЭ������ӿ�
	*************************/
	IAnalysisPacket * getIAnalysisPacket() { return m_IAnalysisPacket; };

	/****************
	*����Ͽ�������
	******************/
	void UnSocketClose(TcpSocketID id);

	/****************
	*��������
	******************/
	void SocketHeart(TcpSocketID id);

	void InitMqttServer();
public:
	IMysql *g_IMysqlInterFace;
public:
	CJgPushRecordManager g_JgPushRecordManager;
	CUseRecordManager g_UseRecordManager;
	CDeviceAlarmManager g_DeviceAlarmManager;
	CRedisClientManager g_RedisClientManager;
	CHttpClientManager g_HttpClientManager;
	CDeviceUuidManager g_DeviceUuidManager;
	CTmallManager g_TmallManager;
	CWechatManager g_WechatManager;
	CCrc32 g_Crc32;
private:
	//Э���������
	IAnalysisPacket *m_IAnalysisPacket;

public:
	//����
	//CCTConnectServer * g_ConnectServer;
	
	//���ݷ���
	CAnalsysNetData *g_AnalsysNetData;
	//�����ļ�
	CLConfigManager g_ConfigManager;

	//Http
	CHttpServerManager g_HttpServerManager;
	//
	CServiceManager g_ServiceManager;
	//
	//�������ӿڶ���
	CSocketService *m_SocketService;
	//
	CMQTTManager g_MqttManager;
	//
	CVersionManager g_VersionManager;
public:
	//����Ŀ¼
	char m_SysPath[MAX_PATH + 1];
	//�����ļ�
	string m_ConfigName;
	
};
#endif

