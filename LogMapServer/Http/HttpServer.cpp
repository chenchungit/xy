#include "ServerHead.h"
//std::string HttpServer::s_web_dir; // 网页根目录 
//mg_serve_http_opts HttpServer::s_server_option; // web服务器选项
//std::unordered_map<std::string, ReqHandler> HttpServer::s_handler_map; // 回调函数映射表

void HttpServer::Init(const std::string &port)
{
	m_port = port;
	s_server_option.enable_directory_listing = "yes";
	s_server_option.document_root = s_web_dir.c_str();
	// TODO：其他设置
}

bool HttpServer::Start()
{
	mg_mgr_init(&m_mgr, NULL);
	mg_connection *connection = mg_bind(&m_mgr, m_port.c_str(), OnHttpEvent);
	if (connection == NULL)
		return false;
	mg_set_protocol_http_websocket(connection);

	printf("starting http server at port: %s\n", m_port.c_str());
	// loop
	while (true)
		mg_mgr_poll(&m_mgr, 500); // ms

	return true;
}

void HttpServer::OnHttpEvent(mg_connection *connection, int event_type, void *event_data)
{
	//printf("OnHttpEvent: %d\r\n", event_type);

	switch (event_type)
	{
	case MG_EV_HTTP_REQUEST:
	{
		http_message *http_req = (http_message *)event_data;
		HandleEvent(connection, http_req);
		break;
	}
	case MG_EV_WEBSOCKET_FRAME:
	{
		struct websocket_message *wm = (struct websocket_message*)event_data;
		struct mg_str ms = { (const char*)wm->data, wm->size };
		char szBuff[wm->size + 1];
		RtlZeroMemory(szBuff, wm->size+1);
		RtlCopyMemory(szBuff, wm->data, wm->size);

		std::string szData = szBuff;

		g_ServerManager->g_HttpServerManager.m_HttpServer->AnsysWebSocketData(szData, connection);
		
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "OnHttpEvent [%s]", szBuff);

		//mg_send_websocket_frame(connection, WEBSOCKET_OP_TEXT, wm->data, wm->size);
		//SendRsp(connection, szBuff);
		break;
	}

	default:
		break;
	}
}
void HttpServer::sendWebSocketData(mg_connection *_Con, char *_data, int len)
{
	
	Guard guard(&g_ServerManager->g_HttpServerManager.m_HttpServer->m_Mutex, true);
	//if(_Con->flags > )
	try
	{
		if (_Con)
		{
			printf("_Con err = %d flag = %d \r\n", _Con->err, _Con->flags);
			if (_Con->err == 0 && MG_F_IS_WEBSOCKET == _Con->flags)
				mg_send_websocket_frame(_Con, WEBSOCKET_OP_TEXT, _data, len);
		}
	}
	catch (const std::exception&)
	{
		printf("sendWebSocketData  catch\r\n");
	}
	

}
void HttpServer::AnsysWebSocketData(std::string _Data, mg_connection *_connection)
{
	Json::FastWriter fast_writer;
	Json::Reader  reader;
	Json::Value szTempValue, szHeaderValue, szPlayloadValue;
	bool isParse = reader.parse(_Data, szTempValue, false);
	if (isParse)
	{
		Json::ValueType szType = szTempValue["header"].type();
		//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "========%d", szType);
		szHeaderValue = szTempValue["header"];
		szPlayloadValue = szTempValue["payload"];
		
		Json::ValueType type = szPlayloadValue.type();
		string szPlayloadStr = fast_writer.write(szPlayloadValue);

		string szName = szHeaderValue["name"].asString();

		auto it = s_WebSocket_map.find(szName);
		if (it != s_WebSocket_map.end())
		{
			WebSocketHandler WebSocketHandler_func = it->second;
			WebSocketHandler_func(szPlayloadStr, _connection);
		}
		else
		{
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "Ansys WebSocket Not find Name error");
		}

	}
	else
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "Ansys WebSocket Data error");
	}

}
static bool route_check(http_message *http_msg, char *route_prefix)
{
	if (mg_vcmp(&http_msg->uri, route_prefix) == 0)
		return true;
	else
		return false;

	// TODO: 还可以判断 GET, POST, PUT, DELTE等方法
	//mg_vcmp(&http_msg->method, "GET");
	//mg_vcmp(&http_msg->method, "POST");
	//mg_vcmp(&http_msg->method, "PUT");
	//mg_vcmp(&http_msg->method, "DELETE");
}
void HttpServer::AddWebSocketCallBack(const std::string _name, WebSocketHandler _WebSocketHandler)
{
	if (s_WebSocket_map.find(_name) != s_WebSocket_map.end())
		return;

	s_WebSocket_map.insert(std::make_pair(_name, _WebSocketHandler));
}
void HttpServer::AddHandler(const std::string &url, ReqHandler req_handler)
{
	if (s_handler_map.find(url) != s_handler_map.end())
		return;

	s_handler_map.insert(std::make_pair(url, req_handler));
}

void HttpServer::RemoveHandler(const std::string &url)
{
	auto it = s_handler_map.find(url);
	if (it != s_handler_map.end())
		s_handler_map.erase(it);
}

void HttpServer::SendRsp(mg_connection *connection, std::string rsp)
{
	// 必须先发送header
	mg_printf(connection, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\nContent-Type: application/json\r\n\r\n");
	// 以json形式返回
	mg_printf_http_chunk(connection, "{ \"result\": %s }", rsp.c_str());
	// 发送空白字符快，结束当前响应
	mg_send_http_chunk(connection, "", 0);
}

void HttpServer::HandleEvent(mg_connection *connection, http_message *http_req)
{
	std::string req_str = std::string(http_req->message.p, http_req->message.len);
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "got request: %s", req_str.c_str());
	//printf("got request: %s\r\n", req_str.c_str());
	std::string szParam = std::string(http_req->query_string.p, http_req->query_string.len);
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "got request Param: [%s]", szParam.c_str());
	//printf("got request Param: [%s]\r\n", szParam.c_str());
	// 先过滤是否已注册的函数回调
	std::string url = std::string(http_req->uri.p, http_req->uri.len);
	std::string body = std::string(http_req->body.p, http_req->body.len);
	auto it = s_handler_map.find(url);
	if (it != s_handler_map.end())
	{
		ReqHandler handle_func = it->second;
		handle_func(url, body, szParam, connection, SendRsp);
	}

	// 其他请求
	if (route_check(http_req, "/")) // index page
		mg_serve_http(connection, http_req, s_server_option);
	else if (route_check(http_req, "/api/hello"))
	{
		// 直接回传
		SendRsp(connection, "welcome to httpserver");
	}
	else if (route_check(http_req, "/api/sum"))
	{
		// 简单post请求，加法运算测试
		char n1[100], n2[100];
		double result;

		/* Get form variables */
		mg_get_http_var(&http_req->body, "n1", n1, sizeof(n1));
		mg_get_http_var(&http_req->body, "n2", n2, sizeof(n2));

		/* Compute the result and send it back as a JSON object */
		result = strtod(n1, NULL) + strtod(n2, NULL);
		SendRsp(connection, std::to_string(result));
	}
	else
	{
		mg_printf(
			connection,
			"%s",
			"HTTP/1.1 501 Not Implemented\r\n"
			"Content-Length: 0\r\n\r\n");
	}
}

bool HttpServer::Close()
{
	mg_mgr_free(&m_mgr);
	return true;
}