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
	int32_t            InitSockets;			//��ʼ���������ӳش�С
	int32_t				BuffBlockSize;			//�������ݿ��С
	int32_t            BuffBlockNum;			//ÿ�����ӳ�ʼ�����ݻ�������
	int32_t				MaxPacketsPerSeconds;	//ÿ�������շ�������,Ϊ0ʱ��������
	int32_t            RSThreads;
	int32_t				HeartTest;               //��ʱ����s
	ITcpServerService* pServerService;		//����ص��ӿ�
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