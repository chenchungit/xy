#pragma once
#ifndef _HTTPSERVER_H_
#define _HTTPSERVER_H_

// 定义http返回callback
typedef void OnRspCallback(mg_connection *c, std::string);
// 定义http请求handler
using ReqHandler = std::function<bool(std::string, std::string, std::string, mg_connection *c, OnRspCallback)>;

// 定义WebSocket请求handler
using WebSocketHandler = std::function<bool(std::string, mg_connection *c)>;

class HttpServer
{
public:
	HttpServer() {}
	~HttpServer() {}
	void Init(const std::string &port); // 初始化设置
	bool Start(); // 启动httpserver
	bool Close(); // 关闭
	void AddHandler(const std::string &url, ReqHandler req_handler); // 注册事件处理函数
	void AddWebSocketCallBack(const std::string _name, WebSocketHandler _WebSocketHandler); // 注册事件处理函数

	void AnsysWebSocketData(std::string, mg_connection *);
	static void sendWebSocketData(mg_connection *,char *_data,int len);

	void RemoveHandler(const std::string &url); // 移除时间处理函数
	static std::string s_web_dir; // 网页根目录 
	static mg_serve_http_opts s_server_option; // web服务器选项
	static std::unordered_map<std::string, ReqHandler> s_handler_map; // 回调函数映射表

	static std::unordered_map<std::string, WebSocketHandler> s_WebSocket_map; // 回调函数映射表

private:
	// 静态事件响应函数
	static void OnHttpEvent(mg_connection *connection, int event_type, void *event_data);
	static void HandleEvent(mg_connection *connection, http_message *http_req);
public:
	static void SendRsp(mg_connection *connection, std::string rsp);
private:
	std::string m_port;    // 端口
	mg_mgr m_mgr;          // 连接管理器
	CC::ThreadMutex m_Mutex;
};
#endif