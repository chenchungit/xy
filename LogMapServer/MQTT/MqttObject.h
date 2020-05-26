#pragma once
#ifndef _MQTTOBJECT_H_
#define _MQTTOBJECT_H_

class CMqttMessage
{
public:
	CMqttMessage() {
	};
	~CMqttMessage() {};
	inline void setMessage(char* _msg,int _Len) {
		RtlCopyMemory(m_MqttMsg, _msg, _Len);
		m_MsgLen = _Len;
	};
	inline void setTopic(const char * _Topic, int _Len) {
		RtlCopyMemory(m_Topic, _Topic, _Len);
		m_TopicLen = _Len;
	};

	inline char* getMessage() {
		return m_MqttMsg;
	};
	inline char*  getTopic(int &_Len) {
		return m_Topic;
	};
	inline void  Init() {
		//m_MQTTClient_message = MQTTClient_message_initializer;
		RtlZeroMemory(m_Topic, MAX_BUFF_LOG_SIZE);
		RtlZeroMemory(m_MqttMsg, MAX_BUFF_NETPAKAGESIZE);
		m_TopicLen = m_MsgLen = 0;
	};
private:
	//MQTTClient_message m_MQTTClient_message;
	int m_TopicLen,m_MsgLen;
	char m_Topic[MAX_BUFF_LOG_SIZE];
	char m_MqttMsg[MAX_BUFF_NETPAKAGESIZE];
};

class CMqttObject
{
public:
	CMqttObject() {};
	~CMqttObject() {};
protected:
	//连接参数
	MQTTClient_connectOptions m_Options;

	//连接句柄
	MQTTClient m_Client;
	//Token
	CC::ThreadMutex m_TokenMutex;
	char m_MqttConnectAddress[1024];
	char m_Name[64];
	char m_Psw[64];
	char m_ClientID[64];
	int m_KeepAliveInterval;
	int m_Cleansession;
};
#endif
