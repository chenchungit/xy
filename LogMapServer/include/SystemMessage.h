
/*           _________           _________ 
*         / /________/        / /________/ 
*       / /                 / /     
*     / /                 / /
*    / /                 / /
*	/ /                  / /
*	/ /                  / /
*	 / /                  / /
*	  / /_________         / /_________
*	  / /________/         / /________/ 
* 
* mairycal@163.com
*/
/***************************************************************************
*功能:系统Tips输出
*时间:2016-04-30
*编写人:春哥mairycal@163.com
**************************************************************************/
#ifndef _SYSTEMMESSAGE_H_
#define _SYSTEMMESSAGE_H_
#include "_Mutex.h"
using namespace CC;
//日志接口

class CSystemMessage
{
public:
	CSystemMessage();
	~CSystemMessage();
	/*************************Parameter
	*初始化接口
	*************************/
	void InitLogInterFace(string _FileName);
	/*************************Parameter
	*系统输出
	*************************/
  virtual void  SystemCharacter(int32_t _type,char *_txt,...);
  /*************************Parameter
	*系统输出
	*************************/
  virtual void  SystemCharacter(char *_txt,...);



  virtual void setIoStream(int32_t _LogType) {
	  SystemCharacter(static_cast<char*>("setting log input success[%d]"), _LogType);
	  m_IoStream = _LogType;
  
  };
private:
	
	//
	ThreadMutex m_LogMutex;
	//日志
	ILogInterFace *g_ILogInterFace;

	//文件路径
	char m_SysPath[MAX_PATH + 1];

	//临时转换数据的内存池
	singletonPool_CC<PConvertBuffer> m_dzConvertBuffList;
	//是够流输出
	int32_t m_IoStream;
};

#endif