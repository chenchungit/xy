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
/************************************************************************
*该类是逻辑服务器的所有网络来的所有数据的临时缓存并分析塞选，分发做处理
*时间：2014-3-5
*mairycal@163.com
************************************************************************/

#ifndef _ANALSYSNETDATA_H_
#define _ANALSYSNETDATA_H_
#pragma once

#include <queue>
#include "_Mutex.h"
using namespace std;
using namespace CC;

class CAnalsysNetData :public XCode
{
public:
public:
	CAnalsysNetData();
	~CAnalsysNetData();
public:
	//接收网络未处理的数据
	bool InsertDataQueue(TcpSocketID id, UChar * _data, const int _len);
	bool InsertMQTTDataQueue(string _Topic,UChar * _data, const int _len);
	//未处理的数据出栈
	int PopAnalSysDataQueue(list<LPClient> & _DataList);
	int PopMQTTAnalSysDataQueue(list<LPClient> & _DataList);
	//结构化字符串复制
	bool _memcpy_Array(void * _drc, void *_src, int _len);

	static void* AnsysThreadFuntion(void* para);
private:
	queue<LPClient> m_dzNotAnalSysData;
	queue<LPClient> m_dzMQTTAnalSysData;
	ThreadMutex PoolMutex_AnsysFromClient;
	ThreadMutex PoolMutex_AnsysFromMQTT;

	pthread_t m_ThreadID;

	//内存池
	singletonPool_CC<PClient> m_dzNetNotAnalsysDataList;

	singletonPool_CC<PClient> m_dzMQTTAnalsysDataList;
	//objectPool_CC<PConvertBuffer> m_dzConvertBuffList;

public:
};
#endif