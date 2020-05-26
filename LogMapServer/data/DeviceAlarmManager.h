#ifndef _DEVICEALARMMANAGER_H_
#define _DEVICEALARMMANAGER_H_
#pragma once
class CDeviceAlarmManager : public CDataBaseObject
{
public:
	/*********************
	*��ʼ��
	************************/
	virtual void Initialize();

	/*********************
	*��ȡ���ݿ�
	************************/
	virtual bool ReadDB();

	virtual bool JgPush(Json::Value &_Root);
	void jgGroupPackage(string _Mac, UChar* _DevState, Json::Value &_Root);
	/*********************
	*�ύ����
	************************/
	virtual void SubmitData(string _Mac, UINT _CurTime, UINT _HeppenTime,UINT _State,UINT _DevSort, UChar* _DevState, UINT _DevStateLen);
	bool saveDB(CDeviceAlarmObject * _DeviceAlarmObject);
	void QueueData(string _Mac, Json::Value &_payLoad, int _Page, int _Size);


	static bool DeviceAlarmComp( CDeviceAlarmObject *_Src,  CDeviceAlarmObject *_Drc);

private:
	//�ڴ��
	objectPool_CC<CDeviceAlarmObject> m_dzDeviceAlarmPool;
	//���ݶ���
	hash_map<string, vector<CDeviceAlarmObject*>*> m_dzDeviceAlarmData;
	

};
#endif