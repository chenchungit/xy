#include "ServerHead.h"

void CVersionManager::Initialize()
{
	ScanSingledir(g_ServerManager->g_HttpClientManager.getFilePath().c_str());
}
void CVersionManager::ScanSingledir(const char * _DirName)
{
	if (NULL == _DirName)
	{
		cout << " dir_name is null ! " << endl;
		return;
	}
	struct stat szstat;
	lstat(_DirName, &szstat);
	if (!S_ISDIR(szstat.st_mode))
	{
		return;
	}
	struct dirent * szFileName;
	DIR * szDir;
	szDir = opendir(_DirName);
	if (NULL == szDir)
	{
		return;
	}
	//int iName = 0;
	while ((szFileName = readdir(szDir)) != NULL)
	{
		if (strcmp(szFileName->d_name, ".") == 0 ||
			strcmp(szFileName->d_name, "..") == 0)
			continue;
		string szName(szFileName->d_name);
		string::size_type szPosition;
		szPosition = szName.find("_");
		if (szPosition != szName.npos)
		{
			LoadVerFile(szFileName->d_name+ szPosition + 1,atoi(szName.substr(0, szPosition).c_str()),0);
		}
		
	}
	closedir(szDir);
}
void CVersionManager::DownVerFileData(ITopic _Topic,UINT _FileCode)
{


	map<UINT, CVersionObject*>::iterator col = m_VersionList.find(_FileCode);
	if (col != m_VersionList.end())
	{
		for (int i = 0;i< col->second->getSumSize();i += 240)
		{
			DownVerFileData(_Topic, _FileCode, i, 240);
		}
	}
	
}
//void CVersionManager::DownVerFileData(ITopic _Topic, UINT _FileCode, UINT _Offet, UChar _SumSize)


void CVersionManager::DownVerFileData(ITopic _Topic, UINT _FileCode, UINT _Offet, UChar _SumSize)
{
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "Down Ver File Data _FileCode = %d _Offet = %d _SumSize = %d", _FileCode, _Offet, _SumSize);

	stringstream szStreamTopic;
	UChar szDefine = 0x00;
	UChar szDefineLen = _SumSize + 11;
	UChar szDefineCMD = CMD_VERSION_DOWN_FILE;
	UChar szData[_SumSize] = { 0x00 };
	UChar szDataLen = 0;
	//UChar szPayload[36] = { 0 };
	CDataObject *dzPayload = m_dzDataObjectList._malloc_boost();

	//长度
	*dzPayload << szDefineLen;
	*dzPayload << szDefineCMD;
	
	

	map<UINT, CVersionObject*>::iterator col = m_VersionList.find(_FileCode);
	if (col != m_VersionList.end())
	{
		int szPackage = 0;
		

		vector<UChar> &szVerFileArray = col->second->getFileVersion();
		int szIndex = 0;
		if (_Offet < szVerFileArray.size())
		{
			for (auto row : szVerFileArray)
			{

				if (szIndex >= _Offet)
				{
					szData[szIndex - _Offet] = row;
				}
				szIndex++;
				if ((szIndex > _Offet) && (szIndex - _Offet) == _SumSize)
					break;
			}
			if (szIndex > _Offet)
			{
				*dzPayload << FUCK_VERSION_STATE_SUCCESS;
				szDataLen = szIndex - _Offet;
				float szPencent = ((_Offet+ szDataLen) / (float)col->second->getSumSize());
				/*g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, ">>>>>>>[%d][%d]",
					_Offet / (UINT)_SumSize, (_Offet / (UINT)_SumSize % 10));*/
				//RtlCopyMemory(szPayloadTemp + 11, szData, szIndex - _Offet);
				//szTempData << (szIndex - _Offet + 10) << 0x47 << FUCK_VERSION_STATE_SUCCESS << htonl(_FileCode) << htonl(_Offet) << szData;
				if (((_Offet / (UINT)_SumSize) % 10 == 0) ||
					szDataLen < _SumSize)
				{
					g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, ">>>>>>>>>>>>>down load file percent[%0.2f][%u]",
						szPencent, col->second->getSumSize());
					if (LimitedFileInfo(_FileCode, (_Offet + szDataLen), col->second->getSumSize(), (char*)_Topic.m_Mac.c_str()))
					{
						g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "down load file percent[%d]", szPencent * 100);
					}
				}
			}
			else
				*dzPayload << FUCK_VERSION_STATE_FAIL;
				//szTempData << 0x0A << 0x47 << FUCK_VERSION_STATE_FAIL << htonl(_FileCode) << htonl(_Offet);
		}
		else
			*dzPayload << FUCK_VERSION_STATE_FAIL;
			//szTempData << 0x0A << 0x47 << FUCK_VERSION_STATE_FAIL << htonl(_FileCode) << htonl(_Offet);

	}
	else
	{
		*dzPayload << FUCK_VERSION_STATE_FAIL;
		//szTempData << 0x0A << 0x47 << FUCK_VERSION_STATE_FAIL << htonl(_FileCode) << htonl(_Offet);
	}
	uint32_t szU32Temp = 0;
	//文件号
	szU32Temp = htonl(_FileCode);
	*dzPayload << szU32Temp;
	//偏移量
	szU32Temp = htonl(_Offet);
	*dzPayload << szU32Temp;
	//数据
	dzPayload->write((char*)szData, szDataLen);
	
	
	
	//加校验
	unsigned short szCrc = htons(g_ServerManager->g_ServiceManager.GetCrc16Ccitt(CRC_BASE_KEY, (unsigned char*)dzPayload->m_Data, dzPayload->m_DataLen));
	*dzPayload << szCrc;
	if (g_ServerManager->getCrypt())
	//加密
	g_ServerManager->g_DeviceUuidManager.UnCryptProtocol(_Topic.m_Mac.c_str(), (UChar*)dzPayload->m_Data, dzPayload->m_DataLen);

	unsigned char *szTempData = (unsigned char *)m_dzConvertBuffList._malloc_boost();
	RtlZeroMemory(szTempData, MAX_BUFF_NET_SIZE);
	size_t szTempDataLen = MAX_BUFF_NET_SIZE;
	
