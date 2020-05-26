#pragma once
#ifndef _MQTTMANAGER_H_
#define _MQTTMANAGER_H_

class CMQTTManager:public CMqttObject
{
public:
	CMQTTManager() {};
	~CMQTTManager() {};

public:
	static void Delivered(void *_context, MQTTClient_deliveryToken _dt);
	static int Msgarrvd(void *_context, char *_topicName, int _topicLen, MQTTClient_message *_message);
	static void Connlost(void *_context, char *_cause);

	void CloseMqtt();

	bool InitMqtt(char * _ConnectAdr,int _KeepAliveInterval,int _Cleansession,char *_Name,char* _Password, char * _ClientID);

	
	bool Publish(string _Topic,char * _Data,int _len);
	bool Subscribe(string _Topic, int _QOS);


	bool OnTimer();
	bool MqttCreate();

	bool ReadConfig();

	//======================================
	/*********************
	*设备上报告警
	************************/
	virtual bool InitPublish();

private:
	//消息队列
	map<MQTTClient_deliveryToken, CMqttMessage*> m_DeliveryMessage;
	//消息Token内存池
	objectPool_CC<CMqttMessage> m_MqttMessagePool;
	
};

#endif
