#ifndef _DATAFUNTIONMAP_H_
#define _DATAFUNTIONMAP_H_

#pragma once
class CDataFuntionMap
{
public:
	CDataFuntionMap(void);
	~CDataFuntionMap(void);

	 /****************
	*ע��ص�����
	******************/
	/****************
	*ע��ص�����
	******************/
	 virtual void RegisterFuncation();
	/****************
	*������ע�ᴦ��
	******************/
	static void RegisterCallback(TcpSocketID id,TcpSocketID _Token,UChar * _data,const int _len,int _Code);
	/****************
	*������ϵͳ��Ϣ����
	******************/
	static void Systemcmd(TcpSocketID id,TcpSocketID _Token,UChar * _data,const int _len,int _Code);

	/****************
	*��������
	******************/
	static void SubmitJgPush(TcpSocketID id, TcpSocketID _Token, UChar * _data, const int _len, int _Code);
	/****************
	*��־
	******************/
	static void SubmitDeviceLog(TcpSocketID id, TcpSocketID _Token, UChar * _data, const int _len, int _Code);
	/****************
	*�澯
	******************/
	static void SubmitDeviceAlarm(TcpSocketID id, TcpSocketID _Token, UChar * _data, const int _len, int _Code);

	/****************
	*��ȡ�豸�汾
	******************/
	static void getDeviceVersion(ITopic _Topic,UChar * _data, const int _len);

	/****************
	*������־
	******************/
	static void Lock(ITopic _Topic, UChar * _data, const int _len);
	/****************
	*������
	******************/
	static void  DeviceAlarm(ITopic _Topic, UChar * _data, const int _len);
	/****************
	*�����豸
	******************/
	static void  DeviceGateWay(ITopic _Topic, UChar * _data, const int _len);
	/****************
	*��������
	******************/
	static void  DeviceSmokeAlarm(ITopic _Topic, UChar * _data, const int _len);
	/****************
	*��ȼ������
	******************/
	static void  DeviceGASAlarm(ITopic _Topic, UChar * _data, const int _len);
	/****************
	*�Ŵ�
	******************/
	static void  DeviceDoorAlarm(ITopic _Topic, UChar * _data, const int _len);
	static void  VersionTest(ITopic _Topic, UChar * _data, const int _len);

	/****************
	*����������
	******************/
	static void  HumanCnt(ITopic _Topic, UChar * _data, const int _len);

	void QueueDeviceInfo(char *_Name, char* _Psw, char* _Site, int _Pos, int _Pidx, int _Fet, int _Type, int _Idx, ITopic _Topic);
	
private:
		objectPool_CC<CDataObject> m_dzDataObjectList;
protected:
		//�������ڲ����ӹ���
		map<DWORD, CServiceObject*>m_ServiceLink;
		CC::ThreadMutex m_SocketMutex;

		//��ʱת�����ݵ��ڴ��
		objectPool_CC<PConvertBuffer> m_dzConvertBuffList;
};

#endif

