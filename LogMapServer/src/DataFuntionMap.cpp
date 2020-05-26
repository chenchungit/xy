#include "ServerHead.h"


CDataFuntionMap::CDataFuntionMap(void)
{
}


CDataFuntionMap::~CDataFuntionMap(void)
{
}

/****************
*注册回调函数
******************/
void CDataFuntionMap::RegisterFuncation()
{
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "***************Register  Funcation Begin");
	IAnalysisPacket*szIAnalysisPacket = g_ServerManager->getIAnalysisPacket();
	if (!szIAnalysisPacket)
		return;
	szIAnalysisPacket->BindFunToMessage(SERVER_REGISTER_ANSWER, RegisterCallback);
	szIAnalysisPacket->BindFunToMessage(SERVER_SYSTEM_CODE, RegisterCallback);
	szIAnalysisPacket->BindFunToMessage(SERVER_INSERT_UPDATE_JGPUSHRECORD, SubmitJgPush);
	szIAnalysisPacket->BindFunToMessage(SERVER_INSERT_UPDATE_DEVICEALARM, SubmitDeviceAlarm);
	szIAnalysisPacket->BindFunToMessage(SERVER_INSERT_UPDATE_USERECORD, SubmitDeviceLog);

	//szIAnalysisPacket->BindFunToMessage(D2S_VERSION_FILE_REQUEST, getDeviceVersion);
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "******************Register  Funcation over");
	szIAnalysisPacket->BindFunToMessage(static_cast<string>("LOCK"), Lock);
	szIAnalysisPacket->BindFunToMessage(static_cast<string>("ALARM"), DeviceAlarm);
	szIAnalysisPacket->BindFunToMessage(static_cast<string>("GATEWAY"), DeviceGateWay);
	szIAnalysisPacket->BindFunToMessage(static_cast<string>("DOOR_ALARM"), DeviceDoorAlarm);
	szIAnalysisPacket->BindFunToMessage(static_cast<string>("SMOKE_ALARM"), DeviceSmokeAlarm);
	szIAnalysisPacket->BindFunToMessage(static_cast<string>("Version"), VersionTest);
	szIAnalysisPacket->BindFunToMessage(static_cast<string>("HUMAN_CNT"), HumanCnt);
	szIAnalysisPacket->BindFunToMessage(static_cast<string>("GAS_ALARM"), DeviceGASAlarm);
}

void  CDataFuntionMap::HumanCnt(ITopic _Topic, UChar * _data, const int _len)
{
	unsigned char *szTempData = (unsigned char *)g_ServerManager->g_ServiceManager.m_dzConvertBuffList._malloc_boost();
	size_t szTempDataLen = MAX_BUFF_NET_SIZE;
#ifdef VIANS
	if (base64_decode(szTempData, &szTempDataLen, _data, _len) != 0)
	{
		g_ServerManager->g_ServiceManager.m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);
		return;
	}
#else
	RtlCopyMemory(szTempData, _data, _len);
	szTempDataLen = _len;
#endif
	
		if (g_ServerManager->getCrypt())
			//解密
			g_ServerManager->g_DeviceUuidManager.UnCryptProtocol(_Topic.m_Mac.c_str(), szTempData, szTempDataLen);

		/*g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "[%d-%d-%d-%d-%d-%d-%d-%d]"
			, szUuidBit[0], szUuidBit[1], szUuidBit[2], szUuidBit[3], szUuidBit[4], szUuidBit[5], szUuidBit[6], szUuidBit[7]);*/

		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "\r\n\r\n\r\n\r\n====================Device Human Cnt");
		UChar szLen = 0;
		UChar szTrig;//触发标志
		uint16_t szIn, szOut, szSum;
		szIn = szOut = szSum = 0;
		UChar szData[8];
		UChar szDeviceMac[6] = { 0 };
		RtlCopyMemory(&szLen, szTempData, sizeof(UChar));
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "Device  Human Cnt [len=%d]", szLen);

		UChar szCMD = 0;
		RtlCopyMemory(&szCMD, szTempData + 1, sizeof(UChar));
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "Device  Human Cnt [len=%d][CMD=%d]", szLen, szCMD);
		if (szCMD == CMD_DEVICE_HUMANCNT)
		{
			RtlCopyMemory(&szTrig, szTempData + 2, 1);

			szData[0] = IFT_HUMAN_CNT;//指定为人数计数器
			//RtlCopyMemory(szData+1, _data + 3, 6);

			RtlCopyMemory(&szSum, szTempData + 3, 2);
			RtlCopyMemory(&szIn, szTempData + 5, 2);
			RtlCopyMemory(&szOut, szTempData + 7, 2);
			szSum = ntohs(szSum);
			szIn = ntohs(szIn);
			szOut = ntohs(szOut);

			RtlCopyMemory(szData + 1, &szTrig, sizeof(UChar));
			RtlCopyMemory(szData + 2, &szSum, sizeof(uint16_t));
			RtlCopyMemory(szData + 4, &szIn, sizeof(uint16_t));
			RtlCopyMemory(szData + 6, &szOut, sizeof(uint16_t));

			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "Device  Human Cnt[%d][%d][%d]", (int32_t)szSum, (int32_t)szIn, (int32_t)szOut);
			uint32_t szTime = 0;
			RtlCopyMemory(&szTime, szTempData + 9, 4);

			RtlCopyMemory(&szDeviceMac, szTempData + 13, MAC_UCHAR_LENGTH);
			char szMacTemp[32] = { 0 };
			char szDataTemp[32] = { 0 };

			g_ServerManager->g_ServiceManager.ByteToHexStr(szDeviceMac, szMacTemp, MAC_UCHAR_LENGTH);
			g_ServerManager->g_ServiceManager.ByteToHexStr(szData, szDataTemp, 8);

			g_ServerManager->g_DeviceAlarmManager.SubmitData(szMacTemp, time(NULL),
				time(NULL),
				(UINT)szTrig, 0, (UChar *)szDataTemp, 16);
		}
	g_ServerManager->g_ServiceManager.m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);
}
void  CDataFuntionMap::VersionTest(ITopic _Topic, UChar * _data, const int _len)
{
	g_ServerManager->g_VersionManager.DownVerFileData(_Topic,112);
}

