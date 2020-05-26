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
//��ʱ��
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
#define MAX_BUFF_NET_SINGLE_SIZE MAX_BUFF_NET_SIZE / 2	//�ְ�����ʱ, �������ĳ���
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
//����������
typedef enum _ServerType
{
	YD_CONNECT_UNKNOW = 0,//δ֪
	YD_CONNECT_DB = 1,//���ݿ�����
	YD_CONNECT_ALLOWBATCH = 2,//Ⱥ��������
	YD_CONNECT_NETSERVER = 3,//���ط�����
	YD_CONNECT_LOG = 4,//��־������
	YD_CONNECT_LOGIC = 5,//�߼�������
	YD_CONNECT_COORD = 6,//Э��������
	YD_CONNECT_DATALOGIC = 7,//���ݷ�����
	YD_CONNECT_RPC = 8,//RPC������
}ServerType;
/////
//����������
typedef enum _DBType
{
	DB_STATE_UNKOWN = 0,//��
	DB_STATE_ADD = 1,//����
	DB_STATE_UPDATE = 2,//����
	DB_STATE_DELETE = 3,//ɾ��
}DBType;

//�˻�״̬
typedef enum _DataState
{
	DATA_STATE_DEL = 0,//ɾ��
	DATA_STATE_QIYONG = 1,//����
	DATA_STATE_TINGYONG = 2,//ͣ��
	DATA_STATE_APPLOGIN = 3,//app��½
	DATA_STATE_NETLOGIN = 4,//��̨
	DATA_STATE_BAOFEI = 5,//����
}DataState;
//�洢���̲�������
typedef enum _DBUseType
{
	DB_USE_UNKOWN = 0,
	DB_USE_QUEUE = 1,//��ѯ
	DB_USE_ADD = 2,//����
	DB_USE_UPDATE = 3,//����
	DB_USE_DELETE = 4,//ɾ��
}DBUseType;
//д����־����
typedef enum _LogType
{
	LOG_WRITE_IO = 1,//�����IO
	LOG_WRITE_FILE = 2,//������ļ�
	LOG_WRITE_ERROR = 4,//������־
	LOG_WRITE_SYSTEM = 8,//ϵͳ��־
	LOG_WRITE_GENERAL = 16,//һ����־
	LOG_WRITE_BOTH = 32,//��Ҫд��־��Ҫ���
}LogType;
typedef enum _NetStart
{
	NET_START_ON = 0,//��ʼ����
	NET_START_OFF = 1,//���ӹر�
	NET_START_REON = 2,//����
}NetStart;
//������״̬
typedef enum _ServiceState
{
	YD_SERVICE_STATE_ON = 1,//��Ծ״̬
	YD_SERVICE_STATE_OFF = 2,//����״̬
	YD_SERVICE_STATE_UNKNOW = 3,//δ֪״̬
	YD_SERVICE_STATE_UNREGISTOR = 4,//δע��ֻ������
}ServiceState;
//����������
typedef enum _ReturnValue
{
	YD_RETURN_OK = 0,//�ɹ�
	YD_RETURN_EXP = 1,//�쳣
	YD_RETURN_FAIL = 2,//ʧ��
}ReturnValue;
//���ݷ��Ͷ���
typedef enum  _ServiceType
{
	SERVICE_SEND_APP = 1, //���͸�app
	SERVICE_SEND_SERVER = 2, //���͸�server
}ServiceType;


enum ProtocolType {
	PROTOCOL_TYPE_OK = 1,
	PROTOCOL_TYPE_EXCEPTION = 2,
	PROTOCOL_TYPE_ISFULL = 3,
	PROTOCOL_TYPE_UNKNOW = 4
};

//������������
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
////��ʱ����ת��
typedef struct ConvertBuffer
{
public:
	ConvertBuffer() {
		RtlZeroMemory(dzPSocketBuff, MAX_BUFF_NET_SIZE);
	};
	unsigned char dzPSocketBuff[MAX_BUFF_NET_SIZE];
}*LPConvertBuffer, PConvertBuffer;




//����������
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