#ifdef VIANS
		szStreamTopic << "Vians/v3/Cnf/SmartHome/"
			<< _Topic.m_Mac << "/" << _Topic.m_Site
			<< "/" << _Topic.m_Position << "/"
			<< _Topic.m_PosIdx << "/" << _Topic.m_Feature
			<< "/" << _Topic.m_Type << "/" << _Topic.m_Index;


		//RtlCopyMemory(szTempData, dzPayload->m_Data, dzPayload->m_DataLen);
		//szTempDataLen = dzPayload->m_DataLen;
		if (base64_encode(szTempData, &szTempDataLen, (UChar*)dzPayload->m_Data, dzPayload->m_DataLen) != 0)
		{
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "DownVerFileData base64_decode error");
			m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);
			m_dzDataObjectList._free_boost(dzPayload);
			return ;
		}
#else
		szStreamTopic << "Nexless/v2/Cnf/SmartHome/"
			<< _Topic.m_Mac << "/" << _Topic.m_Site
			<< "/" << _Topic.m_Position << "/"
			<< _Topic.m_PosIdx << "/" << _Topic.m_Feature
			<< "/" << _Topic.m_Type << "/" << _Topic.m_Index;   //Mac/Site/Position/PosIdx/GATEWAY/Type/Index";


		RtlCopyMemory(szTempData, dzPayload->m_Data, dzPayload->m_DataLen);
		szTempDataLen = dzPayload->m_DataLen;
#endif
		
	if (!g_ServerManager->g_MqttManager.Publish(szStreamTopic.str(), (char*)szTempData, szTempDataLen))
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "MQTT Submission [%s] failure", "version updata fail");
	else
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "MQTT Submission version updata Success[%s]", szStreamTopic.str().c_str());
	m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);
	m_dzDataObjectList._free_boost(dzPayload);
	//sleep(2);
	

	
}