void  CDataFuntionMap::DeviceGASAlarm(ITopic _Topic, UChar * _data, const int _len)
{
	unsigned char *szTempData = (unsigned char *)g_ServerManager->g_ServiceManager.m_dzConvertBuffList._malloc_boost();
	size_t szTempDataLen = MAX_BUFF_NET_SIZE;
#ifdef VIANS
	if (base64_decode(szTempData, &szTempDataLen, _data, _len) != 0)
	{
		g_ServerManager->g_ServiceManager.m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);
		return;
	}
#else
	RtlCopyMemory(szTempData, _data, _len);
	szTempDataLen = _len;
#endif
		if (g_ServerManager->getCrypt())
			//解密
			g_ServerManager->g_DeviceUuidManager.UnCryptProtocol(_Topic.m_Mac.c_str(), szTempData, szTempDataLen);

		/*g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "[%d-%d-%d-%d-%d-%d-%d-%d]"
			, szUuidBit[0], szUuidBit[1], szUuidBit[2], szUuidBit[3], szUuidBit[4], szUuidBit[5], szUuidBit[6], szUuidBit[7]);*/

		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "\r\n\r\n\r\n\r\n====================Device Gas Alarm");
		UChar szLen = 0;
		UChar szAlarmType;
		UChar szData[2];
		UChar szDeviceMac[6] = { 0 };
		RtlCopyMemory(&szLen, szTempData, sizeof(UChar));
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "Device Gas Alarm [len=%d]", szLen);

		UChar szCMD = 0;
		RtlCopyMemory(&szCMD, szTempData + 1, sizeof(UChar));
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "Device Gas Alarm [len=%d][CMD=%d]", szLen, szCMD);
		if (szCMD == CMD_DEVICE_ALARM)
		{
			RtlCopyMemory(&szAlarmType, szTempData + 2, 1);
			uint32_t szTime = 0;
			RtlCopyMemory(&szTime, szTempData + 3, 4);
			szData[0] = IFT_GAS_SENSOR;//指定为可燃报警器
			RtlCopyMemory(szData + 1, szTempData + 7, 1);
			RtlCopyMemory(&szDeviceMac, szTempData + 8, MAC_UCHAR_LENGTH);
			char szMacTemp[32] = { 0 };
			char szDataTemp[32] = { 0 };

			g_ServerManager->g_ServiceManager.ByteToHexStr(szDeviceMac, szMacTemp, MAC_UCHAR_LENGTH);
			g_ServerManager->g_ServiceManager.ByteToHexStr(szData, szDataTemp, 2);

			g_ServerManager->g_DeviceAlarmManager.SubmitData(szMacTemp, time(NULL),
				time(NULL),
				szAlarmType, 0, (UChar *)szDataTemp, 4);
		}
	g_ServerManager->g_ServiceManager.m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);
}
void  CDataFuntionMap::DeviceSmokeAlarm(ITopic _Topic, UChar * _data, const int _len)
{
	unsigned char *szTempData = (unsigned char *)g_ServerManager->g_ServiceManager.m_dzConvertBuffList._malloc_boost();
	size_t szTempDataLen = MAX_BUFF_NET_SIZE;
#ifdef VIANS
	if (base64_decode(szTempData, &szTempDataLen, _data, _len) != 0)
	{
		g_ServerManager->g_ServiceManager.m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);
		return;
	}
#else
	RtlCopyMemory(szTempData, _data, _len);
	szTempDataLen = _len;
#endif
		if (g_ServerManager->getCrypt())
			//解密
			g_ServerManager->g_DeviceUuidManager.UnCryptProtocol(_Topic.m_Mac.c_str(), szTempData, szTempDataLen);

		/*g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "[%d-%d-%d-%d-%d-%d-%d-%d]"
			, szUuidBit[0], szUuidBit[1], szUuidBit[2], szUuidBit[3], szUuidBit[4], szUuidBit[5], szUuidBit[6], szUuidBit[7]);*/

		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "\r\n\r\n\r\n\r\n====================Device Smoke Alarm");
		UChar szLen = 0;
		UChar szAlarmType;
		UChar szData[2];
		UChar szDeviceMac[6] = { 0 };
		RtlCopyMemory(&szLen, szTempData, sizeof(UChar));
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "Device Smoke Alarm [len=%d]", szLen);

		UChar szCMD = 0;
		RtlCopyMemory(&szCMD, szTempData + 1, sizeof(UChar));
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "Device Smoke Alarm [len=%d][CMD=%d]", szLen, szCMD);
		if (szCMD == CMD_DEVICE_ALARM)
		{
			RtlCopyMemory(&szAlarmType, szTempData + 2, 1);
			uint32_t szTime = 0;
			RtlCopyMemory(&szTime, szTempData + 3, 4);
			szData[0] = IFT_SMOKE_ALARM;//指定为烟雾报警器
			RtlCopyMemory(szData + 1, szTempData + 7, 1);
			RtlCopyMemory(&szDeviceMac, szTempData + 8, MAC_UCHAR_LENGTH);
			char szMacTemp[32] = { 0 };
			char szDataTemp[32] = { 0 };

			g_ServerManager->g_ServiceManager.ByteToHexStr(szDeviceMac, szMacTemp, MAC_UCHAR_LENGTH);
			g_ServerManager->g_ServiceManager.ByteToHexStr(szData, szDataTemp, 2);

			g_ServerManager->g_DeviceAlarmManager.SubmitData(szMacTemp, time(NULL),
				time(NULL),
				szAlarmType, 0, (UChar *)szDataTemp, 4);
		}
	g_ServerManager->g_ServiceManager.m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);
}
void  CDataFuntionMap::DeviceDoorAlarm(ITopic _Topic, UChar * _data, const int _len)
{
	//base64 decode
	unsigned char *szTempData = (unsigned char *)g_ServerManager->g_ServiceManager.m_dzConvertBuffList._malloc_boost();
	size_t szTempDataLen = MAX_BUFF_NET_SIZE;
#ifdef VIANS
	if (base64_decode(szTempData, &szTempDataLen, _data, _len) != 0)
	{
		g_ServerManager->g_ServiceManager.m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);
		return;
	}
#else
	RtlCopyMemory(szTempData, _data, _len);
	szTempDataLen = _len;
