#pragma once
#ifndef _SERVERPARAMOBJECT_H_
#define _SERVERPARAMOBJECT_H_

class CServerParamObject
{
public:
	/*************************Parameter
	*获取服务器ID
	*************************/
	inline int32_t getServerID() { return m_ServerID; };
	/*************************Parameter
	*获取服务器类型
	*************************/
	inline int32_t getServerType() { return m_ServerType; };

	/*************************Parameter
	*获取网络数据加密
	*************************/
	inline int32_t getCrypt() { return m_Crypt; };
public:
	//当前服务器ID
	int32_t m_ServerType;
	//当前服务器ID
	int32_t m_ServerID;
	//网络通信是否加密
	int32_t m_Crypt;
};

#endif