void CVersionManager::LoadVerFile(string _FileName, UINT _FileCode, UINT _SumSize)
{
	stringstream szSavePath;
	szSavePath<< g_ServerManager->g_HttpClientManager.getFilePath() << _FileCode << "_" << _FileName;
	ifstream szVerFile(szSavePath.str().c_str(), ios_base::binary | ios_base::in);
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "create File name = %s", szSavePath.str().c_str());
	if (!szVerFile.is_open())
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "open src File  Error opening file");
		
		return;
	}
	//获取文件大小
	//szVerFile.seekg(0, ios_base::end);
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "create File name = %s Success", szSavePath.str().c_str());
	CVersionObject *szVersionObject = new CVersionObject();
	szVersionObject->setIndex(_FileCode);
	
	//szVersionObject->setDescribeFlag(_DescribeFlag);
	vector<UChar> &szVerFileArray = szVersionObject->getFileVersion();
	while (!szVerFile.eof())
	{
		UChar szData = 0x00;
		szVerFile.read((char*)&szData, 1);
		int szReadCount = szVerFile.gcount();
		if (szReadCount > 0)
			szVerFileArray.push_back(szData);

	}
	cout << endl;
	szVerFile.close();
	szVersionObject->setSumSize(szVerFileArray.size());
	m_VersionList[_FileCode] = szVersionObject;
	cout << "Ver File Size = " << szVerFileArray.size() << "  Success" << endl;
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "Ver File Size = %d  Success", szVerFileArray.size());
	//DownVerFileData(_DescribeFlag);
}


void CVersionManager::LoadVerFile(UChar* _FileData, UINT _FileCode, UINT _SumSize)
{
	CVersionObject *szVersionObject = new CVersionObject();
	szVersionObject->setIndex(_FileCode);
	szVersionObject->setSumSize(_SumSize);
	//szVersionObject->setDescribeFlag(_DescribeFlag);
	vector<UChar> &szVerFileArray = szVersionObject->getFileVersion();
	for (int i =0;i< _SumSize;i++)
	{
		szVerFileArray.push_back(_FileData[i]);
	}
	m_VersionList[_FileCode] = szVersionObject;
	//DownVerFileData(_DescribeFlag);
}

bool CVersionManager::SaveDeviceVer(const char* _Mac, uint16_t _Ver)
{
	std::stringstream szCharacters;
	szCharacters << "UPDATE xy_devicebaseinfo set db_fwVer = ";
	szCharacters << _Ver << " where db_Mac = '";
	szCharacters << _Mac << "';";
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "Save Device Ver [%s]", szCharacters.str().c_str());
	MYSQL_RES *dzRes = g_ServerManager->g_IMysqlInterFace->ExcelSql(szCharacters.str());
	MYSQL_ROW dzSqlrow;
	if (dzRes)
	{
		while ((dzSqlrow = mysql_fetch_row(dzRes)))
		{
			g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_ERROR, "write new Version Success[%s]", SysTemContext[atoi(dzSqlrow[0])]);
		}
		mysql_free_result(dzRes);
		return true;
	}
	return false;
}

bool CVersionManager::ReadSingleDB(char *_BundleID, uint32_t &_FileCode, uint16_t &_VerValue, uint32_t &_FileSize)
{
	MYSQL_RES *dzRes;
	MYSQL_ROW dzSqlrow;
	char *szRetConvert;
	char *dzSql = (char*)m_dzConvertBuffList._malloc_boost();
	snprintf(dzSql, MAX_BUFF_LOG_SIZE,
		"select a1.release_version_id ,a1.release_version_code,a2.size from xy_app as a1,xy_app_version as a2 where a1.release_version_id = a2.id  and  a1.bundle_id =  '%s';", _BundleID);

	dzRes = g_ServerManager->g_IMysqlInterFace->QuerySql(dzSql);
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, dzSql);
	m_dzConvertBuffList._free_boost((LPConvertBuffer)dzSql);
	if (!dzRes)
	{
		mysql_free_result(dzRes);
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "Database stored procedure access error");
		return false;
	}
	else
	{
		while ((dzSqlrow = mysql_fetch_row(dzRes)))
		{
			if (mysql_num_fields(dzRes) < 3)
			{
				continue;
			}
			_FileCode = atoi(dzSqlrow[0]);
			string szStrVer = dzSqlrow[1];
			_VerValue = getDeviceVersion(szStrVer);
			_FileSize = atoi(dzSqlrow[2]);
			
		}
		mysql_free_result(dzRes);

	}
	return true;
}