#endif
		if (g_ServerManager->getCrypt())
			//解密
			g_ServerManager->g_DeviceUuidManager.UnCryptProtocol(_Topic.m_Mac.c_str(), szTempData, szTempDataLen);

		/*g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "[%d-%d-%d-%d-%d-%d-%d-%d]"
			, szUuidBit[0], szUuidBit[1], szUuidBit[2], szUuidBit[3], szUuidBit[4], szUuidBit[5], szUuidBit[6], szUuidBit[7]);*/

		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "\r\n\r\n\r\n\r\n====================Device Smoke Alarm");
		UChar szLen = 0;
		UChar szAlarmType;
		UChar szData[2];
		UChar szDeviceMac[6] = { 0 };
		RtlCopyMemory(&szLen, szTempData, sizeof(UChar));
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "Device Smoke Alarm [len=%d]", szLen);

		UChar szCMD = 0;
		RtlCopyMemory(&szCMD, szTempData + 1, sizeof(UChar));
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "Device Smoke Alarm [len=%d][CMD=%d]", szLen, szCMD);
		if (szCMD == CMD_DEVICE_ALARM)
		{
			RtlCopyMemory(&szAlarmType, szTempData + 2, 1);
			uint32_t szTime = 0;
			RtlCopyMemory(&szTime, szTempData + 3, 4);
			szData[0] = IFT_DOOR_ALARM;//指定为烟雾报警器
			RtlCopyMemory(szData + 1, szTempData + 7, 1);
			RtlCopyMemory(&szDeviceMac, szTempData + 8, MAC_UCHAR_LENGTH);
			char szMacTemp[32] = { 0 };
			char szDataTemp[32] = { 0 };

			g_ServerManager->g_ServiceManager.ByteToHexStr(szDeviceMac, szMacTemp, MAC_UCHAR_LENGTH);
			g_ServerManager->g_ServiceManager.ByteToHexStr(szData, szDataTemp, 2);

			g_ServerManager->g_DeviceAlarmManager.SubmitData(szMacTemp, time(NULL),
				time(NULL),
				szAlarmType, 0, (UChar *)szDataTemp, 4);
		}
	g_ServerManager->g_ServiceManager.m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);
}
void  CDataFuntionMap::DeviceGateWay(ITopic _Topic, UChar * _data, const int _len)
{
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "\r\n\r\n\r\n\r\n====================GateWay[%d]", _len);
	//base64 decode
	unsigned char *szTempData = (unsigned char *)g_ServerManager->g_ServiceManager.m_dzConvertBuffList._malloc_boost();
	size_t szTempDataLen = MAX_BUFF_NET_SIZE;
#ifdef VIANS
	if (base64_decode(szTempData, &szTempDataLen, _data, _len) != 0)
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "GateWay base64_decode error");
		g_ServerManager->g_ServiceManager.m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);
		return;
	}
#else
	RtlCopyMemory(szTempData, _data, _len);
	szTempDataLen = _len;
#endif
		if (g_ServerManager->getCrypt())
			//解密
			g_ServerManager->g_DeviceUuidManager.UnCryptProtocol(_Topic.m_Mac.c_str(), szTempData, szTempDataLen);


		UChar szLen = 0;
		RtlCopyMemory(&szLen, szTempData, sizeof(UChar));

		UChar szCMD = 0;
		RtlCopyMemory(&szCMD, szTempData + 1, sizeof(UChar));
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "GateWay [len=%d][CMD=%d]", szLen, szCMD);
		/*for (int i = 0; i < szTempDataLen; i++)
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL,"%02X" , szTempData[i]);*/
		switch (szCMD)
		{
		case CMD_XS://校时
		{
			std::stringstream szTopic;
			szTopic << _Topic.m_Company << "/" << _Topic.m_Ver << "/" << "Cnf/" << _Topic.m_Name << "/" << _Topic.m_Mac << "/" <<
				_Topic.m_Site << "/" << _Topic.m_Position << "/" << _Topic.m_PosIdx << "/" << _Topic.m_Feature << "/" << _Topic.m_Type << "/" <<
				_Topic.m_Index;
			/*std::stringstream szPayload;
			szPayload << 0x06 << 0x0E << htonl(time(NULL));


			unsigned short szCrc = htons(g_ServerManager->g_ServiceManager.GetCrc16Ccitt(CRC_BASE_KEY, (unsigned char*)szPayload, 6));
			RtlCopyMemory(szPayload + 6, &szCrc, sizeof(uint16_t));*/


			UChar szPayload[8] = { 0 };
			szPayload[0] = 0x06;
			szPayload[1] = 0x0E;
			uint32_t szTime = htonl(time(NULL));
			RtlCopyMemory(szPayload + 2, &szTime, sizeof(uint32_t));
			//加校验
			unsigned short szCrc = htons(g_ServerManager->g_ServiceManager.GetCrc16Ccitt(CRC_BASE_KEY, (unsigned char*)szPayload, 6));
			RtlCopyMemory(szPayload + 6, &szCrc, sizeof(uint16_t));

			if (g_ServerManager->getCrypt())
				//加密
				g_ServerManager->g_DeviceUuidManager.UnCryptProtocol(_Topic.m_Mac.c_str(), szPayload, sizeof(szPayload));

#ifdef VIANS
			
			szTempData = (unsigned char *)g_ServerManager->g_ServiceManager.m_dzConvertBuffList._malloc_boost();
			RtlZeroMemory(szTempData, MAX_BUFF_NET_SIZE);
			szTempDataLen = MAX_BUFF_NET_SIZE;
			if (base64_encode(szTempData, &szTempDataLen, szPayload, sizeof(szPayload)) != 0)
			{
				g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "GateWay base64_decode error");
				g_ServerManager->g_ServiceManager.m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);
				return;
			}
#else
			RtlCopyMemory(szTempData, szPayload, sizeof(szPayload));
			szTempDataLen = _len;
