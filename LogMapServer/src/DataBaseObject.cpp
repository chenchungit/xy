#include "ServerHead.h"


void CDataBaseObject::McuEnCrypt(UChar* pucBuff, uint32_t wLen, UChar* pucKey)
{
	md5_context Ctx;
	UChar ucMd5[16];
	md5_init(&Ctx);
	md5_starts(&Ctx);
	md5_update(&Ctx, pucKey, 8);
	md5_finish(&Ctx, ucMd5);
	for (uint32_t wCnt = 0; wCnt < wLen; wCnt++) {
		UChar ucPos = wCnt & 0x0F;
		if (ucPos == 0) {
			md5_starts(&Ctx);
			md5_update(&Ctx, ucMd5, 16);
			md5_finish(&Ctx, ucMd5);
		}
		pucBuff[wCnt] ^= ucMd5[ucPos];
	}
}

void CDataBaseObject::SendMQTTProtocolData(int32_t _Type, ::google::protobuf::Message& _Protocol, string _Topic, const char * _Context)
{
	//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "\r\n\r\n\r\n\r\n");
	UChar* szData = NULL;
	uint32_t szLen = 0;
	szData = (UChar*)m_dzConvertBuffList._malloc_boost();
	memcpy(szData, &_Type, sizeof(int32_t));
	if (g_ServerManager->getIAnalysisPacket()->SerializeData(szData + sizeof(int32_t), szLen, &_Protocol) == PROTOCOL_TYPE_OK)
	{
		//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "Publish   [len = %d][msg len = %d]", strlen((char*)szData), szLen+ sizeof(int32_t));
		if (!g_ServerManager->g_MqttManager.Publish(_Topic, (char*)szData, szLen + sizeof(int32_t)))
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "MQTT Submission [%s] failure", _Context);

	}
	else
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "Submission failure, serialization [%s]", _Context);
	m_dzConvertBuffList._free_boost((LPConvertBuffer)szData);

	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "============================[Topic = %s] [Code = %d] [%s]==============================\r\n",
		_Topic.c_str(), _Type, _Context);
}

void CDataBaseObject::SendProtocolData(TcpSocketID id,  ::google::protobuf::Message& _Protocol, int32_t _Cmd, const char * _Context)
{
	UChar* szData = NULL;
	uint32_t szLen = 0;
	szData = (UChar*)m_dzConvertBuffList._malloc_boost();
	if (g_ServerManager->getIAnalysisPacket()->SerializeData(szData, szLen, &_Protocol) == PROTOCOL_TYPE_OK)
	{
		
		if (g_ServerManager->m_SocketService->TrsportTokenMessage(id, id, _Cmd, (char*)szData,
			szLen) == 0)
		{
			//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "Send %s Success", _Context);
		}
		else
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "Send %s Fail", _Context);

	}
	else
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "Send %s Fail，Serialize", _Context);
	m_dzConvertBuffList._free_boost((LPConvertBuffer)szData);
}

int  CDataBaseObject::OnXCoreErrorMessage(char* _EMessage)
{
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_ERROR, _EMessage);
	return true;
}


char *  CDataBaseObject::SpecialCharacters(char * _OutChar, char* _Characters, ...)
{

	va_list marker;
	va_start(marker, _Characters);
	int szContextSize = vsnprintf(_OutChar, MAX_BUFF_LOG_SIZE, _Characters, marker);
	va_end(marker);

	/*std::string strTemp(_OutChar);
	Replase(strTemp, "%", "\%");
	RtlZeroMemory(_OutChar, MAX_BUFF_LOG_SIZE);
	RtlCopyMemory(_OutChar, strTemp.c_str(), strTemp.length());*/
	return _OutChar;
}
template<class out_type, class in_value>
out_type CDataBaseObject::Convert(const in_value & t)
{
	stringstream stream;
	stream << t;//向流中传值
	out_type result;//这里存储转换结果
	stream >> result;//向result中写入值
	return result;
}
