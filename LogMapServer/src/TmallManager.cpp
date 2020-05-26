#include "ServerHead.h"

string CTmallManager::PublishTmallCmd(string _ControlType, uint64_t _DeviceID, string _DeviceMac, uint32_t _DeviceType)
{
	CTmallObject szTmallObject;
	string szFeature = getFeature(_DeviceType);
	if (szFeature.length() == 0)
		return TMALL_STATE_INVALIDATE_PARAMS;
	if (!QueueTmall(_DeviceID, szFeature, szTmallObject))
		return TMALL_STATE_DEVICE_IS_NOT_EXIST;

	if (Publish(szTmallObject, _DeviceType, _ControlType))
	{
		return TMALL_STATE_OK;
	}

	return TMALL_STATE_SERVICE_ERROR;
}
string CTmallManager::getFeature(uint32_t _DeviceType)
{
	//判断特性
	switch (_DeviceType)
	{
		case SENSOR:
		case ALARM:
		{
			return "ALARM";
			break;
		}
		case CURTAIN:
		{
			return "CURTAIN";
			break;
		}
		case SOCKET:
		{
			return "SOCKET";
			break;
		}
		case SWITCH1:
		case SWITCH2:
		case SWITCH3:
		case REMOTE_SWITCH1:
		case REMOTE_SWITCH2:
		case REMOTE_SWITCH3:
		{
			return "SWITCH";
			break;
		}
		case LAMP:
		case COLOR_LAMP:
		case LDP:
		{
			return "LIGHTING";
			break;
		}
		case MJ_LOCK:
		case CORE_LOCK:
		case DOOR_LOCK:
		case HAND_LOCK:
		case PARKING_LOCK:
		case CABINET_LOCK:
		case U_LOCK:
		case LUGGAGE_LOCK:
		{
			return "LOCK";
			break;
		}
	}
	return "";
}
bool CTmallManager::QueueTmall(uint64_t _DeviceID, string _feature, CTmallObject &_TmallObject)
{
	MYSQL_RES *dzRes;
	MYSQL_ROW dzSqlrow;
	stringstream dzSql;
	dzSql << "SELECT d.db_Mac,b.place_en_name,b.idx as PosIdx,c.type,c.idx,c.feature from xy_areadeviceinfo as a,xy_areabaseinfo as b ,xy_device_local_feature as c";
	dzSql << ",xy_devicebaseinfo as d where a.ad_id = " << _DeviceID;
	dzSql << " and a.ad_areaid = b.ab_id and a.ad_state = 1 and c.area_device_id = a.ad_id and c.feature != 'POWER_MEASURE' AND c.feature != 'UPDATE_MCU'";
	//dzSql << _feature.c_str();
	dzSql << " and d.db_ID = (SELECT b.dc_GWId from xy_devicebaseinfo as a,xy_gwdevicecontrast as b,xy_areadeviceinfo as c where c.ad_id =";
	dzSql << _DeviceID;
	dzSql << " and c.ad_deviceid = a.db_ID and b.dc_DeviceId = a.db_ID and b.dc_State = 1 );";
	
	dzRes = g_ServerManager->g_IMysqlInterFace->QuerySql(dzSql.str());
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, " QueueTmall[Mac=%s]", dzSql.str().c_str());
	

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
			if (mysql_num_fields(dzRes) < 2)
			{
				continue;
			}
			_TmallObject.setGwMac(dzSqlrow[0]);
			_TmallObject.setPosition(dzSqlrow[1]);
			_TmallObject.setPosIdx(dzSqlrow[2]);
			_TmallObject.setType(dzSqlrow[3]);
			_TmallObject.setIndex(dzSqlrow[4]);
			_TmallObject.setFeature(dzSqlrow[5]);
			
		}
		mysql_free_result(dzRes);
	}

	return _TmallObject.getGwMac().length() > 0;
}

