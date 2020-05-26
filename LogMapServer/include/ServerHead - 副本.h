/*            _________           _________
*          / /________/        / /________/
*        / /                 / /
*      / /                 / /
*    / /                 / /
*	/ /                  / /
*	/ /                  / /
*	 / /                  / /
*	  / / _________         / / _________
*	   / /________/          / /________/
*
* mairycal@163.com
*/
#ifndef _SERVERHEAD_H_
#define _SERVERHEAD_H_
#pragma once
#include <stdio.h>
#include "message.h"
#include <sstream>
#include <time.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "Cmd_Head.h"

#ifdef _WIN32
#include <winsock2.h>
#include <time.h>
#else
#include <sys/time.h>
#include <fstream>
#include<unistd.h>
#include <iconv.h>  
#include "math.h"
#include <uuid/uuid.h>
//定时器
#include <unistd.h>
#include <signal.h>
#include <future>
//hashmap
#include<algorithm>
#include <ext/hash_map>
#endif

using namespace __gnu_cxx;
using namespace std;


#include <mysql/mysql.h>
#include <boost/shared_ptr.hpp>

#define S_OK                                   (0L)
#define S_FALSE                                (1L)
#define ULONG unsigned long
#define UShort unsigned short 
#define UINT uint32_t
typedef void *HANDLE;
#define MAX_PATH 261
#define DWORD unsigned long
#define UChar unsigned char

typedef void(*ProcessFunc)(TcpSocketID id, unsigned char * _data, const int32_t _len, int32_t _Code);
typedef void(*ProcessFuncToken)(TcpSocketID id, TcpSocketID _Token, unsigned char * _data, const int32_t _len, int32_t _Code);
typedef void(*ProcessMQTTFunc)(unsigned char * _data, const int _len);




#define SD_OK 0
#define HUNDRED 100
#define SECOND_OF_DAY 86400
#define MAX_BUFF_NET_SIZE 1024*10
#define MAX_BUFF_NETPAKAGESIZE 1024*9
#define MAX_BUFF_LOG_SIZE 2048
#define MAX_BUFF_NET_SINGLE_SIZE MAX_BUFF_NET_SIZE / 2	//分包发送时, 单个包的长度
#define CENTRESERVER_HEART_THREAD_TIME 1000 * 20
#define ERROR_CODE_INDEX 10000
#define IO_OUT_LOG 1
#define MQTT_TIMEOUT     10000L

#define TIMEOUT_REGISTER_FUN 20*1000

#define TIMER_CORE 1 * 10
#define SERVER_TIMER_CORE 1000 * 90

#define __FUCK___STR___(str,size) strlen(str) <= size ? strlen(str) : size
#define __FUCK_STR_LEN__(CurSize,MaxSize) CurSize <= MaxSize ? CurSize : MaxSize

#define CUChar unsigned char
using namespace std;

#define _____FUCK____ARE__YOU___V1__________  0
#define SERVER_TOPIC_UP "Nexless/v1/up/#"
#define SERVER_TOPIC_DOWN "Nexless/v1/down/"
#define TOPIC_SUB_VERSION "version"

#define  MQTT_TOPIC_HEARD "down/"
#define  MQTT_TOPIC_HARDWARE "down/Hardware"
//=========================================json
#define ___FACK___JSON__JGRECORD "nexless.Queue.JgpushRecord"
#define ___FACK___JSON__VWESION "1"

/////
typedef enum _MqttQOS
{
	QOS_0 = 0,
	QOS_1 = 1,
	QOS_2 = 2,
}MqttQOS;

//using namespace ServerProtocol;
//using namespace NetProtocol;
//using namespace MySqlpool;
//服务器类型
typedef enum _ServerType
{
	YD_CONNECT_UNKNOW = 0,//未知
	YD_CONNECT_DB = 1,//数据库连接
	YD_CONNECT_ALLOWBATCH = 2,//群发服务器
	YD_CONNECT_NETSERVER = 3,//网关服务器
	YD_CONNECT_LOG = 4,//日志服务器
	YD_CONNECT_LOGIC = 5,//逻辑服务器
	YD_CONNECT_COORD = 6,//协调服务器
	YD_CONNECT_DATALOGIC = 7,//数据服务器
	YD_CONNECT_RPC = 8,//RPC服务器
}ServerType;
/////
//服务器类型
typedef enum _DBType
{
	DB_STATE_UNKOWN = 0,//无
	DB_STATE_ADD = 1,//增加
	DB_STATE_UPDATE = 2,//更新
	DB_STATE_DELETE = 3,//删除
}DBType;

