#include "ServerHead.h"

CServerManager::CServerManager()
{
	
}

void CServerManager::InitServer()
{
	//��ʼ����־�ӿ�
	InitLogInterFace("HttpService.log");
	//��ȡ�����ļ�
	ReadConfig();
	printf("=====Init Mysql======\r\n");
	//��ʼ�����ݿ�
	InitMysql();
	printf("=====Init data base======\r\n");
	
	//��ʼ�����ݿ����
	InitDataBase();
	printf("=====Init Redis======\r\n");
	//��ʼ��redis
	InitRedis();
	printf("=====Init InitAnalysisPacket======\r\n");
	/*g_HttpClientManager.SendReq("https://cdn.i.xeiot.com/firmware/stm32gateway/Stm32MqttGw.upg");

	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "-------------------------------------------------");
	sleep(20);*/
	//==================================
	//��ʼ��������
	InitAnalysisPacket();
	//��ʼ���ص�����
	g_ServiceManager.InitService();
	//���ݵĽ���
	g_AnalsysNetData = new CAnalsysNetData();
	//
	InitMqttServer();
	
	//��ʼ������
	InitSocket();
	//g_VersionManager.LoadVerFile("./Version/Stm32MqttGw.upg", 1, 205990);
	signal(SIGALRM, OnTimer);
	alarm(1);
	g_HttpClientManager.Init();
	g_HttpClientManager.setFilePath("./Version/");
	g_VersionManager.Initialize();


	//////////////////////////////////////////////////////////////////////////
	/*UChar szTest[] = { "tuJcuEFveBHMFh0MIK4Mj3sF49AU9WtKD//UyQ==" };
	UChar szTempData[1024] = {0};
	RtlZeroMemory(szTempData, 1024);
	size_t szTempDataLen = 1024;
	int state = base64_decode(szTempData, &szTempDataLen, szTest, 40);
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "test state = %d  [%s]", state, szTempData);*/
	//��ʼ��Http����
	if (g_HttpServerManager.InitHttp())
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "------>Http server success");
	}
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "------>Http server Fail");
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "******************************************************************");
	
}


void CServerManager::InitRedis()
{
	g_ConfigManager.OpenFile("./config/LogMapServer.ini", "r");

	int szTimeout, szDBIndex, szPort, szRedisID;
	char szRedisIP[32] = { 0 };
	char szRedisKey[64] = { 0 };
	RtlZeroMemory(szRedisIP, 32);
	RtlZeroMemory(szRedisKey, 64);
	memcpy(szRedisIP, g_ConfigManager.GetStr("Redis", "IP"), strlen(g_ConfigManager.GetStr("Redis", "IP")));
	szPort = g_ConfigManager.GetInt("Redis", "Port");
	szTimeout = g_ConfigManager.GetInt("Redis", "Timeout");
	szRedisID = g_ConfigManager.GetInt("Redis", "RedisID");
	szDBIndex = g_ConfigManager.GetInt("Redis", "DBIndex");
	char *szTemp = g_ConfigManager.GetStr("Redis", "Key");
	memcpy(szRedisKey, szTemp, strlen(szTemp));
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_SYSTEM, "-----[%s][%s]",
		szTemp, szRedisKey);
	if (g_RedisClientManager.InitRedis(szRedisIP, szPort, szDBIndex, szRedisKey))
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_SYSTEM, "init Redis success[%s][%d][%d][%d][%s]",
			szRedisIP, szTimeout, szDBIndex, szPort, szRedisKey);
	}
	else
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_SYSTEM, "init Redis fail[%s][%d][%d][%d][%s]",
			szRedisIP, szTimeout, szDBIndex, szPort, szRedisKey);
	g_ConfigManager.CloseFile();
}
void CServerManager::InitMqttServer()
{
	//��ʼ����������
	//jpushInit(&g_JPush, &g_JPushNotification);
	// ����˭�͵�ƽ̨
	//setPlatform(g_JPush, J_PUSH_PLATFORM_IOS | J_PUSH_PLATFORM_ANDROID);

	if (g_MqttManager.InitMqtt("tcp://192.168.1.112:1883", 20, 1, "admin", "123456", "HTTPServer_111"))
	{
		g_MqttManager.InitPublish();
	}
	else
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "MQTT Server Start Fail...");
	}

}
void CServerManager::InitSocket()
{
	m_SocketService = new CSocketService();
	TcpServerInitItem szItem;
	szItem.BuffBlockNum = 1024;
	szItem.BuffBlockSize = 1024 * 10;
	szItem.HeartTest = 100;
	szItem.InitSockets = 50;
	szItem.port = 5000;
	szItem.RSThreads = 8;

	m_SocketService->Init(&szItem);
	m_SocketService->Start();
	SystemCharacter("Start Server Monitoring");
	g_ThreadRun = true;


}

