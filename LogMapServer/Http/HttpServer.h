#pragma once
#ifndef _HTTPSERVER_H_
#define _HTTPSERVER_H_

// ����http����callback
typedef void OnRspCallback(mg_connection *c, std::string);
// ����http����handler
using ReqHandler = std::function<bool(std::string, std::string, std::string, mg_connection *c, OnRspCallback)>;

// ����WebSocket����handler
using WebSocketHandler = std::function<bool(std::string, mg_connection *c)>;

class HttpServer
{
public:
	HttpServer() {}
	~HttpServer() {}
	void Init(const std::string &port); // ��ʼ������
	bool Start(); // ����httpserver
	bool Close(); // �ر�
	void AddHandler(const std::string &url, ReqHandler req_handler); // ע���¼�������
	void AddWebSocketCallBack(const std::string _name, WebSocketHandler _WebSocketHandler); // ע���¼�������

	void AnsysWebSocketData(std::string, mg_connection *);
	static void sendWebSocketData(mg_connection *,char *_data,int len);

	void RemoveHandler(const std::string &url); // �Ƴ�ʱ�䴦����
	static std::string s_web_dir; // ��ҳ��Ŀ¼ 
	static mg_serve_http_opts s_server_option; // web������ѡ��
	static std::unordered_map<std::string, ReqHandler> s_handler_map; // �ص�����ӳ���

	static std::unordered_map<std::string, WebSocketHandler> s_WebSocket_map; // �ص�����ӳ���

private:
	// ��̬�¼���Ӧ����
	static void OnHttpEvent(mg_connection *connection, int event_type, void *event_data);
	static void HandleEvent(mg_connection *connection, http_message *http_req);
public:
	static void SendRsp(mg_connection *connection, std::string rsp);
private:
	std::string m_port;    // �˿�
	mg_mgr m_mgr;          // ���ӹ�����
	CC::ThreadMutex m_Mutex;
};
#endif