//bool CVersionManager::CheckDeviceVersion(ITopic _Topic, char* _BundleID, uint16_t _Ver)
//{
//	UChar szCRC[4] = {0};
//	UChar szDefine = 0x00;
//	UChar szPayload[36] = { 0 };
//	stringstream szStreamTopic;
//	szPayload[0] = 0x23;
//	szPayload[1] = CMD_DEVICE_VERSION_DATA;
//	//szSendPackage << 0x24 << CMD_DEVICE_VERSION_DATA;
//	uint16_t _VerValue;
//	uint32_t _FileCode, _FileSize;
//	if (ReadSingleDB(_BundleID, _FileCode, _VerValue, _FileSize))
//	{
//		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "CheckDeviceVersion[%d][%d][%d][%d]", _Ver, _VerValue, _FileCode, _FileSize);
//		if (_Ver < _VerValue)
//		{
//			szPayload[2] = FUCK_VERSION_STATE_SUCCESS;
//			
//			map<UINT, CVersionObject*>::iterator col = m_VersionList.find(_FileCode);
//			if (col != m_VersionList.end())
//			{
//				vector<UChar> &szVerFileArray = col->second->getFileVersion();
//				szCRC[0] = szVerFileArray[76];
//				szCRC[1] = szVerFileArray[77];
//				szCRC[2] = szVerFileArray[78];
//				szCRC[3] = szVerFileArray[79];
//			}
//		}
//		else
//			if(_Ver == _VerValue)
//				szPayload[2] = FUCK_VERSION_STATE_FAIL;
//	}
//	else
//		szPayload[2] = 0x02;
//	int szBandleLen = strlen(_BundleID);
//	RtlCopyMemory(szPayload + 3, _BundleID, szBandleLen);
//	for (; szBandleLen < 16; szBandleLen++)
//	{
//		RtlCopyMemory(szPayload + 3 + szBandleLen, &szDefine, 1);
//	}
//	//24 45 00 4E4C5F47574630314300000000000000 00 1802 00035316 0040 A723B3B1 00000052
//	uint16_t szU16Temp = 0;
//	uint32_t szU32Temp = 0;
//	//序号
//	szPayload[19] = szDefine;
//	//版本号
//	szU16Temp = htons(_VerValue);
//	RtlCopyMemory(szPayload + 20, &szU16Temp, sizeof(uint16_t));
//	//文件大小
//	szU32Temp = htonl(_FileSize);
//	RtlCopyMemory(szPayload + 22, &szU32Temp, sizeof(uint32_t));
//	//单包大小64
//	szPayload[26] = 0x40;
//	//单包大小
//	RtlCopyMemory(szPayload + 27, szCRC, sizeof(szCRC));
//	//文件号
//	szU32Temp = htonl(_FileCode);
//	RtlCopyMemory(szPayload + 31, &szU32Temp, sizeof(uint32_t));
//	//序列号+版本号+文件大小+单包大小2字节+cRC校验+文件号
//	//szSendPackage << 0x00<< htonl(_VerValue) << htonl(_FileSize) << htonl(64) << szCRC << htonl(_FileCode);
//
//	UChar  szSendData[38];
//	/*szSendPackage >> szSendDataTemp;
//	szSendPackage >> szSendData;*/
//
//
//	//加校验
//	unsigned short szCrc = htons(g_ServerManager->g_ServiceManager.GetCrc16Ccitt(CRC_BASE_KEY, (unsigned char*)szPayload, sizeof(szPayload)));
//	RtlCopyMemory(szSendData, szPayload, sizeof(szPayload));
//	RtlCopyMemory(szSendData + sizeof(szPayload), &szCrc, sizeof(uint16_t));
//
//	//加密
//	//g_ServerManager->g_DeviceUuidManager.UnCryptProtocol(_Topic.m_Mac.c_str(), szSendData, sizeof(szSendData));
//
//	szStreamTopic << "Nexless/v2/Cnf/SmartHome/"
//		<< _Topic.m_Mac << "/" << _Topic.m_Site
//		<< "/" << _Topic.m_Position << "/"
//		<< _Topic.m_PosIdx << "/" << _Topic.m_Feature
//		<< "/" << _Topic.m_Type << "/" << _Topic.m_Index;   //Mac/Site/Position/PosIdx/GATEWAY/Type/Index";
//	if (!g_ServerManager->g_MqttManager.Publish(szStreamTopic.str(), (char*)szSendData, sizeof(szSendData)))
//		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "MQTT Submission version Check fail [%s] failure", szStreamTopic.str().c_str());
//	else
//		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "MQTT Submission version Check Success[%s] Success", szStreamTopic.str().c_str());
//}

