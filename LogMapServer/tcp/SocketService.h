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
#ifndef SOCKETSERVICE_H
#define SOCKETSERVICE_H
#pragma once
#include <list>
#include <queue>
#include <stack>
#include "_Mutex.h"
#include "TcpSocketServer.h"

using namespace CC;
using namespace std;
struct _DataString
{
    char _dzStr[1024];
};
class CSocketService : public ITcpServerService,public XCode,public CGroupPackageEngine
{
public:
        CSocketService();
        ~CSocketService();
public:
	void Init(TcpServerInitItem *_TcpServerInitItem);
	void Fini();
	void Start();
	void Stop();
	void CloseSocket(TcpSocketID sid);
	bool IsOnLine(TcpSocketID sid);
	virtual int32_t  OnTcpServerStart();
	virtual int32_t  OnTcpServerStop();
	virtual int32_t  OnTcpSocketConnected(TcpSocketID id, DWORD dwIP);
	virtual int32_t  OnTcpSocketClose(TcpSocketID id, DWORD dwIP);
	virtual int32_t  OnTcpSocketRecived(TcpSocketID id, CMD_Head* head, const char* pData, int32_t DataLen);
	virtual int32_t  OnTcpServerErrorMessage(TcpSocketID id,char* _EMessage);
	virtual int32_t  OnTcpServerHeart(TcpSocketID id);
	//static void* ThreadFuntion(void * Param);

      int32_t SendToClient(TcpSocketID id,const char* pData, int32_t DataLen);
	  //_iType:1发送给GM,2发送给其他服务器
	  int32_t SendToClient(const char* pData, int32_t DataLen,TcpSocketID id,int32_t _iType);

private:
	ULONG _count;
public:
	ITcpServer* m_pSocket;
    UShort m_pSocketPort;
	char m_dzComvert[MAX_PATH];
};
#endif