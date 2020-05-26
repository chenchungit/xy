#pragma once
#ifndef _JGPUSHRECORDMANAGER_H_
#define _JGPUSHRECORDMANAGER_H_

class CJgPushRecordManager : public CDataBaseObject
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
	virtual bool JgPush(Json::Value &_Root) { return true; };

	void QueueData(const char * _TagAlias, Json::Value &_payLoad,int _Page,int _Size);

	bool SubmitData(const char * _TagAlias, const char * _ContextTitle, const char * _Context);

private:
	//内存池
	objectPool_CC<CJgPushRecordObject> m_dzJgPushRecordPool;
	//数据队列
	hash_map<string, vector<CJgPushRecordObject*>*> m_dzJgPushRecordData;
	map<uint64_t, CJgPushRecordObject*> m_dzMapJgPushRecordData;
};
#endif