#endif

			g_ServerManager->g_MqttManager.Publish(szTopic.str(), (char*)szTempData, szTempDataLen);
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "GateWay timer [Topic=%s][CMD=%d][time=%02X]", szTopic.str().c_str(), szCMD, szTime);
			break;
		}
		case CMD_FILE_UP_DOWNLOAD://下载文件0B 46 00000052 00000000 40 D4DB
		{

			uint32_t szFileCode, szOffset;
			uint8_t szLength = 0;
			szFileCode = szOffset = 0;
			RtlCopyMemory(&szFileCode, szTempData + 2, sizeof(uint32_t));
			szFileCode = ntohl(szFileCode);
			RtlCopyMemory(&szOffset, szTempData + 2 + sizeof(uint32_t), sizeof(uint32_t));
			szOffset = ntohl(szOffset);
			//	RtlCopyMemory(&szLength, szTempData + 2 + sizeof(uint32_t) + sizeof(uint32_t), sizeof(uint16_t));
			szLength = szTempData[10];
			g_ServerManager->g_VersionManager.DownVerFileData(_Topic, szFileCode, szOffset, szLength);
			break;
		}
		case CMD_VERSION_NUMBER_UPDATE:
		{
			uint16_t szVerCode;
			UChar szDevMac[6] = { 0 };
			char szDevMacStr[12] = { 0 };
			RtlCopyMemory(&szVerCode, szTempData + 19, sizeof(uint16_t));
			RtlCopyMemory(&szDevMac, szTempData + 21, MAC_UCHAR_LENGTH);
			g_ServerManager->g_ServiceManager.ByteToHexStr(szDevMac, szDevMacStr, MAC_UCHAR_LENGTH);
			if (g_ServerManager->g_VersionManager.SaveDeviceVer(szDevMacStr, ntohs(szVerCode)))
				g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "Update mac = %s set ver = %d ", szDevMacStr, szVerCode);

			g_ServerManager->g_VersionManager.LimitedOver(ntohs(szVerCode), szDevMacStr);
			break;
		}
		case CMD_TIME_HEART://心跳
		{
			time_t szTime;
			UChar szDevMac[6] = { 0 };
			char szDevMacStr[13] = { 0 };
			RtlCopyMemory(&szDevMac, szTempData + 2, MAC_UCHAR_LENGTH);
			RtlCopyMemory(&szTime, szTempData + 2 + MAC_UCHAR_LENGTH, sizeof(uint32_t));
			szTime = ntohl(szTime);
			g_ServerManager->g_ServiceManager.ByteToHexStr(szDevMac, szDevMacStr, MAC_UCHAR_LENGTH);
			char szBuf[256] = { 0 };
			strftime(szBuf, sizeof(szBuf), "%Y-%m-%d %H:%M:%S", localtime(&szTime));
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "time heart = %s Mac = %s ", szBuf, szDevMacStr);
			break;
		}
		case CMD_DEVICE_VERSION://硬件比对版本
		{
			time_t szTime;
			char szDevModel[16] = { 0 };
			uint16_t szDevVer = 0;
			RtlCopyMemory(&szDevModel, szTempData + 2, BANDLE_ID_LENGTH);
			RtlCopyMemory(&szDevVer, szTempData + 3 + BANDLE_ID_LENGTH, sizeof(uint16_t));
			szDevVer = ntohs(szDevVer);

			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "device Version DevModel= %s DevVer = %d", szDevModel, szDevVer);
			g_ServerManager->g_VersionManager.CheckDeviceVersion(_Topic, szDevModel, szDevVer);
			break;
		}

		case CMD_GET_QUEUE_DEVICE://查询设备
		{
			
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "===================%d=====================", _len);
			unsigned char szOutput[16] = { 0 };
			char szUserName[32] = { 0 };
			char szPsw[33] = { 0 };
			char szSite[32] = { 0 };
			uint16_t szPos, szFet, szPsw16[32] = {0};
			uint8_t szPidx, szIdx,szType,szTempNameLen, szTempPswLen, szTempSiteLen;

			szTempSiteLen = szTempPswLen = szTempNameLen = szPidx = szType = szIdx = 0;
			szPos = szFet = 0;

			//获取名字长度
			RtlCopyMemory(&szTempNameLen, szTempData + 2, sizeof(szTempNameLen));

			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "===================user len %d=====================", szTempNameLen);
			RtlCopyMemory(&szUserName, szTempData + 2 + sizeof(szTempNameLen), szTempNameLen);
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "===================user name %s=====================", szUserName);
			//获取密码
			RtlCopyMemory(&szTempPswLen, szTempData + 2 + sizeof(szTempNameLen)+ szTempNameLen, sizeof(szTempPswLen));
			RtlCopyMemory(&szPsw, szTempData + 2 + sizeof(szTempNameLen) * 2 + szTempNameLen, szTempPswLen);
			for (int i = 0; i < szTempPswLen;i++)
				szPsw16[i] = szPsw[i];
			md5((unsigned char*)szPsw16, szTempPswLen*2, szOutput);
			RtlZeroMemory(szPsw, 33);
			g_ServerManager->g_ServiceManager.ByteToHexStr(szOutput, szPsw, 16);

			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "===================user szPsw %s=====================", szPsw);

			//获取site
			RtlCopyMemory(&szTempSiteLen, szTempData + 2 + sizeof(szTempNameLen) * 2 + szTempNameLen + szTempPswLen, sizeof(szTempSiteLen));
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "===================site len %d=====================", szTempSiteLen);
			RtlCopyMemory(&szSite, szTempData + 2 + sizeof(szTempNameLen) * 3 + szTempNameLen + szTempPswLen, szTempSiteLen);
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "===================site  %s=====================", szSite);
			//获取pos
			RtlCopyMemory(&szPos, szTempData + 2 + sizeof(szTempNameLen) * 3 + szTempNameLen + szTempPswLen + szTempSiteLen , sizeof(szPos));
			szPos = ntohs(szPos);
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "===================szPos %d=====================", szPos);
			//获取szPidx
			RtlCopyMemory(&szPidx, szTempData + 2 + sizeof(szTempNameLen) * 3 + szTempNameLen + szTempPswLen + szTempSiteLen + sizeof(szPos), sizeof(szPidx));
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "===================szPidx %d=====================", szPidx);
			//获取szFet
			RtlCopyMemory(&szFet, szTempData + 2 + sizeof(szTempNameLen) * 3 + szTempNameLen + szTempPswLen + szTempSiteLen + sizeof(szPos) + sizeof(szPidx), sizeof(szFet));
			szFet = ntohs(szFet);
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "===================szFet %d=====================", szFet);
			//获取szType
			RtlCopyMemory(&szType, szTempData + 2 + sizeof(szTempNameLen) * 3 + szTempNameLen + szTempPswLen + szTempSiteLen + sizeof(szPos) + sizeof(szPidx) + sizeof(szFet), sizeof(szType));
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "===================szType %d=====================", szType);
			//获取szTypeIdx
			RtlCopyMemory(&szIdx, szTempData + 2 + sizeof(szTempNameLen) * 3 + szTempNameLen + szTempPswLen + szTempSiteLen + sizeof(szPos) + sizeof(szPidx) + sizeof(szFet)+ sizeof(szType), sizeof(szIdx));
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "===================szIdx %d=====================", szIdx);

			 
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "CMD_GET_QUEUE_DEVICE UserName= %s Psw = %s Site=%s Pos=%d Pid=%d Fet=%d Type=%d Idx=%d",
				szUserName, szPsw, szSite, szPos, szPidx, szFet, szType, szIdx);
			g_ServerManager->g_ServiceManager.QueueDeviceInfo(szUserName, szPsw, szSite, szPos, szPidx, szFet, szType, szIdx, _Topic);
			break;
		}
		}
	g_ServerManager->g_ServiceManager.m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);
}
void  CDataFuntionMap::DeviceAlarm(ITopic _Topic, UChar * _data, const int _len)
{
	//base64 decode
	unsigned char *szTempData = (unsigned char *)g_ServerManager->g_ServiceManager.m_dzConvertBuffList._malloc_boost();
	size_t szTempDataLen = MAX_BUFF_NET_SIZE;
#ifdef VIANS
	if (base64_decode(szTempData, &szTempDataLen, _data, _len) != 0)
	{
		g_ServerManager->g_ServiceManager.m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);
		return;
	}
