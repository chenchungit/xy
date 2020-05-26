#pragma once
#ifndef _HTTPSERVERMANAGER_H_
#define _HTTPSERVERMANAGER_H_
using namespace std;
class CHttpServerManager:public CDataBaseObject
{
public:
	/*********************
	*初始化
	************************/
	virtual void Initialize() {};

	/*********************
	*读取数据库
	************************/
	virtual bool ReadDB() { return 0; };

	virtual bool JgPush(Json::Value &_Root) { return true; };
public:

	static bool handle_fun1(std::string url, std::string body, std::string param, mg_connection *c, OnRspCallback rsp_callback);


	//查询告警日志
	static bool SubmitVersion(std::string url, std::string body, std::string param, mg_connection *c, OnRspCallback rsp_callback);
	//查询告警日志
	static bool QueueAlarmRecord(std::string url, std::string body, std::string param, mg_connection *c, OnRspCallback rsp_callback);

	//查询使用日志
	static bool QueueUseRecord(std::string url, std::string body, std::string param, mg_connection *c, OnRspCallback rsp_callback);

	//提交极光推送
	static bool SubmitRecord(std::string url, std::string body, std::string param, mg_connection *c, OnRspCallback rsp_callback);
	//查询极光推送
	static bool QueueJgPush(std::string url, std::string body, std::string param, mg_connection *c, OnRspCallback rsp_callback);



	static bool TmallCtl(std::string url, std::string body, std::string param, mg_connection *c, OnRspCallback rsp_callback);
	bool InitHttp();
	

public:
	std::shared_ptr<HttpServer> m_HttpServer;
};

#endif