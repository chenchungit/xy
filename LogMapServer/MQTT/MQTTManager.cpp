#include "ServerHead.h"


void CMQTTManager::Delivered(void *_context, MQTTClient_deliveryToken _dt)
{
	//Guard guard(&g_ServerManager->g_MqttManager.m_TokenMutex, true);
	/*map<MQTTClient_deliveryToken, CMqttMessage*>::iterator col = g_ServerManager->g_MqttManager.m_DeliveryMessage.find(_dt);
	if (col != g_ServerManager->g_MqttManager.m_DeliveryMessage.end())
	{
		g_ServerManager->g_MqttManager.m_MqttMessagePool._free_boost(col->second);
		g_ServerManager->g_MqttManager.m_DeliveryMessage.erase(col);
	}*/
}
int CMQTTManager::Msgarrvd(void *_context, char *_topicName, int _topicLen, MQTTClient_message *_message)
{
	stringstream szStreamTopic;
	char szData[10] = { 0 };
	for (int i = 0;i< _message->payloadlen;i++)
	{
		RtlZeroMemory(szData, 10);
		sprintf(szData, "%02X", ((UChar*)_message->payload)[i]);
		szStreamTopic << szData;
	}
	//MQTT topicName[Nexless/v2/Req/SmartHome/DC2C2800EBF7/Dft/Dft/0/LOCK/0/0][872416536]
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "================MQTT Msgarrvd[%s][%d]================",
		szStreamTopic.str().c_str(), _message->payloadlen);
	//uint32_t szType = 0;
	//memcpy(&szType, _message->payload, sizeof(uint32_t));

	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "================MQTT topicName[%s]================", _topicName);

	
	g_ServerManager->g_AnalsysNetData->InsertMQTTDataQueue(_topicName,(unsigned char*)_message->payload, _message->payloadlen);


	
	MQTTClient_freeMessage(&_message);
	MQTTClient_free(_topicName);
	return 1;
}
void CMQTTManager::Connlost(void *_context, char *_cause)
{
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, " MQTT Client Connection close[%s]", _cause);
}
void CMQTTManager::CloseMqtt()
{
	if (MQTTClient_isConnected(m_Client))
	{
		MQTTClient_disconnect(m_Client, 10000);

	}
	MQTTClient_destroy(&m_Client);
}
bool CMQTTManager::MqttCreate()
{
	int szState = MQTTClient_create(&m_Client, m_MqttConnectAddress, m_ClientID,
		MQTTCLIENT_PERSISTENCE_NONE, NULL);
	if (szState != MQTTCLIENT_SUCCESS)
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "Create MQTT Client Fail");
		return false;
	}
	else
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "Create MQTT Client Success");
	}
	m_Options = MQTTClient_connectOptions_initializer;
	m_Options.keepAliveInterval = m_KeepAliveInterval;
	m_Options.cleansession = m_Cleansession;
	m_Options.username = m_Name;
	m_Options.password = m_Psw;//paho-mqtt3a paho-mqttpp3 paho-mqtt3c
	//m_Options.MQTTVersion = MQTTVERSION_DEFAULT;


	szState = MQTTClient_setCallbacks(m_Client, NULL, Connlost, Msgarrvd, Delivered);
	if (szState != MQTTCLIENT_SUCCESS)
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "set MQTT call back Fail");
		CloseMqtt();
		return false;
	}
	if ((szState = MQTTClient_connect(m_Client, &m_Options)) != MQTTCLIENT_SUCCESS)
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL,
			"Connect MQTT Service Fail[error code = %d][%s][%d][%d]", szState,
			m_MqttConnectAddress, m_Cleansession, m_KeepAliveInterval);
		CloseMqtt();
		return false;
	}
	return true;
}
bool CMQTTManager::InitMqtt(char * _ConnectAdr, int _KeepAliveInterval, int _Cleansession, char *_Name, char* _Password, char * _ClientID)
{
	RtlZeroMemory(m_MqttConnectAddress, 1024);
	RtlZeroMemory(m_Name, 64);
	RtlZeroMemory(m_Psw, 64);
	RtlZeroMemory(m_ClientID, 64);
	ReadConfig();
	/*RtlCopyMemory(m_MqttConnectAddress, _ConnectAdr, strlen(_ConnectAdr));
	RtlCopyMemory(m_Name, _Name, strlen(_Name));
	RtlCopyMemory(m_Psw, _Password, strlen(_Password));
	RtlCopyMemory(m_ClientID, _ClientID, strlen(_ClientID));
	m_KeepAliveInterval = _KeepAliveInterval;
	m_Cleansession = _Cleansession;*/
	return MqttCreate();

}