#else
	RtlCopyMemory(szTempData, _data, _len);
	szTempDataLen = _len;
#endif
		if (g_ServerManager->getCrypt())
			//解密
			g_ServerManager->g_DeviceUuidManager.UnCryptProtocol(_Topic.m_Mac.c_str(), szTempData, szTempDataLen);


		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "\r\n\r\n\r\n\r\n====================DeviceAlarm");
		UChar szLen = 0;
		UChar szAlarmType;
		UChar szData[2];
		UChar szDeviceMac[6] = { 0 };
		RtlCopyMemory(&szLen, szTempData, sizeof(UChar));
		g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "Alarm [len=%d]", szLen);
		if (szLen == 14)
		{
			UChar szCMD = 0;
			RtlCopyMemory(&szCMD, szTempData + 1, sizeof(UChar));
			g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "Alarm [len=%d][CMD=%d]", szLen, szCMD);
			if (szCMD == CMD_DEVICE_ALARM)
			{
				RtlCopyMemory(&szAlarmType, szTempData + 2, 1);
				uint32_t szTime = 0;
				RtlCopyMemory(&szTime, szTempData + 3, 4);
				szData[0] = IFT_ALARM;//指定为红外报警器
				RtlCopyMemory(szData + 1, szTempData + 7, 1);
				RtlCopyMemory(&szDeviceMac, szTempData + 8, MAC_UCHAR_LENGTH);
				char szMacTemp[32] = { 0 };
				char szDataTemp[32] = { 0 };

				g_ServerManager->g_ServiceManager.ByteToHexStr(szDeviceMac, szMacTemp, MAC_UCHAR_LENGTH);
				g_ServerManager->g_ServiceManager.ByteToHexStr(szData, szDataTemp, 2);

				g_ServerManager->g_DeviceAlarmManager.SubmitData(szMacTemp, time(NULL),
					time(NULL),
					szAlarmType, 0, (UChar *)szDataTemp, 4);
			}
		}
	g_ServerManager->g_ServiceManager.m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);
}
void CDataFuntionMap::Lock(ITopic _Topic, UChar * _data, const int _len)
{
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "\r\n\r\n\r\n\r\n====================Lock Log[%d][%s]", _len, _Topic.m_Mac.c_str());
	//base64 decode
	unsigned char *szTempData = (unsigned char *)g_ServerManager->g_ServiceManager.m_dzConvertBuffList._malloc_boost();
	size_t szTempDataLen = MAX_BUFF_NET_SIZE;
#ifdef VIANS
	if (base64_decode(szTempData, &szTempDataLen, _data, _len) != 0)
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "Lock base64_decode error");
		g_ServerManager->g_ServiceManager.m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);
		return;
	}
#else
	RtlCopyMemory(szTempData, _data, _len);
	szTempDataLen = _len;
