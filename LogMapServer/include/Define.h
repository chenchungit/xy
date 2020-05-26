#pragma once
#ifndef _DEFINE_H_
#define _DEFINE_H_

using namespace std;
struct ITopic
{
	string m_Company;
	string m_Ver;
	string m_Flag;
	string m_Name;
	string m_Mac;
	string m_Site;
	string m_Position;
	string m_PosIdx;
	string m_Feature;
	string m_Type;
	string m_Index;
};
typedef void(*ProcessFunc)(TcpSocketID id, unsigned char * _data, const int32_t _len, int32_t _Code);
typedef void(*ProcessFuncToken)(TcpSocketID id, TcpSocketID _Token, unsigned char * _data, const int32_t _len, int32_t _Code);
typedef void(*ProcessMQTTFunc)(unsigned char * _data, const int _len);
typedef void(*ProcessMqttTopicFunc)(std::string _Topic, unsigned char * _data, const int _len);
typedef void(*ProcessITopicFunc)(ITopic _Topic, unsigned char * _data, const int _len);


#define S_OK                                   (0L)
#define S_FALSE                                (1L)
#define ULONG unsigned long
#define UShort unsigned short 
#define UINT uint32_t
#define UINT64 uint64_t
typedef void *HANDLE;

#define DWORD unsigned long
#define UChar unsigned char

#define CRYPT_DATA



#define SD_OK 0
#define HUNDRED 100
#define SECOND_OF_DAY 86400
#define BASE_NET_BUFF_SIZE 1024
#define MAX_BUFF_NET_SIZE BASE_NET_BUFF_SIZE*10
#define MAX_BUFF_NETPAKAGESIZE BASE_NET_BUFF_SIZE*9
#define MAX_BUFF_LOG_SIZE BASE_NET_BUFF_SIZE*2
#define MAX_BUFF_NET_SINGLE_SIZE MAX_BUFF_NET_SIZE / 2	//分包发送时, 单个包的长度
#define CENTRESERVER_HEART_THREAD_TIME 1000 * 20
#define ERROR_CODE_INDEX 10000
#define IO_OUT_LOG 1
#define MQTT_TIMEOUT     10000L
#define TIMEOUT_REGISTER_FUN 20*1000
#define SERVER_TIMER_CORE 1000 * 90
#define MAX_PATH 261
#define TIMER_CORE 1 * 10
#define MAC_UCHAR_LENGTH 6//网关无符号整型长度
#define MAC_STRING_LENGTH 12//网关16进制长度
#define BANDLE_ID_LENGTH 16//bandle id



const UChar FUCK_VERSION_STATE_SUCCESS = 0;
const UChar FUCK_VERSION_STATE_FAIL = 1;
const UChar FUCK_VERSION_STATE_NOBUNDLE = 2;
//#define FUCK_VERSION_STATE_SUCCESS 0//版本操作成功
//#define FUCK_VERSION_STATE_FAIL 1//版本操作失败
//#define FUCK_VERSION_STATE_NOBUNDLE 2//没找到bundle id

#define _____FUCK____ARE__YOU___V1__________  0
//心跳包类型
#define SOCKET_HEARTPANAGE 1000
#define  CODE_ACTION_SHOW 1
//======================================down file=================================
#define SERVER_CURL_DOWNLOADURL "https://img.i.xeiot.com/apps/"
#define SERVER_VIANS_CURL_DOWNLOADURL "http://imgs.szvians.cn/apps/"
//============================================redis================================
#define SERVER_REDIS_KEY "xy-open-api:events_sync_redis_list"
//=========================================MQTT TOPIC===============================

