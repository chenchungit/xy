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
#include <iomanip>
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


#include "Define.h"
#include "StructDefine.h"
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

//============================加密===========================
#include "polarssl/md5.h"
#include "polarssl/base64.h"


//=============================内存管理======================
#include "DataObject.h"
#include "PoolObject.h"

//==========================接口======================
#include "LogInterface.h"
#include "IMysqlInterface.h"
#include "IAnalysisPacket.h"


//==========================协议=======================
#include "ServerProtocol.pb.h"
#include "DeviceVersion.pb.h"
//==========================MQTT=======================
#include "MQTTClient.h"
//=======================redis==================
#include <hiredis/hiredis.h>
//=========================curl=======================
#include <curl/curl.h>

using namespace ServerProtocol;
using namespace MySqlpool;
using namespace NetProtocol;
#include "GroupPackageEngine.h"

#include "ServerManager.h"

extern CServerManager *g_ServerManager;
extern bool g_ThreadRun;
extern char ErrorChinaContext[][128];
extern char ErrorEnglishContext[][128];
extern char SysTemContext[][128];
extern char TableName[][128];
extern char DeviceFeatureName[][128];

#endif