#endif
		//解密
		if (g_ServerManager->getCrypt())
			g_ServerManager->g_DeviceUuidManager.UnCryptProtocol(_Topic.m_Mac.c_str(), szTempData, _len);

		UChar szLen = 0;
		UChar szCardType, szFuncType, szLogType, szReserved;
		RtlCopyMemory(&szLen, szTempData, sizeof(UChar));
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "[len=%d]", szLen);
		if (szLen == 24)
		{
			UChar szCMD = 0;
			RtlCopyMemory(&szCMD, szTempData + sizeof(UChar), sizeof(UChar));
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "[len=%d][CMD=%d]", szLen, szCMD);//494CF5CBC2C0782E
			if (szCMD == CMD_LOCK_LOG)
			{
				unsigned char szCardNum[8] = { 0 };
				RtlCopyMemory(&szCardNum, szTempData + 2, 8);

				UChar szDeviceMac[6] = { 0 };
				//RtlCopyMemory(&szCardNum, szTempData + (sizeof(UChar) * 2), 8);

				uint32_t szTime = 0;
				RtlCopyMemory(&szTime, szTempData + 10, 4);
				RtlCopyMemory(&szCardType, szTempData + 14, 1);
				RtlCopyMemory(&szFuncType, szTempData + 15, 1);
				RtlCopyMemory(&szLogType, szTempData + 16, 1);
				RtlCopyMemory(&szReserved, szTempData + 17, 1);
				RtlCopyMemory(&szDeviceMac, szTempData + 18, MAC_UCHAR_LENGTH);

				g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "[len=%d][CMD=%d][LogType = %d]", szLen, szCMD, szLogType);

				char szCardTemp[32] = { 0 };
				char szMacTemp[32] = { 0 };
				g_ServerManager->g_ServiceManager.ByteToHexStr(szCardNum, szCardTemp, 8);
				g_ServerManager->g_ServiceManager.ByteToHexStr(szDeviceMac, szMacTemp, MAC_UCHAR_LENGTH);


				if (szLogType == DEVICE_LOG_TYPE_2)
				{
					unsigned char szData[128] = { 0x00 };
					RtlZeroMemory(szData, 128);
					szData[0] = IFT_LOCK;
					szData[1] = szLogType & 0xff;
					szData[2] = szCardType & 0xff;
					szData[3] = szFuncType & 0xff;
					RtlCopyMemory((char*)&szData[4], (char*)szCardNum, 8);
					//memcpy_s(&szData[3 + 8], szDeviceLog.message().length(), szDeviceLog.message().c_str(), szDeviceLog.message().length());
					char szDataTemp[32] = { 0 };
					g_ServerManager->g_ServiceManager.ByteToHexStr(szData, szDataTemp, MAC_STRING_LENGTH);
					g_ServerManager->g_DeviceAlarmManager.SubmitData(szMacTemp, time(NULL), szTime,
						szReserved, 0, (UChar*)szDataTemp, 24);
				}
				else
					//if (szLogType == DEVICE_LOG_TYPE_0)
				{
					g_ServerManager->g_UseRecordManager.SubmitData(szMacTemp, szCardTemp,
						szFuncType,
						szTime,
						time(NULL), "", szLogType, szCardType, _Topic.m_Mac, 0);
				}
			}
		}
	g_ServerManager->g_ServiceManager.m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);

}
void CDataFuntionMap::getDeviceVersion(ITopic _Topic, UChar * _data, const int _len)
{
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "get  Version............");
	/*IAnalysisPacket *szIAnalysisPacket = g_ServerManager->getIAnalysisPacket();
	DeviceVersionRequest szDeviceVersionRequest;
	if (szIAnalysisPacket)
	{

		if (szIAnalysisPacket->ParsePartialData(&szDeviceVersionRequest, _data, _len)
			== PROTOCOL_TYPE_OK)
		{
			VersionParam szVersionParam = szDeviceVersionRequest.versionparamrequest();
			g_ServerManager->g_VersionManager.DownVerFileData(szVersionParam.describeflag(), szVersionParam.index(),
				szVersionParam.offset(), szVersionParam.maxlenght(), szDeviceVersionRequest.devmac());
		}
	}*/
}
void CDataFuntionMap::SubmitDeviceAlarm(TcpSocketID id, TcpSocketID _Token, UChar * _data, const int _len, int _Code)
{
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "Device Log record");
	IAnalysisPacket *szIAnalysisPacket = g_ServerManager->getIAnalysisPacket();
	SFrameDeviceAlarm szSFrameDeviceAlarm;
	if (szIAnalysisPacket)
	{

		if (szIAnalysisPacket->ParsePartialData(&szSFrameDeviceAlarm, _data, _len)
			== PROTOCOL_TYPE_OK)
		{
			ServerProtocol::DeviceAlarm szDeviceAlarm = szSFrameDeviceAlarm.devicealarmlist(0);
			g_ServerManager->g_DeviceAlarmManager.SubmitData(szDeviceAlarm.devicemac(), (UINT)szDeviceAlarm.curtime(),
				(UINT)szDeviceAlarm.heppentime(), (UINT)szDeviceAlarm.state(), (UINT)szDeviceAlarm.devsort(),
				(UChar*)szDeviceAlarm.devstate().c_str(), szDeviceAlarm.devstate().length());
		}
	}
}
void CDataFuntionMap::SubmitDeviceLog(TcpSocketID id, TcpSocketID _Token, UChar * _data, const int _len, int _Code)
{
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "Device Log record");
	IAnalysisPacket *szIAnalysisPacket = g_ServerManager->getIAnalysisPacket();
	SFramUseRecord szSFramUseRecord;

	if (szIAnalysisPacket)
	{

		if (szIAnalysisPacket->ParsePartialData(&szSFramUseRecord, _data, _len)
			== PROTOCOL_TYPE_OK)
		{
			SUseRecord szSUseRecord = szSFramUseRecord.suserecordlist(0);
			g_ServerManager->g_UseRecordManager.SubmitData(szSUseRecord.mac(), szSUseRecord.user(), szSUseRecord.usetype(),
				szSUseRecord.usertime(), szSUseRecord.uptime(), szSUseRecord.remark(),
				szSUseRecord.logtype(), szSUseRecord.cartype(), szSUseRecord.gwmac(), szSUseRecord.areadeviceid());
		}
	}
}
void CDataFuntionMap::SubmitJgPush(TcpSocketID id, TcpSocketID _Token, UChar * _data, const int _len, int _Code)
{
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "jg push record");
	IAnalysisPacket *szIAnalysisPacket = g_ServerManager->getIAnalysisPacket();
	SFrameJgPushRecord szSFrameJgPushRecord;

	if (szIAnalysisPacket)
	{

		if (szIAnalysisPacket->ParsePartialData(&szSFrameJgPushRecord, _data, _len)
			== PROTOCOL_TYPE_OK)
		{
			JgPushRecord szJgPushRecord = szSFrameJgPushRecord.jgpushrecordlist(0);
			g_ServerManager->g_JgPushRecordManager.SubmitData(szJgPushRecord.tagalias().c_str(),
				szJgPushRecord.contenttitle().c_str(),
				szJgPushRecord.content().c_str());
		}
	}
}
void CDataFuntionMap::Systemcmd(TcpSocketID id, TcpSocketID _Token, UChar * _data, const int32_t _len, int32_t _Code)
{
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "Recv Sevice Code info");
	IAnalysisPacket *szIAnalysisPacket = g_ServerManager->getIAnalysisPacket();
	SystemCode szSystemCode;

	if (szIAnalysisPacket)
	{

		if (szIAnalysisPacket->ParsePartialData(&szSystemCode, _data, _len)
			== PROTOCOL_TYPE_OK)
		{
			//g_ServerManager->g_ItemManager.SubmitData(id, szSFrameItem);
		}
	}
}

