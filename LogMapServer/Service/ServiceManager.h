#ifndef _SERVICEMANAGER_H_
#define _SERVICEMANAGER_H_
#pragma once

class CServiceManager :public CDataFuntionMap, public CDataBaseObject
{
public:
	CServiceManager();
	~CServiceManager();
	//====================================================================================================
	/*********************
	*初始化
	************************/
	virtual void Initialize() {};

	/*********************
	*读取数据库
	************************/
	virtual bool ReadDB() { return 0; };

	/*==============================
	*同步初始数据库数据
	*id：网络ID
	*=================================*/
	virtual void SynchronousData(TcpSocketID id) {};

	/*********************
	*提交数据
	************************/
	virtual void SubmitData(TcpSocketID id, ::google::protobuf::Message & _Protocol) {};
	/*==============
	*时间函数
	*MajorID：生成时间ID的高2字节数据
	*MinorID：生成时间ID的低2字节数据
	*=================================*/
	virtual bool OnTimer(unsigned short _MajorID, unsigned short _MinorID, int32_t _Delay) { return 0; };

	/*==============================
	*输出的日志消息
	*id：错误消息的网络ID
	*_EMessage：错误消息
	*=================================*/
	virtual int32_t  OnXCoreErrorMessage(char* _EMessage) { return 0; };
//========================================================
/****************
	*初始化
	******************/
	 void InitService();
	 void SynchronousInitData(TcpSocketID id,int ServiceType=0x00);

	/****************
	*处理断开的连接
	******************/
	 void UnSocketClose(TcpSocketID id);

	 /****************
	*查找空闲链接
	******************/
	 CServiceObject* FindFreeConnect(int32_t _ServrID);

	 /****************
	*查找当前最优的逻辑服务器分配
	******************/
	DWORD FindOptimalConnect();

	/****************
	*发送数据到相关的逻辑服务器
	******************/
	bool SendDataToLogicService(DWORD _Handle,int32_t _iType,UChar* _Data,int32_t _len);
	/****************
	*发送数据到相关的逻辑服务器
	******************/
	bool SendDataToLogicService(int32_t _iType,UChar* _Data,int32_t _len);

	/*************************Parameter
	*查找服务器对象
	*************************/
	CServiceObject * FindServerObject(int32_t _ServiceType);

	/*********************
	*发送带Token的数据
	************************/
	virtual void SynchronousProtocolData(TcpSocketID id,::google::protobuf::Message& _Protocol, int32_t _Cmd, const char * _Context);

	/*************************Parameter
	*更新数据
	*************************/
	void  UpdateSynchronous();
	/*************************Parameter
	*插入服务对象
	*************************/
	void InsertServiceObject(CServiceObject *_ServiceObject);
public:
	//临时转换数据的内存池
	objectPool_CC<PConvertBuffer> m_dzConvertBuffList;
	
};

#endif