#pragma once
#ifndef _DEVICEUUIDMANAGER_H_
#define _DEVICEUUIDMANAGER_H_

class CDeviceUuidManager : public CDataBaseObject
{
public:
	/*********************
	*初始化
	************************/
	virtual void Initialize();

	/*********************
	*读取数据库
	************************/
	virtual bool ReadDB();
	virtual bool ReadSingleDB(char *_Mac, char *_Uuid);
	//=======================================
	//查找MAC
	bool FindMacUuid(const char *_Mac, char *_Uuid);

	//=================================================
	//解密
	bool UnCryptProtocol(const char * _Mac,UChar *_data, uint32_t _Length);

private:
	//内存池
	objectPool_CC<CDeviceUuidObject> m_dzDeviceUuidPool;
	//数据队列
	map<string, CDeviceUuidObject*> m_dzDeviceUuidData;
};
#endif