void CDataFuntionMap::RegisterCallback(TcpSocketID id, TcpSocketID _Token, UChar * _data, const int32_t _len, int32_t _Code)
{
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_SYSTEM, "===>Recv Register Service");
	uint32_t szLen = 0;
	UChar* szData = NULL;
	IAnalysisPacket*szIAnalysisPacket = g_ServerManager->getIAnalysisPacket();
	if (szIAnalysisPacket)
	{
		FrameRegisterServer szRegisterServer;
		if (szIAnalysisPacket->ParsePartialData(&szRegisterServer, _data, _len) == PROTOCOL_TYPE_OK)
		{
			CServiceObject *szObject = g_ServerManager->g_ServiceManager.FindFreeConnect(szRegisterServer.id());
			if (NULL == szObject)
			{
				szObject = new CServiceObject(szRegisterServer.type(),
					szRegisterServer.id(), id, YD_SERVICE_STATE_ON/*,
																  g_ServerManager->m_MassIP,g_ServerManager->m_MassPort*/);


				g_ServerManager->g_ServiceManager.InsertServiceObject(szObject);
				g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_SYSTEM, "new Register Service");
			}
			else
			{
				szObject->UpdateParameter(szRegisterServer.type(),
					szRegisterServer.id(), id, YD_SERVICE_STATE_ON/*,
																  g_ServerManager->m_MassIP,g_ServerManager->m_MassPort*/);
				g_ServerManager->g_ServiceManager.InsertServiceObject(szObject);

				g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_SYSTEM, "Update Service");
			}
			//发送注册成功消息
			FrameRegisterServerAnswer szRegisterServerAnswer;
			szRegisterServerAnswer.set_id(g_ServerManager->getServerID());
			szRegisterServerAnswer.set_state(REGISTER_GW_SUCCESSFULL);
			//g_ServerManager->g_UserInfoManager.InitUserSubscribe(szRegisterServerAnswer);

			szData = (UChar*)g_ServerManager->g_ServiceManager.m_dzConvertBuffList._malloc_boost();
			if (szData && szIAnalysisPacket->SerializeData(szData, szLen, &szRegisterServerAnswer) == PROTOCOL_TYPE_OK)
			{
				g_ServerManager->m_SocketService->TrsportTokenMessage(id, id, SERVER_REGISTER_ANSWER, (char*)szData,
					szLen);


				g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_SYSTEM, "Service Register Success, Service type=%d Service ID=%d subsci count= %d",
					szRegisterServer.type(), szRegisterServer.id(), szRegisterServerAnswer.frameuserinfolist_size());
			}
			g_ServerManager->g_ServiceManager.m_dzConvertBuffList._free_boost((LPConvertBuffer)szData);

			//同步数据
			//g_ServerManager->g_ServiceManager.SynchronousInitData(id, szRegisterServer.type());
		}
		else
		{
			//发送注册成功消息
			FrameRegisterServerAnswer szRegisterServerAnswer;
			szRegisterServerAnswer.set_id(0);
			szRegisterServerAnswer.set_state(REGISTER_GW_FAIL);

			szData = (UChar*)g_ServerManager->g_ServiceManager.m_dzConvertBuffList._malloc_boost();
			if (szData && szIAnalysisPacket->SerializeData(szData, szLen, &szRegisterServerAnswer) == PROTOCOL_TYPE_OK)
				g_ServerManager->m_SocketService->TrsportTokenMessage(id, id, SERVER_REGISTER_ANSWER, (char*)szData,
					szLen);
			/*g_ServerManager->m_SocketService->TrsportMessage(SERVER_REGISTER_ANSWER, szData,
				szLen, id, GAME_S2O_SERVER);*/
			g_ServerManager->g_ServiceManager.m_dzConvertBuffList._free_boost((LPConvertBuffer)szData);
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_SYSTEM, "Service Register Fail");
		}
	}
}


