#ifndef _USERECORDMANAGER_H_
#define _USERECORDMANAGER_H_
#pragma once
class CUseRecordManager : public CDataBaseObject
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

	virtual bool JgPush(Json::Value &_Root);

	void jgGroupPackage(string _Mac, int _LogType,int _CarType, Json::Value &_Root);
	/*********************
	*提交数据
	************************/
	virtual void SubmitData(string _Mac, string _User, UINT _UseType, UINT _UseTime, UINT _UpTime, string _Remark
	, UINT _LogType, UINT _CarType, string _GwMac, UINT _AreaDeviceID);
	//存储数据库
	bool saveDB(CUseRecordObject * _UseRecordObject);
	//查询
	void QueueData(Json::Value &_payLoad, int _Page, int _Size,
		string _Mac = "",string _UserID = "",uint64_t _BeginTime = 0L,uint64_t _EndTime = 0L);

	void getAllUseRecord(Json::Value &_payLoad,int _Page, int _Size);
	void getMacUseRecord(Json::Value &_payLoad, int _Page, int _Size, string _Mac);
	void getCardsUseRecord(Json::Value &_payLoad, int _Page, int _Size, string _UserID);
	void getTimeUseRecord(Json::Value &_payLoad, int _Page, int _Size, uint64_t _BeginTime, uint64_t _EndTime);
	void getMacTimeUseRecord(Json::Value &_payLoad, int _Page, int _Size, string _Mac, uint64_t _BeginTime, uint64_t _EndTime);
private:
	//内存池
	objectPool_CC<CUseRecordObject> m_dzUseRecordPool;
	//数据队列
	hash_map<string, vector<CUseRecordObject*>*> m_dzUseRecordData;
	hash_map<string, vector<CUseRecordObject*>*> m_dzCardsUseRecordData;
	//临时转换数据的内存池
	objectPool_CC<PConvertBuffer> m_dzConvertBuffList;

	//内核
	//ITimerService *g_TimerService;

	static int32_t m_MaxID;
};
#endif
