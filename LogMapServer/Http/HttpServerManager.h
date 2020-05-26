#pragma once
#ifndef _HTTPSERVERMANAGER_H_
#define _HTTPSERVERMANAGER_H_
using namespace std;
class CHttpServerManager:public CDataBaseObject
{
public:
	/*********************
	*��ʼ��
	************************/
	virtual void Initialize() {};

	/*********************
	*��ȡ���ݿ�
	************************/
	virtual bool ReadDB() { return 0; };

	virtual bool JgPush(Json::Value &_Root) { return true; };
public:

	static bool handle_fun1(std::string url, std::string body, std::string param, mg_connection *c, OnRspCallback rsp_callback);


	//��ѯ�澯��־
	static bool SubmitVersion(std::string url, std::string body, std::string param, mg_connection *c, OnRspCallback rsp_callback);
	//��ѯ�澯��־
	static bool QueueAlarmRecord(std::string url, std::string body, std::string param, mg_connection *c, OnRspCallback rsp_callback);

	//��ѯʹ����־
	static bool QueueUseRecord(std::string url, std::string body, std::string param, mg_connection *c, OnRspCallback rsp_callback);

	//�ύ��������
	static bool SubmitRecord(std::string url, std::string body, std::string param, mg_connection *c, OnRspCallback rsp_callback);
	//��ѯ��������
	static bool QueueJgPush(std::string url, std::string body, std::string param, mg_connection *c, OnRspCallback rsp_callback);



	static bool TmallCtl(std::string url, std::string body, std::string param, mg_connection *c, OnRspCallback rsp_callback);
	bool InitHttp();
	

public:
	std::shared_ptr<HttpServer> m_HttpServer;
};

#endif