bool CMQTTManager::Publish(string _Topic, char * _Data, int _len)
{
	if (!MQTTClient_isConnected(m_Client))
		return false;
	//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "Publish data len [%s][%d]",  _Topic.c_str(),_len);
	MQTTClient_deliveryToken szMQTTClient_deliveryToken;
	//char szData[] = { "123456789" };

	MQTTClient_message szMsg = MQTTClient_message_initializer;
	szMsg.payload = _Data;
	szMsg.payloadlen = _len;
	szMsg.qos = QOS_2;
	szMsg.retained = 0;
	int szState = MQTTClient_publishMessage(m_Client, _Topic.c_str(), &szMsg, &szMQTTClient_deliveryToken);

	if (szState != MQTTCLIENT_SUCCESS)
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, static_cast<const char*>("MQTT Client publish Message Fail"));
		//CloseMqtt();
		return false;
	}
	szState = MQTTClient_waitForCompletion(m_Client, szMQTTClient_deliveryToken, MQTT_TIMEOUT);//&_msg, &szMQTTClient_deliveryToken
	if (szState != MQTTCLIENT_SUCCESS)
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, static_cast<const char*>("MQTT Client wait For Completion Message Fail"));
		//CloseMqtt();
		return false;
	}


	/*CMqttMessage *szMqttMessage = m_MqttMessagePool._malloc_boost();
	szMqttMessage->Init();
	szMqttMessage->setMessage(_Data, _len);
	szMqttMessage->setTopic(_Topic.c_str(), static_cast<int>(_Topic.length()));
	m_DeliveryMessage[szMQTTClient_deliveryToken] = szMqttMessage;*/
	return true;
}

bool CMQTTManager::Subscribe(string _Topic, int _QOS)
{
	if (!MQTTClient_isConnected(m_Client))
		return false;
	int szState = MQTTClient_subscribe(m_Client, _Topic.c_str(), _QOS);
	if (szState != MQTTCLIENT_SUCCESS)
	{
		CloseMqtt();
		return false;
	}
	return true;
}
bool CMQTTManager::OnTimer()
{
	if (!MQTTClient_isConnected(m_Client))
	{
		CloseMqtt();
		MqttCreate() ?
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, static_cast<const char*>("MQTT Reconnect Seccuss"))
			: g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, static_cast<const char*>("MQTT Reconnect Fail"));
		if (MQTTClient_isConnected(m_Client))
		{
			g_ServerManager->g_MqttManager.InitPublish();
		}
	}
	return true;
}

bool CMQTTManager::ReadConfig()
{
	g_ServerManager->g_ConfigManager.OpenFile("./config/LogMapServer.ini", "r");
	m_KeepAliveInterval = g_ServerManager->g_ConfigManager.GetInt("MQTT", "keepalive");
	m_Cleansession = g_ServerManager->g_ConfigManager.GetInt("MQTT", "cleansession");;
	memcpy(m_MqttConnectAddress, g_ServerManager->g_ConfigManager.GetStr("MQTT", "Adr"), strlen(g_ServerManager->g_ConfigManager.GetStr("MQTT", "Adr")));
	memcpy(m_Name, g_ServerManager->g_ConfigManager.GetStr("MQTT", "name"), strlen(g_ServerManager->g_ConfigManager.GetStr("MQTT", "name")));
	memcpy(m_Psw, g_ServerManager->g_ConfigManager.GetStr("MQTT", "psw"), strlen(g_ServerManager->g_ConfigManager.GetStr("MQTT", "psw")));
	memcpy(m_ClientID, g_ServerManager->g_ConfigManager.GetStr("MQTT", "clientID"), strlen(g_ServerManager->g_ConfigManager.GetStr("MQTT", "clientID")));
	g_ServerManager->g_ConfigManager.CloseFile();

	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_SYSTEM,
		static_cast<const char*>("Adr[%s] name[%s] psw [%s]clientID[%s]"), m_MqttConnectAddress, m_Name, m_Psw, m_ClientID);
	return true;
}


