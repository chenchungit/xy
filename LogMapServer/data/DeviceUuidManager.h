#pragma once
#ifndef _DEVICEUUIDMANAGER_H_
#define _DEVICEUUIDMANAGER_H_

class CDeviceUuidManager : public CDataBaseObject
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
	virtual bool ReadSingleDB(char *_Mac, char *_Uuid);
	//=======================================
	//����MAC
	bool FindMacUuid(const char *_Mac, char *_Uuid);

	//=================================================
	//����
	bool UnCryptProtocol(const char * _Mac,UChar *_data, uint32_t _Length);

private:
	//�ڴ��
	objectPool_CC<CDeviceUuidObject> m_dzDeviceUuidPool;
	//���ݶ���
	map<string, CDeviceUuidObject*> m_dzDeviceUuidData;
};
#endif