bool CVersionManager::CheckDeviceVersion(ITopic _Topic, char* _BundleID, uint16_t _Ver)
{
	UChar szCRC[4] = { 0 };
	UChar szDefine = 0x00;
	UChar szDefineLen = 0x23;
	UChar szDefineCMD = CMD_DEVICE_VERSION_DATA;
	//UChar szPayload[36] = { 0 };
	CDataObject *dzPayload = m_dzDataObjectList._malloc_boost();
	
	stringstream szStreamTopic;
	*dzPayload << szDefineLen;
	*dzPayload << szDefineCMD;
	uint16_t _VerValue;
	uint32_t _FileCode, _FileSize;
	if (ReadSingleDB(_BundleID, _FileCode, _VerValue, _FileSize))
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "CheckDeviceVersion[%d][%d][%d][%d]", _Ver, _VerValue, _FileCode, _FileSize);
		if (_Ver < _VerValue)
		{
			*dzPayload << FUCK_VERSION_STATE_SUCCESS;

			map<UINT, CVersionObject*>::iterator col = m_VersionList.find(_FileCode);
			if (col != m_VersionList.end())
			{
				vector<UChar> &szVerFileArray = col->second->getFileVersion();
				szCRC[0] = szVerFileArray[76];
				szCRC[1] = szVerFileArray[77];
				szCRC[2] = szVerFileArray[78];
				szCRC[3] = szVerFileArray[79];
			}
		}
		else
			if (_Ver == _VerValue)
				*dzPayload << FUCK_VERSION_STATE_FAIL;
	}
	else
		*dzPayload << FUCK_VERSION_STATE_NOBUNDLE;
	int szBandleLen = strlen(_BundleID);
	*dzPayload << _BundleID;
	for (; szBandleLen < 16; szBandleLen++)
	{
		*dzPayload << szDefine;
	}
	//24 45 00 4E4C5F47574630314300000000000000 00 1802 00035316 0040 A723B3B1 00000052
	uint16_t szU16Temp = 0;
	uint32_t szU32Temp = 0;
	//序号
	*dzPayload << szDefine;
	//版本号
	szU16Temp = htons(_VerValue);
	*dzPayload << szU16Temp;
	//文件大小
	szU32Temp = htonl(_FileSize);
	*dzPayload << szU32Temp;
	//单包大小64
	UChar szDefineSize = htons(0x40);
	*dzPayload << szDefineSize;
	//单包大小
	*dzPayload << szCRC;
	//文件号
	szU32Temp = htonl(_FileCode);
	*dzPayload << szU32Temp;
	//序列号+版本号+文件大小+单包大小2字节+cRC校验+文件号
	
	//加校验
	unsigned short szCrc = htons(g_ServerManager->g_ServiceManager.GetCrc16Ccitt(CRC_BASE_KEY, (unsigned char*)dzPayload->m_Data, dzPayload->m_DataLen));
	*dzPayload << szCrc;

	if (g_ServerManager->getCrypt())
	//加密
	g_ServerManager->g_DeviceUuidManager.UnCryptProtocol(_Topic.m_Mac.c_str(), (UChar*)dzPayload->m_Data, dzPayload->m_DataLen);

	unsigned char *szTempData = (unsigned char *)m_dzConvertBuffList._malloc_boost();
	RtlZeroMemory(szTempData, MAX_BUFF_NET_SIZE);
	size_t szTempDataLen = MAX_BUFF_NET_SIZE;