bool CTmallManager::Publish(CTmallObject &_TmallObject, uint32_t _DeviceType, string _ControlType)
{
	stringstream szStreamTopic;
	CDataObject *dzPayload = m_dzDataObjectList._malloc_boost();
	UChar szDefineLen = 0;
	UChar szDefineCMD = 0;

	string szFeature;

	switch (_DeviceType)
	{
	case SENSOR:
	case ALARM:
	{
		
		break;
	}
	case CURTAIN:
	{
		szDefineLen = 0x02;
		if (_ControlType == "TurnOn")
			szDefineCMD = 0x01;
		if (_ControlType == "TurnOff")
			szDefineCMD = 0x00;
		if (_ControlType == "Pause")
			szDefineCMD = 0x02;

		*dzPayload << szDefineLen;
		*dzPayload << szDefineCMD;
		szFeature = "CURTAIN";
		break;
	}
	case SOCKET:
	{
		szDefineLen = 0x02;
		if (_ControlType == "TurnOn")
			szDefineCMD = 0x01;
		if (_ControlType == "TurnOff")
			szDefineCMD = 0x00;

		*dzPayload << szDefineLen;
		*dzPayload << szDefineCMD;
		szFeature = "SOCKET";
		//return "SWITCH";
		break;
	}
	case SWITCH1:
	case SWITCH2:
	case SWITCH3:
	case REMOTE_SWITCH1:
	case REMOTE_SWITCH2:
	case REMOTE_SWITCH3:
	{
		szDefineLen = 0x02;
		if (_ControlType == "TurnOn")
			szDefineCMD = 0x01;
		if (_ControlType == "TurnOff")
			szDefineCMD = 0x00;

		*dzPayload << szDefineLen;
		*dzPayload << szDefineCMD;
		szFeature = "SWITCH";
		//return "SWITCH";
		break;
	}
	case LAMP:
	case COLOR_LAMP:
	case LDP:
	{
		szDefineLen = 0x02;
		if (_ControlType == "TurnOn")
			szDefineCMD = 0x01;
		if (_ControlType == "TurnOff")
			szDefineCMD = 0x00;

		*dzPayload << szDefineLen;
		*dzPayload << szDefineCMD;
		szFeature = "LIGHTING";
		break;
	}
	case MJ_LOCK:
	case CORE_LOCK:
	case DOOR_LOCK:
	case HAND_LOCK:
	case PARKING_LOCK:
	case CABINET_LOCK:
	case U_LOCK:
	case LUGGAGE_LOCK:
	{
		szDefineLen = 0x0A;
		szDefineCMD = 0x00;

		*dzPayload << szDefineLen;
		*dzPayload << szDefineCMD;
		*dzPayload << 0x00 << 0x00 << 0x00 << 0x00 << 0x00 << 0x00 << 0x00 << 0x00;
		szFeature = "LOCK";
		break;
	}
	}
	
	if (szDefineLen == 0x00)
	{
		return false;
	}
	//加校验
	unsigned short szCrc = htons(g_ServerManager->g_ServiceManager.GetCrc16Ccitt(CRC_BASE_KEY, (unsigned char*)dzPayload->m_Data, dzPayload->m_DataLen));
	*dzPayload << szCrc;
	if (g_ServerManager->getCrypt())
		//加密
		g_ServerManager->g_DeviceUuidManager.UnCryptProtocol(_TmallObject.getGwMac().c_str(), (UChar*)dzPayload->m_Data, dzPayload->m_DataLen);

	//Nexless/v2/Ind/SmartHome/DC2C2800EBF4/Dft/Dft/0/LIGHTING/0/0
	//sleep(2);
	szStreamTopic << "Nexless/v2/Ind/SmartHome/"
		<< _TmallObject.getGwMac() << "/Dft"
		<< "/" << _TmallObject.getPosition() << "/"
		<< _TmallObject.getPosIdx() << "/" << _TmallObject.getFeature()
		<< "/" << _TmallObject.getType() << "/" << _TmallObject.getIndex();   //Mac/Site/Position/PosIdx/GATEWAY/Type/Index";
	if (!g_ServerManager->g_MqttManager.Publish(szStreamTopic.str(), (char*)dzPayload->m_Data, dzPayload->m_DataLen))
	{
		m_dzDataObjectList._free_boost(dzPayload);
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "MQTT Submission [%s] failure", "version updata fail");
		return false;
	}
	else
	{
		m_dzDataObjectList._free_boost(dzPayload);
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "MQTT Submission version updata Success[%s]", szStreamTopic.str().c_str());
		return true;
	}
	
}