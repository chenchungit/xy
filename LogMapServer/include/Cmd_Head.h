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
#ifndef __FANS_BASE_CMD_HEAD_H__
#define __FANS_BASE_CMD_HEAD_H__



//检测结构信息
struct CMD_KN_DetectSocket
{
	uint64_t								dwSendTickCount;					//发送时间
	uint64_t								dwRecvTickCount;					//接收时间
};

struct CMD_Head 
{
	uint16_t len;
	uint8_t  MajorID;
	uint8_t  MinorID;
};

const int32_t PACKET_HEAD_SIZE = sizeof(CMD_Head);

//////////////////////////////////////////////////////////////////////////

struct TcpSocketID
{
	union
	{
		struct  
		{
			 uint32_t ChannelID;
			 uint32_t HandlerID;
		};
		uint64_t key;
	};
	bool operator == (TcpSocketID id) const 
	{
		return (id.key == key);
	}
	bool operator != (TcpSocketID id) const 
	{
		return id.key != key;
	}
	
};

const TcpSocketID INVALID_TCP_SOCKETID = { 0xffffffff, 0xffffffff };

//////////////////////////////////////////////////////////////////////////
struct Address 
{
	union
	{
		struct{
			uint32_t ip;
			uint16_t  port;
		};
		uint64_t key;
	};

	Address():ip(0xFFFFFFFF),port(0xFFFF){}
	bool operator == (Address addr) const {
		return (key == addr.key);
	}

	bool operator != (Address addr) const {
		return (key != addr.key);
	}

	bool operator >  (Address addr) const {
		return (key > addr.key);
	}

	bool operator <  (Address addr) const {
		return (key < addr.key);
	}

	operator uint64_t (void) const {
		return key;
	}
};

const Address INVALID_ADDR;

#endif//__FANS_BASE_CMD_HEAD_H__