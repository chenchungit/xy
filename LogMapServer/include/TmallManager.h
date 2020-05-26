#pragma once
#ifndef _TMALLMANAGER_H_
#define _TMALLMANAGER_H_
using namespace std;
class CTmallManager
{
public:
	string PublishTmallCmd(string _ControlType, uint64_t _DeviceID, string _DeviceMac, uint32_t _DeviceType);

	//��ѯ��è����
	bool QueueTmall(uint64_t _DeviceID, string _feature, CTmallObject &_TmallObject);

	//��ȡ��������
	string getFeature(uint32_t _DeviceType);

	
	//��������
	bool  Publish(CTmallObject &_TmallObject, uint32_t _DeviceType, string _ControlType);

private:
	objectPool_CC<CDataObject> m_dzDataObjectList;

};
#endif