bool CMQTTManager::InitPublish()
{
	//¶©ÔÄ¸æ¾¯
	if (g_ServerManager->g_MqttManager.Subscribe(SERVER_TOPIC_ALARM, QOS_2))
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "MQTT Vians/v3/Req/SmartHome/+/+/+/+/ALARM/+/+ Subscribe Success...");
	else
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "MQTT Vians/v3/Req/SmartHome/+/+/+/+/ALARM/+/+ Subscribe Fail...");

	//¶©ÔÄËø
	if (g_ServerManager->g_MqttManager.Subscribe(SERVER_TOPIC_LOCK, QOS_2))
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "MQTT Vians/v3/Req/SmartHome/+/+/+/+/LOCK/+/+ Subscribe Success...");
	else
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "MQTT Vians/v3/Req/SmartHome/+/+/+/+/LOCK/+/+ Subscribe Fail...");

	//¶©ÔÄÍø¹Ø
	if (g_ServerManager->g_MqttManager.Subscribe(SERVER_TOPIC_GATEWAY, QOS_2))
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "MQTT Nexless/v2/Req/SmartHome/+/+/+/+/GATEWAY/+/+ Subscribe Success...");
	else
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "MQTT Nexless/v2/Req/SmartHome/+/+/+/+/GATEWAY/+/+ Subscribe Fail...");


	//¶©ÔÄÃÅ´Å
	if (g_ServerManager->g_MqttManager.Subscribe(SERVER_TOPIC_DOORALARM, QOS_2))
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "MQTT Vians/v3/Req/SmartHome/+/+/+/+/DOOR_ALARM/+/+ Subscribe Success...");
	else
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "MQTT Vians/v3/Req/SmartHome/+/+/+/+/DOOR_ALARM/+/+ Subscribe Fail...");
	//¶©ÔÄÑÌ¸Ð
	if (g_ServerManager->g_MqttManager.Subscribe(SERVER_TOPIC_SMOKEALARM, QOS_2))
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "MQTT Vians/v3/Req/SmartHome/+/+/+/+/SMOKE_ALARM/+/+ Subscribe Success...");
	else
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "MQTT Vians/v3/Req/SmartHome/+/+/+/+/SMOKE_ALARM/+/+ Subscribe Fail...");

	//¶©ÔÄ¼ÆÊýÆ÷
	if (g_ServerManager->g_MqttManager.Subscribe(SERVER_TOPIC_HUMANCNT, QOS_2))
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "MQTT Vians/v3/Req/SmartHome/+/+/+/+/HUMAN_CNT/+/+ Subscribe Success...");
	else
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "MQTT Vians/v3/Req/SmartHome/+/+/+/+/HUMAN_CNT/+/+ Subscribe Fail...");

	//¶©ÔÄ²âÊÔ
	if (g_ServerManager->g_MqttManager.Subscribe(SERVER_TOPIC_TEST, QOS_2))
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "MQTT Vians/v3/Req/SmartHome/+/+/+/+/Version/+/+ Subscribe Success...");
	else
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "MQTT Vians/v3/Req/SmartHome/+/+/+/+/Version/+/+ Subscribe Fail...");
	//È¼Æø±¨¾¯Æ÷
	if (g_ServerManager->g_MqttManager.Subscribe(SERVER_TOPIC_GASALARM, QOS_2))
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "MQTT Vians/v3/Req/SmartHome/+/+/+/+/GAS_ALARM/+/+ Subscribe Success...");
	else
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "MQTT Vians/v3/Req/SmartHome/+/+/+/+/GAS_ALARM/+/+ Subscribe Fail...");

	/*if (g_ServerManager->g_MqttManager.Subscribe(SERVER_TOPIC_REQ, QOS_2))
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "MQTT Nexless/v2/Cnf/SmartHome/# Subscribe Success...");
	else
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_SYSTEM, "MQTT Nexless/v2/Cnf/SmartHome/# Subscribe Fail...");*/
}