#include "ServerHead.h"



void CDeviceUuidManager::Initialize()
{
	if (g_ServerManager->getIAnalysisPacket())
		m_IAnalysisPacket = g_ServerManager->getIAnalysisPacket();
	ReadDB();
}

bool CDeviceUuidManager::ReadDB()
{
	MYSQL_RES *dzRes;
	MYSQL_ROW dzSqlrow;
	char *szRetConvert;
	char *dzSql = (char*)m_dzConvertBuffList._malloc_boost();
	snprintf(dzSql, MAX_BUFF_LOG_SIZE,
		"select SUBSTR(HEX(d_mac),5,12),d_uuid from xy_dc_deviceinfo;");

	dzRes = g_ServerManager->g_IMysqlInterFace->QuerySql(dzSql);
	m_dzConvertBuffList._free_boost((LPConvertBuffer)dzSql);
	if (!dzRes)
	{
		mysql_free_result(dzRes);
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "Database stored procedure access error");
	}
	else
	{
		while ((dzSqlrow = mysql_fetch_row(dzRes)))
		{
			if (mysql_num_fields(dzRes) < 2)
			{
				continue;
			}
			CDeviceUuidObject *szDeviceUuidObject = m_dzDeviceUuidPool._malloc_boost();
			if (!szDeviceUuidObject) return false;

			dzSqlrow[0] ? szDeviceUuidObject->setMac(dzSqlrow[0]) : szDeviceUuidObject->setMac("");
			ostringstream szTr;
			szTr << setw(16) << setfill('0') << dzSqlrow[1];
			dzSqlrow[1] ? szDeviceUuidObject->setDevUuid(szTr.str().c_str()) : szDeviceUuidObject->setDevUuid("");
			m_dzDeviceUuidData[szDeviceUuidObject->getMac()] = szDeviceUuidObject;
		}
		mysql_free_result(dzRes);

	}
	return true;
}
bool CDeviceUuidManager::ReadSingleDB(char *_Mac, char *_Uuid)
{
	MYSQL_RES *dzRes;
	MYSQL_ROW dzSqlrow;
	char *szRetConvert;
	char *dzSql = (char*)m_dzConvertBuffList._malloc_boost();
	snprintf(dzSql, MAX_BUFF_LOG_SIZE,
		"select SUBSTR(HEX(d_mac),5,12) ,d_uuid from xy_dc_deviceinfo where  SUBSTR(HEX(d_mac),5,12) = '%s';", _Mac);

	dzRes = g_ServerManager->g_IMysqlInterFace->QuerySql(dzSql);
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, " Find Mac Uuid[Mac=%s]", dzSql);
	m_dzConvertBuffList._free_boost((LPConvertBuffer)dzSql);
	
	if (!dzRes)
	{
		mysql_free_result(dzRes);
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "Database stored procedure access error");
	}
	else
	{
		
		while ((dzSqlrow = mysql_fetch_row(dzRes)))
		{
			if (mysql_num_fields(dzRes) < 2)
			{
				continue;
			}
			CDeviceUuidObject *szDeviceUuidObject = m_dzDeviceUuidPool._malloc_boost();
			if (!szDeviceUuidObject) return false;

			dzSqlrow[0] ? szDeviceUuidObject->setMac(dzSqlrow[0] + 4) : szDeviceUuidObject->setMac("");
			ostringstream szTr;
			szTr << setw(16) << setfill('0')<< dzSqlrow[1];
			szDeviceUuidObject->setDevUuid(szTr.str().c_str());
			m_dzDeviceUuidData[szDeviceUuidObject->getMac()] = szDeviceUuidObject;
			RtlZeroMemory(_Uuid, 16);
			RtlCopyMemory(_Uuid, szDeviceUuidObject->getDevUuid(), 16);
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "read DB %s",
				szDeviceUuidObject->getDevUuid());
		}
		mysql_free_result(dzRes);

	}
	return true;
}
bool CDeviceUuidManager::FindMacUuid(const char *_Mac, char *_Uuid)
{
	stringstream szStream;
	szStream << _Mac;
	map<string, CDeviceUuidObject*>::iterator col = m_dzDeviceUuidData.find(szStream.str());
	if (col != m_dzDeviceUuidData.end())
	{
		
		//找到了
		RtlZeroMemory(_Uuid, 16);
		RtlCopyMemory(_Uuid, col->second->getDevUuid(), 16);
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "Find Mac Uuid[Mac=%s][Uuid = %s][%s]", _Mac, _Uuid, col->second->getDevUuid());
	}
	else
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "not Find Mac Uuid[Mac=%s]", _Mac);
		return ReadSingleDB((char*)_Mac, _Uuid);
		//没找到
	}
	return true;
}


bool CDeviceUuidManager::UnCryptProtocol(const char * _Mac, UChar *_data, uint32_t _Length)
{
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "==========>[%s]",
		 _Mac);
	char szUuid[17] = { 0 };
	g_ServerManager->g_DeviceUuidManager.FindMacUuid(_Mac, szUuid);
	UChar szUuidBit[8] = { 0 };
	UChar szTemp = 0L;
	g_ServerManager->g_DeviceUuidManager.HexStrToByte(szUuid, szUuidBit, 16);
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "[%s    %s]",
		szUuid, _Mac);

	uint64_t szSrcUuid = 0L;
	RtlCopyMemory(&szSrcUuid, szUuidBit, sizeof(szSrcUuid));
	szSrcUuid = htonll(szSrcUuid); //(((uint64_t)htonl(szSrcUuid)) << 32) + htonl(szSrcUuid >> 32);//htonl(szSrcUuid);
	RtlCopyMemory(szUuidBit,&szSrcUuid , sizeof(szSrcUuid));
	
	/*g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "%02X %02X %02X %02X %02X %02X %02X %02X",
		szUuidBit[0], szUuidBit[1], szUuidBit[2], szUuidBit[3], szUuidBit[4], szUuidBit[5], szUuidBit[6], szUuidBit[7]);*/
	/*for (int i = 0; i < 4; i++)
	{
		szTemp = szUuidBit[i];
		szUuidBit[i] = szUuidBit[7 - i];
		szUuidBit[7 - i] = szTemp;
	}*/
	
	g_ServerManager->g_DeviceUuidManager.McuEnCrypt(_data, _Length, szUuidBit);
	return true;
}