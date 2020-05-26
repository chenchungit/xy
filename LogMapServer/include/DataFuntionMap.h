#ifndef _DATAFUNTIONMAP_H_
#define _DATAFUNTIONMAP_H_

#pragma once
class CDataFuntionMap
{
public:
	CDataFuntionMap(void);
	~CDataFuntionMap(void);

	 /****************
	*注册回调函数
	******************/
	/****************
	*注册回调函数
	******************/
	 virtual void RegisterFuncation();
	/****************
	*服务器注册处理
	******************/
	static void RegisterCallback(TcpSocketID id,TcpSocketID _Token,UChar * _data,const int _len,int _Code);
	/****************
	*服务器系统消息处理
	******************/
	static void Systemcmd(TcpSocketID id,TcpSocketID _Token,UChar * _data,const int _len,int _Code);

	/****************
	*极光推送
	******************/
	static void SubmitJgPush(TcpSocketID id, TcpSocketID _Token, UChar * _data, const int _len, int _Code);
	/****************
	*日志
	******************/
	static void SubmitDeviceLog(TcpSocketID id, TcpSocketID _Token, UChar * _data, const int _len, int _Code);
	/****************
	*告警
	******************/
	static void SubmitDeviceAlarm(TcpSocketID id, TcpSocketID _Token, UChar * _data, const int _len, int _Code);

	/****************
	*获取设备版本
	******************/
	static void getDeviceVersion(ITopic _Topic,UChar * _data, const int _len);

	/****************
	*门锁日志
	******************/
	static void Lock(ITopic _Topic, UChar * _data, const int _len);
	/****************
	*报警器
	******************/
	static void  DeviceAlarm(ITopic _Topic, UChar * _data, const int _len);
	/****************
	*网关设备
	******************/
	static void  DeviceGateWay(ITopic _Topic, UChar * _data, const int _len);
	/****************
	*烟雾报警器
	******************/
	static void  DeviceSmokeAlarm(ITopic _Topic, UChar * _data, const int _len);
	/****************
	*可燃报警器
	******************/
	static void  DeviceGASAlarm(ITopic _Topic, UChar * _data, const int _len);
	/****************
	*门磁
	******************/
	static void  DeviceDoorAlarm(ITopic _Topic, UChar * _data, const int _len);
	static void  VersionTest(ITopic _Topic, UChar * _data, const int _len);

	/****************
	*人数计数器
	******************/
	static void  HumanCnt(ITopic _Topic, UChar * _data, const int _len);

	void QueueDeviceInfo(char *_Name, char* _Psw, char* _Site, int _Pos, int _Pidx, int _Fet, int _Type, int _Idx, ITopic _Topic);
	
private:
		objectPool_CC<CDataObject> m_dzDataObjectList;
protected:
		//服务器内部链接管理
		map<DWORD, CServiceObject*>m_ServiceLink;
		CC::ThreadMutex m_SocketMutex;

		//临时转换数据的内存池
		objectPool_CC<PConvertBuffer> m_dzConvertBuffList;
};

#endif