void CServerManager::InitAnalysisPacket()
{
	//��ʼ��Э�������
	m_IAnalysisPacket = CreateAnalsysNetDataImpl();
	////Э������ӿ�
	AnalysisItem szAnalysisItem;
	szAnalysisItem.pASMessage = this;
	m_IAnalysisPacket->InitAnalsysProtocol(&szAnalysisItem);

	
}
void CServerManager::InitMysql()
{
	g_ConfigManager.OpenFile("./config/DB.ini", "r");
	g_IMysqlInterFace  = CreateMysqlImpl();
	DBInitItem szDBInitItem;
	szDBInitItem.nSqlConCount = g_ConfigManager.GetInt("DBCount", "ConCount");
	szDBInitItem.nSqlPort = (unsigned short)g_ConfigManager.GetInt("DB1", "Port");
	szDBInitItem.nSqlKey = g_ConfigManager.GetInt("DB1", "key");
	szDBInitItem.pMysqlInterFace = this;
	memcpy(szDBInitItem.nSqlIP, g_ConfigManager.GetStr("DB1", "ip"),strlen(g_ConfigManager.GetStr("DB1", "ip")));//183.66.233.86
	memcpy(szDBInitItem.nSqlName, g_ConfigManager.GetStr("DB1", "DBName"), strlen(g_ConfigManager.GetStr("DB1", "DBName")));
	memcpy(szDBInitItem.nSqlUser, g_ConfigManager.GetStr("DB1", "root"), strlen(g_ConfigManager.GetStr("DB1", "root")));
	memcpy(szDBInitItem.nSqlPassword, g_ConfigManager.GetStr("DB1", "DBPassword"), strlen(g_ConfigManager.GetStr("DB1", "DBPassword")));
	//szDBInitItem.nSqlConCount = 10;
	g_IMysqlInterFace->StartDBConnet(&szDBInitItem);
	g_ServerManager->g_ConfigManager.CloseFile();
}
void CServerManager::InitDataBase()
{
	//g_JgPushRecordManager.Initialize();
	g_DeviceAlarmManager.Initialize();
	g_UseRecordManager.Initialize();
	g_DeviceUuidManager.Initialize();
	
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "=====Database Load over======");
}
int32_t  CServerManager::OnDBErrorMessage(const char* _EMessage)
{
	SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, (char*)_EMessage);
	return 0;
}

int32_t  CServerManager::OnProtocolErrorMessage(TcpSocketID id, char* _EMessage)
{
	SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "Protocol Message [%s]",(char*)_EMessage);
}
void CServerManager::OnTimer(int _single)
{
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL,"timer out..............");
	if (SIGALRM == _single)
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "Save DataBase..............");
		//g_ServerManager->g_TcpcManager.OnTimer();
		g_ServerManager->g_MqttManager.OnTimer();
	}
	alarm(TIMER_CORE);
}

void CServerManager::UnSocketClose(TcpSocketID id)
{
	//g_ServerManager->g_TcpcManager.UnSocketClose(id);
}

void CServerManager::SocketHeart(TcpSocketID id)
{
}


bool CServerManager::ReadConfig()
{
	g_ConfigManager.OpenFile("./config/LogMapServer.ini", "r");
	m_ServerID = g_ConfigManager.GetInt("SSocket", "ServerID");
	m_ServerType = g_ConfigManager.GetInt("SSocket", "ServerType");
	setIoStream(g_ConfigManager.GetInt("LogIO", "IOType"));
	m_Crypt = g_ConfigManager.GetInt("Crypt", "Encrypt");
 
	
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM,
		static_cast<const char*>("Server ID[%d] type[%d]"), m_ServerID, m_ServerType);
	g_ConfigManager.CloseFile();

return true;
}