#ifdef VIANS
	szStreamTopic << "Vians/v3/Cnf/SmartHome/"
		<< _Topic.m_Mac << "/" << _Topic.m_Site
		<< "/" << _Topic.m_Position << "/"
		<< _Topic.m_PosIdx << "/" << _Topic.m_Feature
		<< "/" << _Topic.m_Type << "/" << _Topic.m_Index;

	
	if (base64_encode(szTempData, &szTempDataLen, (UChar*)dzPayload->m_Data, dzPayload->m_DataLen) != 0)
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "CheckDeviceVersion base64_decode error");
		m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);
		m_dzDataObjectList._free_boost(dzPayload);
		return false;
	}
#else
	szStreamTopic << "Nexless/v2/Cnf/SmartHome/"
		<< _Topic.m_Mac << "/" << _Topic.m_Site
		<< "/" << _Topic.m_Position << "/"
		<< _Topic.m_PosIdx << "/" << _Topic.m_Feature
		<< "/" << _Topic.m_Type << "/" << _Topic.m_Index;   //Mac/Site/Position/PosIdx/GATEWAY/Type/Index";
	RtlCopyMemory(szTempData, dzPayload->m_Data, dzPayload->m_DataLen);
	szTempDataLen = dzPayload->m_DataLen;
#endif
	
	if (!g_ServerManager->g_MqttManager.Publish(szStreamTopic.str(), (char*)szTempData, szTempDataLen))
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "MQTT Submission version Check fail [%s] failure", szStreamTopic.str().c_str());
	else
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "MQTT Submission version Check Success[%s] Success", szStreamTopic.str().c_str());
	m_dzDataObjectList._free_boost(dzPayload);
	m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);
	return true;
}


bool CVersionManager::LimitedFileInfo(UINT _FileCode, UINT _Offet, UINT _SumSize, char* _Mac)
{
	Json::Value szRoot,szData ;
	szRoot["msgType"] = __FACK_REDIS_JG_UPGRADE__;

	szData["fileCode"] = _FileCode;
	szData["mac"] = _Mac;
	//float szPencent = _Offet / (float)col->second->getSumSize();
	uint32_t  szPercent = (int)(_Offet / (float)_SumSize * 100);
	//uint32_t szPercent = _Offet / (float)_SumSize * 100;
	szData["percent"] = szPercent;

	szRoot["msgData"] = szData;

	if (!szRoot.empty())
	{
		Json::FastWriter fast_writer;
		Json::ValueType type = szRoot.type();
		string SendStr = fast_writer.write(szRoot);
		std::stringstream szJson(SendStr);
		return g_ServerManager->g_RedisClientManager.channel(SERVER_REDIS_KEY, szJson);
	}
	else
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_ERROR, "JgPush Json::Value is null");
		return false;
	}
}
bool CVersionManager::LimitedOver( UINT _Ver, char* _Mac)
{
	Json::Value szRoot, szData;
	szRoot["msgType"] = __FACK_REDIS_JG_UPGRADE__END;

	//szData["fileCode"] = _FileCode;
	szData["mac"] = _Mac;
	szData["afterVersion"] = _Ver;
	szData["success"] = true;
	//szData["remark"] = "下载成功";rpush

	szRoot["msgData"] = szData;
	if (!szRoot.empty())
	{
		Json::FastWriter fast_writer;
		Json::ValueType type = szRoot.type();
		string SendStr = fast_writer.write(szRoot);
		std::stringstream szJson(SendStr);
		return g_ServerManager->g_RedisClientManager.channel(SERVER_REDIS_KEY, szJson);
	}
	else
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_ERROR, "JgPush Json::Value is null");
		return false;
	}
}