//账户状态
typedef enum _DataState
{
	DATA_STATE_DEL = 0,//删除
	DATA_STATE_QIYONG = 1,//正常
	DATA_STATE_TINGYONG = 2,//停用
	DATA_STATE_APPLOGIN = 3,//app登陆
	DATA_STATE_NETLOGIN = 4,//后台
	DATA_STATE_BAOFEI = 5,//报废
}DataState;
//存储过程操作类型
typedef enum _DBUseType
{
	DB_USE_UNKOWN = 0,
	DB_USE_QUEUE = 1,//查询
	DB_USE_ADD = 2,//新增
	DB_USE_UPDATE = 3,//更新
	DB_USE_DELETE = 4,//删除
}DBUseType;
//写入日志类型
typedef enum _LogType
{
	LOG_WRITE_IO = 1,//输出到IO
	LOG_WRITE_FILE = 2,//输出到文件
	LOG_WRITE_ERROR = 4,//错误日志
	LOG_WRITE_SYSTEM = 8,//系统日志
	LOG_WRITE_GENERAL = 16,//一般日志
	LOG_WRITE_BOTH = 32,//既要写日志又要输出
}LogType;
typedef enum _NetStart
{
	NET_START_ON = 0,//初始连接
	NET_START_OFF = 1,//连接关闭
	NET_START_REON = 2,//重连
}NetStart;
//服务器状态
typedef enum _ServiceState
{
	YD_SERVICE_STATE_ON = 1,//活跃状态
	YD_SERVICE_STATE_OFF = 2,//待机状态
	YD_SERVICE_STATE_UNKNOW = 3,//未知状态
	YD_SERVICE_STATE_UNREGISTOR = 4,//未注册只是连接
}ServiceState;
//服务器类型
typedef enum _ReturnValue
{
	YD_RETURN_OK = 0,//成功
	YD_RETURN_EXP = 1,//异常
	YD_RETURN_FAIL = 2,//失败
}ReturnValue;
//数据发送对象
typedef enum  _ServiceType
{
	SERVICE_SEND_APP = 1, //发送给app
	SERVICE_SEND_SERVER = 2, //发送给server
}ServiceType;


enum ProtocolType {
	PROTOCOL_TYPE_OK = 1,
	PROTOCOL_TYPE_EXCEPTION = 2,
	PROTOCOL_TYPE_ISFULL = 3,
	PROTOCOL_TYPE_UNKNOW = 4
};

//网络来的数据
typedef struct _ClientData
{
public:
	_ClientData() {
		memset(dzPSocketBuff, 0, MAX_BUFF_NET_SIZE);
		dzlen = 0;
		dzSocketID.key = 0L;
	};
	UChar dzPSocketBuff[MAX_BUFF_NET_SIZE];
	int32_t dzlen;
	TcpSocketID dzSocketID;
}*LPClient, PClient;

#define RtlZeroMemory(_SrcMem,_Len)\
{\
    memset(_SrcMem,0,_Len);\
}
#define RtlCopyMemory(_DrcMem,_SrcMem,_len)\
{\
   memcpy(_DrcMem, _SrcMem, _len);\
}
////临时数据转换
typedef struct ConvertBuffer
{
public:
	ConvertBuffer() {
		RtlZeroMemory(dzPSocketBuff, MAX_BUFF_NET_SIZE);
	};
	unsigned char dzPSocketBuff[MAX_BUFF_NET_SIZE];
}*LPConvertBuffer, PConvertBuffer;




//心跳包类型
#define SOCKET_HEARTPANAGE 1000
#define  CODE_ACTION_SHOW 1

//#include "json.h"
#include <string>
#include <unordered_map>
#include <functional>
#include "mongoose.h"

#include<json/json.h>
#include "_Mutex.h"
#include "XCode.h"
#include "Event.h"
#include "EventManager.h"


#include "DataObject.h"
#include "PoolObject.h"



#include "LogInterface.h"
#include "IMysqlInterface.h"
#include "IAnalysisPacket.h"

#include "ServerProtocol.pb.h"

using namespace ServerProtocol;
using namespace MySqlpool;

#include "GroupPackageEngine.h"

#include "ServerManager.h"

extern CServerManager *g_ServerManager;
extern bool g_ThreadRun;
extern char ErrorChinaContext[][128];
extern char ErrorEnglishContext[][128];
extern char SysTemContext[][128];
extern char TableName[][128];

#endif
