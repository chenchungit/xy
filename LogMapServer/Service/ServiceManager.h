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
	*��ʼ��
	************************/
	virtual void Initialize() {};

	/*********************
	*��ȡ���ݿ�
	************************/
	virtual bool ReadDB() { return 0; };

	/*==============================
	*ͬ����ʼ���ݿ�����
	*id������ID
	*=================================*/
	virtual void SynchronousData(TcpSocketID id) {};

	/*********************
	*�ύ����
	************************/
	virtual void SubmitData(TcpSocketID id, ::google::protobuf::Message & _Protocol) {};
	/*==============
	*ʱ�亯��
	*MajorID������ʱ��ID�ĸ�2�ֽ�����
	*MinorID������ʱ��ID�ĵ�2�ֽ�����
	*=================================*/
	virtual bool OnTimer(unsigned short _MajorID, unsigned short _MinorID, int32_t _Delay) { return 0; };

	/*==============================
	*�������־��Ϣ
	*id��������Ϣ������ID
	*_EMessage��������Ϣ
	*=================================*/
	virtual int32_t  OnXCoreErrorMessage(char* _EMessage) { return 0; };
//========================================================
/****************
	*��ʼ��
	******************/
	 void InitService();
	 void SynchronousInitData(TcpSocketID id,int ServiceType=0x00);

	/****************
	*����Ͽ�������
	******************/
	 void UnSocketClose(TcpSocketID id);

	 /****************
	*���ҿ�������
	******************/
	 CServiceObject* FindFreeConnect(int32_t _ServrID);

	 /****************
	*���ҵ�ǰ���ŵ��߼�����������
	******************/
	DWORD FindOptimalConnect();

	/****************
	*�������ݵ���ص��߼�������
	******************/
	bool SendDataToLogicService(DWORD _Handle,int32_t _iType,UChar* _Data,int32_t _len);
	/****************
	*�������ݵ���ص��߼�������
	******************/
	bool SendDataToLogicService(int32_t _iType,UChar* _Data,int32_t _len);

	/*************************Parameter
	*���ҷ���������
	*************************/
	CServiceObject * FindServerObject(int32_t _ServiceType);

	/*********************
	*���ʹ�Token������
	************************/
	virtual void SynchronousProtocolData(TcpSocketID id,::google::protobuf::Message& _Protocol, int32_t _Cmd, const char * _Context);

	/*************************Parameter
	*��������
	*************************/
	void  UpdateSynchronous();
	/*************************Parameter
	*����������
	*************************/
	void InsertServiceObject(CServiceObject *_ServiceObject);
public:
	//��ʱת�����ݵ��ڴ��
	objectPool_CC<PConvertBuffer> m_dzConvertBuffList;
	
};

#endif