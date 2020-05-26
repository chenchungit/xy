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
#ifndef _SENDDATAOBJECT_H_
#define _SENDDATAOBJECT_H_
#pragma once
#include <iostream>
using namespace std;
using namespace CC;
class CGroupPackageEngine
{
public:
   CGroupPackageEngine();
  virtual ~CGroupPackageEngine();
public:
	/****************************
	*组包数据
	*
	***************************/
	virtual int32_t TrsportMessage(int32_t iType,UChar *dzData,int32_t dzLen,const TcpSocketID dzDrcChannelID,int32_t _iType = SERVICE_SEND_SERVER);
	
	/****************************
	*协议序列化发送处理
	***************************/
	virtual int32_t TrsportMessage(int32_t _iType,const TcpSocketID _SocketID,void * _SrcData);

	/****************************
	*协议序列化发送处理
	***************************/
	virtual int32_t TrsportMessage(DWORD _handle,int32_t _iType,const TcpSocketID _SocketID,void * _SrcData);

	/****************************
	*发送数据包
	***************************/
	virtual int32_t TrsportTokenMessage(const TcpSocketID _SocketID,TcpSocketID _Token,int32_t iType,const char * _SrcData,int32_t _Len);
	/****************************
	*发送数据包
	***************************/
	virtual int32_t TrsportMessage(const TcpSocketID _SocketID,const char * _SrcData,int32_t _Len);

private:
	objectPool_CC<PConvertBuffer> m_dzConvertBuffList;
	objectPool_CC<CDataObject> m_dzDataObjectList;
};
#endif