#define VIANS        1
#ifdef VIANS
#define SERVER_TOPIC_ALARM "Vians/v3/Req/SmartHome/+/+/+/+/ALARM/+/+"
#define SERVER_TOPIC_LOCK "Vians/v3/Req/SmartHome/+/+/+/+/LOCK/+/+"
#define SERVER_TOPIC_GATEWAY "Vians/v3/Req/SmartHome/+/+/+/+/GATEWAY/+/+"
#define SERVER_TOPIC_DOORALARM "Vians/v3/Req/SmartHome/+/+/+/+/DOOR_ALARM/+/+"
#define SERVER_TOPIC_SMOKEALARM "Vians/v3/Req/SmartHome/+/+/+/+/SMOKE_ALARM/+/+"
#define SERVER_TOPIC_HUMANCNT "Vians/v3/Req/SmartHome/+/+/+/+/HUMAN_CNT/+/+"
#define SERVER_TOPIC_TEST "Vians/v3/Req/SmartHome/+/+/+/+/Version/+/+"
#define SERVER_TOPIC_GASALARM "Vians/v3/Req/SmartHome/+/+/+/+/GAS_ALARM/+/+"
#else
#define SERVER_TOPIC_ALARM "Nexless/v2/Req/SmartHome/+/+/+/+/ALARM/+/+"
#define SERVER_TOPIC_LOCK "Nexless/v2/Req/SmartHome/+/+/+/+/LOCK/+/+"
#define SERVER_TOPIC_GATEWAY "Nexless/v2/Req/SmartHome/+/+/+/+/GATEWAY/+/+"
#define SERVER_TOPIC_DOORALARM "Nexless/v2/Req/SmartHome/+/+/+/+/DOOR_ALARM/+/+"
#define SERVER_TOPIC_SMOKEALARM "Nexless/v2/Req/SmartHome/+/+/+/+/SMOKE_ALARM/+/+"
#define SERVER_TOPIC_HUMANCNT "Nexless/v2/Req/SmartHome/+/+/+/+/HUMAN_CNT/+/+"
#define SERVER_TOPIC_TEST "Nexless/v2/Req/SmartHome/+/+/+/+/Version/+/+"
#define SERVER_TOPIC_GASALARM "Nexless/v2/Req/SmartHome/+/+/+/+/GAS_ALARM/+/+"

#endif // VIANS


//=================================================================================

#define SERVER_TOPIC_UP "Nexless/v2/Upg/SmartHome/#"
#define SERVER_TOPIC_DOWN "Nexless/v2/Rpg/"
#define SERVER_TOPIC_RSP "Nexless/v2/Rsp/SmartHome/#"   //控制返回 
#define SERVER_TOPIC_REQ "Nexless/v2/Req/SmartHome/#"   //设备上报
#define TOPIC_SUB_VERSION "/Dft/Dft/0/GATEWAY/0/0"
#define  MQTT_TOPIC_HEARD "down/"
#define  MQTT_TOPIC_HARDWARE "down/Hardware"
//=========================================json===========================
#define ___FACK___JSON__JGRECORD "nexless.Queue.JgpushRecord"
#define ___FACK___JSON__ALARM "nexless.Queue.Alarm"
#define ___FACK___JSON__USERECORD "nexless.Queue.UseRecord"
#define ___FACK___JSON__VERSION "nexless.Version.UpLoad"
#define ___FACK___JSON__VWESION "1"
//=====================================redis==================
#define __FACK_REDIS_JG_ALARM__ "ALARM_LOG"
#define __FACK_REDIS_JG_NOMAL__ "NORMAL_LOG"
#define __FACK_REDIS_JG_UPGRADE__ "UPGRADE_START"
#define __FACK_REDIS_JG_UPGRADE__END "UPGRADE_DONE"

//===========================================提交类型=========================
#define SUBMIT_TYPE_JGPUSH 1
#define SUBMIT_TYPE_USERECORD 2
#define SUBMIT_TYPE_ALARMRECORD 3
//=========================================CRC========================
#define  CRC_BASE_KEY 0x5555

//====================================
#define TMALL_STATE_OK "S_OK"//控制指令不正确
#define TMALL_STATE_INVALIDATE_CONTROL_ORDER "INVALIDATE_CONTROL_ORDER"//控制指令不正确 
#define TMALL_STATE_SERVICE_ERROR "SERVICE_ERROR"//服务异常 
#define TMALL_STATE_DEVICE_NOT_SUPPORT_FUNCTION "DEVICE_NOT_SUPPORT_FUNCTION"//设备不支持该操作 
#define TMALL_STATE_INVALIDATE_PARAMS "INVALIDATE_PARAMS"//请求参数有误 
#define TMALL_STATE_DEVICE_IS_NOT_EXIST "DEVICE_IS_NOT_EXIST"//设备未找到 
#define TMALL_STATE_IOT_DEVICE_OFFLINE "IOT_DEVICE_OFFLINE"//设备离线状态 
#define TMALL_STATE_ACCESS_TOKEN_INVALIDATEK "ACCESS_TOKEN_INVALIDATE"//access_token 无效（包括失效） 



#define __FUCK___STR___(str,size) strlen(str) <= size ? strlen(str) : size
#define __FUCK_STR_LEN__(CurSize,MaxSize) CurSize <= MaxSize ? CurSize : MaxSize
#define RtlZeroMemory(_SrcMem,_Len)\
{\
    memset(_SrcMem,0,_Len);\
}
#define RtlCopyMemory(_DrcMem,_SrcMem,_len)\
{\
   memcpy(_DrcMem, _SrcMem, _len);\
}

#endif
