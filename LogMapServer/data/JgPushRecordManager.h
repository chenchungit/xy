#pragma once
#ifndef _JGPUSHRECORDMANAGER_H_
#define _JGPUSHRECORDMANAGER_H_

class CJgPushRecordManager : public CDataBaseObject
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
	virtual bool JgPush(Json::Value &_Root) { return true; };

	void QueueData(const char * _TagAlias, Json::Value &_payLoad,int _Page,int _Size);

	bool SubmitData(const char * _TagAlias, const char * _ContextTitle, const char * _Context);

private:
	//�ڴ��
	objectPool_CC<CJgPushRecordObject> m_dzJgPushRecordPool;
	//���ݶ���
	hash_map<string, vector<CJgPushRecordObject*>*> m_dzJgPushRecordData;
	map<uint64_t, CJgPushRecordObject*> m_dzMapJgPushRecordData;
};
#endif
