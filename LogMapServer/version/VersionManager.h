#pragma once
#ifndef _VERSIONMANAGER_H_
#define _VERSIONMANAGER_H_

class CVersionManager :public CDataBaseObject
{
public:
	CVersionManager() {};
	~CVersionManager() {};
	/*********************
	*初始化
	************************/
	virtual void Initialize();

	/*********************
	*读取数据库
	************************/
	virtual bool ReadDB() {
		return 0;};
	//加载文件
	void LoadVerFile(string _FileName, UINT _FileCode, UINT _SumSize);
	void LoadVerFile(UChar* _FileData, UINT _FileCode, UINT _SumSize);
	//获取版本文件数据
	void DownVerFileData(ITopic _Topic,UINT _FileCode, UINT _Offet, UChar _SumSize);
	void DownVerFileData(ITopic _Topic,UINT _FileCode);
	//
	void ScanSingledir(const char * _DirName);

	//存储版本
	bool SaveDeviceVer(const char* _Mac, uint16_t _Ver);
	//查询数据库版本信息
	bool ReadSingleDB(char *_BundleID, uint32_t &_FileCode, uint16_t &_VerValue, uint32_t &_FileSize);

	//查询比对版本
	bool CheckDeviceVersion(ITopic _Topic, char* _BundleID, uint16_t _Ver);

	//提交升级进度
	bool LimitedFileInfo(UINT _FileCode, UINT _Offet, UINT _SumSize, char* _Mac);
	bool LimitedOver( UINT _Ver, char* _Mac);
private:
	map<UINT, CVersionObject*>m_VersionList;
private:
	objectPool_CC<CDataObject> m_dzDataObjectList;
	//临时转换数据的内存池
	objectPool_CC<PConvertBuffer> m_dzConvertBuffList;
};

#endif