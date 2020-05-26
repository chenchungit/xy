#pragma once
#ifndef _VERSIONMANAGER_H_
#define _VERSIONMANAGER_H_

class CVersionManager :public CDataBaseObject
{
public:
	CVersionManager() {};
	~CVersionManager() {};
	/*********************
	*��ʼ��
	************************/
	virtual void Initialize();

	/*********************
	*��ȡ���ݿ�
	************************/
	virtual bool ReadDB() {
		return 0;};
	//�����ļ�
	void LoadVerFile(string _FileName, UINT _FileCode, UINT _SumSize);
	void LoadVerFile(UChar* _FileData, UINT _FileCode, UINT _SumSize);
	//��ȡ�汾�ļ�����
	void DownVerFileData(ITopic _Topic,UINT _FileCode, UINT _Offet, UChar _SumSize);
	void DownVerFileData(ITopic _Topic,UINT _FileCode);
	//
	void ScanSingledir(const char * _DirName);

	//�洢�汾
	bool SaveDeviceVer(const char* _Mac, uint16_t _Ver);
	//��ѯ���ݿ�汾��Ϣ
	bool ReadSingleDB(char *_BundleID, uint32_t &_FileCode, uint16_t &_VerValue, uint32_t &_FileSize);

	//��ѯ�ȶ԰汾
	bool CheckDeviceVersion(ITopic _Topic, char* _BundleID, uint16_t _Ver);

	//�ύ��������
	bool LimitedFileInfo(UINT _FileCode, UINT _Offet, UINT _SumSize, char* _Mac);
	bool LimitedOver( UINT _Ver, char* _Mac);
private:
	map<UINT, CVersionObject*>m_VersionList;
private:
	objectPool_CC<CDataObject> m_dzDataObjectList;
	//��ʱת�����ݵ��ڴ��
	objectPool_CC<PConvertBuffer> m_dzConvertBuffList;
};

#endif