void CDataFuntionMap::QueueDeviceInfo(char *_Name, char* _Psw, char* _Site, int _Pos, int _Pidx, int _Fet, int _Type, int _Idx,ITopic _Topic )
{
	enum ErrorCode
	{
		DEVICE_NO_REGISTOR = 0,
		DEVICE_USERPSW_ERROR = 1,
		DEVICE_MUBIAO_NO_REGISTOR = 2,
		DEVICE_NO_USER = 3,
		DEVICE_NO_GW_DEVICE = 4
	};
	uint8_t szErrotCode = 0;
	/*SELECT
		db_Mac
		FROM
		xy_userinfo AS a,
		xy_areabaseinfo AS b,
		xy_device_local_feature AS c,
		xy_areadeviceinfo AS d,
		xy_devicebaseinfo AS e
		WHERE
		a.u_phone = ''
		AND a.u_pwd = ''
		AND b.ab_number = a.u_id
		AND b.place_en_name = 'KT'
		AND b.idx = 0
		AND c.area_id = b.ab_id
		AND c.feature = 'LOCK'
		AND c.idx = 0 and c.type = 0
		AND b.ab_id = c.area_device_id
		AND e.db_ID = d.ad_deviceid*/

	string szMAc = "";
	string szGWMAc = "";
	std::stringstream szCharacters;
	szCharacters << "SELECT db_Mac from xy_userinfo AS a,xy_areabaseinfo AS b,xy_device_local_feature AS c,xy_areadeviceinfo AS d,xy_devicebaseinfo AS e WHERE a.u_phone = '";
	szCharacters << _Name << "' and a.u_pwd = '";
	szCharacters << _Psw << "' AND b.ab_number = a.u_id AND c.area_id = b.ab_id AND b.place_id = ";
	szCharacters << _Pos << " AND b.idx = ";
	szCharacters << _Pidx << " AND c.feature ='";
	szCharacters << DeviceFeatureName[_Fet] << "' AND c.idx = ";

	szCharacters << _Idx << " and c.type = ";
	szCharacters << _Type << " AND d.ad_id = c.area_device_id AND e.db_ID = d.ad_deviceid and d.ad_state = 1;";
	//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, (char*)szCharacters.str().c_str());
	MYSQL_RES *dzRes = g_ServerManager->g_IMysqlInterFace->QuerySql(szCharacters.str());
	MYSQL_ROW dzSqlrow;
	if (!dzRes)
	{
		mysql_free_result(dzRes);
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "Database stored procedure access error");
	}
	else
	{

		while ((dzSqlrow = mysql_fetch_row(dzRes)))
		{
			if (mysql_num_fields(dzRes) < 1)
			{
				continue;
			}
			szMAc = dzSqlrow[0];
		}
		mysql_free_result(dzRes);
	}
	if (szMAc != "")
	{
		std::stringstream szGwCharacters;
		szGwCharacters << "SELECT db_Mac from xy_devicebaseinfo where db_ID = (SELECT b.dc_GWId from xy_devicebaseinfo as a,xy_gwdevicecontrast as b where a.db_ID = b.dc_DeviceId  and b.dc_State = 1 and a.db_Mac = '" << szMAc << "');";
		
		//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, (char*)szGwCharacters.str().c_str());
		dzRes = g_ServerManager->g_IMysqlInterFace->QuerySql(szGwCharacters.str());
		
		if (!dzRes)
		{
			mysql_free_result(dzRes);
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "Database stored procedure access error");
		}
		else
		{

			while ((dzSqlrow = mysql_fetch_row(dzRes)))
			{
				if (mysql_num_fields(dzRes) < 1)
				{
					continue;
				}
				szGWMAc = dzSqlrow[0];
			}
			mysql_free_result(dzRes);
		}
		std::stringstream szSubCharacters;
		szSubCharacters<<"select d_uuid,d_sn,d_model,d_hw_version,d_fw_version from xy_dc_deviceinfo where  SUBSTR(HEX(d_mac), 5, 12) = '" << (szGWMAc == "" ? szMAc : szGWMAc) << "';";

		//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, (char*)szSubCharacters.str().c_str());
		dzRes = g_ServerManager->g_IMysqlInterFace->QuerySql(szSubCharacters.str());
	
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
				
				/*ostringstream szTr;
				szTr << setw(16) << setfill('0') << dzSqlrow[0];*/
				//szDeviceUuidObject->setDevUuid(szTr.str().c_str());

				string szHw = dzSqlrow[3];
				vector<string> szVersionTemp;
				g_ServerManager->g_ServiceManager.XSplit(szHw, ".", szVersionTemp);
				g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "HW[%d-%d-%d]", atoi(szVersionTemp[0].c_str()), atoi(szVersionTemp[1].c_str()), atoi(szVersionTemp[2].c_str()));
				
				uint16_t szHw_byte = MAKE_VER_556(atoi(szVersionTemp[0].c_str()), atoi(szVersionTemp[1].c_str()), atoi(szVersionTemp[2].c_str()));

				szVersionTemp.clear();
				g_ServerManager->g_ServiceManager.XSplit(dzSqlrow[4], ".", szVersionTemp);
				g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "FW[%d-%d-%d]", atoi(szVersionTemp[0].c_str()), atoi(szVersionTemp[1].c_str()), atoi(szVersionTemp[2].c_str()));
				uint16_t szFw_byte = MAKE_VER_556(atoi(szVersionTemp[0].c_str()), atoi(szVersionTemp[1].c_str()), atoi(szVersionTemp[2].c_str()));

				g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "HW[%d]  FW[%d]", szHw_byte, szFw_byte);
				//===================
				uint16_t szTemp = 0;
				UChar szDefine = 0x00;
				
				UChar szDefineCMD = CMD_GET_QUEUE_DEVICE;
				CDataObject *dzPayload = m_dzDataObjectList._malloc_boost();
				uint8_t szModelLen = strlen(dzSqlrow[2]);
				UChar szDefineLen = 3 + szModelLen + 6 + 4 +8 + 4 ;
				//长度
				*dzPayload << szDefineLen;
				*dzPayload << szDefineCMD;
				
				*dzPayload << szModelLen;
				//model
				dzPayload->write(dzSqlrow[2], szModelLen);

				//MAc
				unsigned char szMacByte[6] = { 0 };
				g_ServerManager->g_ServiceManager.HexStrToByte((szGWMAc == "" ? szMAc : szGWMAc).c_str(), szMacByte, 12);
				dzPayload->write((char*)szMacByte, 6);

				int32_t szSn =  htonl(atoi(dzSqlrow[1]));
				//sn
				*dzPayload << szSn;

				//UUID
				unsigned char szUUID[8] = { 0 };
				g_ServerManager->g_ServiceManager.HexStrToByte(dzSqlrow[0], szUUID, 16);
				dzPayload->write((char*)szUUID, 8);
				szHw_byte = htons(szHw_byte);
				//hw
				*dzPayload << szHw_byte;
				//fw
				szFw_byte = htons(szFw_byte);
				*dzPayload << szFw_byte;

				//加校验
				unsigned short szCrc = htons(g_ServerManager->g_ServiceManager.GetCrc16Ccitt(CRC_BASE_KEY, (unsigned char*)dzPayload->m_Data, dzPayload->m_DataLen));
				*dzPayload << szCrc;
				if (g_ServerManager->getCrypt())
					//加密
					g_ServerManager->g_DeviceUuidManager.UnCryptProtocol(_Topic.m_Mac.c_str(), (UChar*)dzPayload->m_Data, dzPayload->m_DataLen);

				unsigned char *szTempData = (unsigned char *)m_dzConvertBuffList._malloc_boost();
				RtlZeroMemory(szTempData, MAX_BUFF_NET_SIZE);
				size_t szTempDataLen = MAX_BUFF_NET_SIZE;
				stringstream szStreamTopic;
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
					return;
				}
#else
				
				szStreamTopic << "Nexless/v3/Cnf/SmartHome/"
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
					g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "MQTT Submission get deviceinfo Success[%s]", szStreamTopic.str().c_str());
				m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);
				m_dzDataObjectList._free_boost(dzPayload);
				
				
			}
			mysql_free_result(dzRes);
			return;
		}
	}

	UChar szDefineCMD = CMD_GET_QUEUE_DEVICE;
	CDataObject *dzPayload = m_dzDataObjectList._malloc_boost();
	UChar szDefineLen = 4;
	uint16_t szEr = 0;
	//长度
	*dzPayload << szDefineLen;
	*dzPayload << szDefineCMD;

	*dzPayload << szEr;

	//加校验
	unsigned short szCrc = htons(g_ServerManager->g_ServiceManager.GetCrc16Ccitt(CRC_BASE_KEY, (unsigned char*)dzPayload->m_Data, dzPayload->m_DataLen));
	*dzPayload << szCrc;
	if (g_ServerManager->getCrypt())
		//加密
		g_ServerManager->g_DeviceUuidManager.UnCryptProtocol(_Topic.m_Mac.c_str(), (UChar*)dzPayload->m_Data, dzPayload->m_DataLen);

	unsigned char *szTempData = (unsigned char *)m_dzConvertBuffList._malloc_boost();
	RtlZeroMemory(szTempData, MAX_BUFF_NET_SIZE);
	size_t szTempDataLen = MAX_BUFF_NET_SIZE;
	stringstream szStreamTopic;

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
		return;
	}
#else

	szStreamTopic << "Nexless/v3/Cnf/SmartHome/"
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
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "MQTT Submission get deviceinfo Success[%s]", szStreamTopic.str().c_str());
	m_dzConvertBuffList._free_boost((LPConvertBuffer)szTempData);
	m_dzDataObjectList._free_boost(dzPayload);

}
