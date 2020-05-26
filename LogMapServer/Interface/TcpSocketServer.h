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
#ifndef __TCP_SOCKET_SERVER_INTERFACE_H__
#define __TCP_SOCKET_SERVER_INTERFACE_H__

//#include "Cmd_Head.h"

//////////////////////////////////////////////////////////////////////////

class ITcpServerService
{
public:
	virtual int32_t OnTcpServerStart() = 0;
	virtual int32_t OnTcpServerStop() = 0;
	virtual int32_t OnTcpSocketConnected(TcpSocketID id, DWORD dwIP) = 0;
	virtual int32_t OnTcpSocketClose(TcpSocketID id, DWORD dwIP) = 0;
	virtual int32_t OnTcpSocketRecived(TcpSocketID id, CMD_Head* head, const char* pData, int32_t DataLen) = 0;
	virtual int32_t OnTcpServerErrorMessage(TcpSocketID id,char* _EMessage) = 0;
	virtual int32_t OnTcpServerHeart(TcpSocketID id) = 0;
};

struct TcpServerInitItem
{
	unsigned short  port;
	int32_t            InitSockets;			//初始连接字连接池大小
	int32_t				BuffBlockSize;			//缓存数据库大小
	int32_t            BuffBlockNum;			//每个连接初始化数据缓存块个数
	int32_t				MaxPacketsPerSeconds;	//每秒限制收发包限制,为0时不做限制
	int32_t            RSThreads;
	int32_t				HeartTest;               //超时设置s
	ITcpServerService* pServerService;		//服务回调接口
};

class ITcpServer
{
public:
	virtual int32_t init(TcpServerInitItem* pInitData) = 0;
	virtual int32_t fini() = 0;
	virtual int32_t start() = 0;
	virtual int32_t stop() = 0;

	virtual int32_t CloseSocket(TcpSocketID sid, bool bNotify) = 0;
	virtual int32_t GetMaxConnection() = 0;
	virtual int32_t GetCurrentConnections() = 0;
	virtual int32_t GetOnlineConnections(TcpSocketID* idBuff, int32_t BufLen) = 0;

	virtual bool  GetAddress(TcpSocketID sid, Address& addr) = 0;

	virtual bool  AllowBatchSend(TcpSocketID sid, bool bAllow) = 0;

	virtual int32_t Send(TcpSocketID sid, CMD_Head head, char* pData, int32_t DataLen) = 0;
	virtual int32_t Send(TcpSocketID sid, CMD_Head head) = 0;
	virtual int32_t Send(TcpSocketID sid, char* pData, int32_t DataLen) = 0;
	virtual int32_t SendBatch(CMD_Head head, char* pData, int32_t DataLen) = 0;
	virtual int32_t SendBatch(CMD_Head head) = 0;

	virtual bool  IsOnLine(TcpSocketID sid) = 0;

	virtual void  Dump() = 0;
};
extern "C"
{
	 ITcpServer* CreateSocketServer();
	 void DeleteSocketServer(ITcpServer* pServer);
}


#endif//__TCP_SOCKET_SERVER_INTERFACE_H__