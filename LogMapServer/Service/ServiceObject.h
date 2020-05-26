#ifndef _SERVICEOBJECT_H_
#define _SERVICEOBJECT_H_
#pragma once

class CServiceObject
{
public:
	CServiceObject(int32_t _iType,int32_t _ServerID,TcpSocketID _TcpSocketID,int32_t _State/*,const char *_ip,int32_t _Port*/);
	~CServiceObject();

	 /****************
	*更新链接参数
	******************/
	void UpdateParameter(int32_t _iType,int32_t _ServerID,TcpSocketID _TcpSocketID,int32_t _State/*,const char *_ip,int32_t _Port*/);

public:
	//服务器类型
	int32_t m_iType;
	//服务器 ID
	int32_t m_ServerID;
	//当前状态
	int32_t m_State;
	//网络ID
	TcpSocketID m_TcpSocketID;
	//当前的连接人数
	int32_t m_CurrentUser;
	//最大人数
	int32_t m_MaxUser;
	//IP地址
	//char m_ip[32];
	//端口
	//int32_t m_port;
};


#endif