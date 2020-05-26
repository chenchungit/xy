
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
*����:ϵͳTips���
*ʱ��:2016-04-30
*��д��:����mairycal@163.com
**************************************************************************/
#ifndef _SYSTEMMESSAGE_H_
#define _SYSTEMMESSAGE_H_
#include "_Mutex.h"
using namespace CC;
//��־�ӿ�

class CSystemMessage
{
public:
	CSystemMessage();
	~CSystemMessage();
	/*************************Parameter
	*��ʼ���ӿ�
	*************************/
	void InitLogInterFace(string _FileName);
	/*************************Parameter
	*ϵͳ���
	*************************/
  virtual void  SystemCharacter(int32_t _type,char *_txt,...);
  /*************************Parameter
	*ϵͳ���
	*************************/
  virtual void  SystemCharacter(char *_txt,...);



  virtual void setIoStream(int32_t _LogType) {
	  SystemCharacter(static_cast<char*>("setting log input success[%d]"), _LogType);
	  m_IoStream = _LogType;
  
  };
private:
	
	//
	ThreadMutex m_LogMutex;
	//��־
	ILogInterFace *g_ILogInterFace;

	//�ļ�·��
	char m_SysPath[MAX_PATH + 1];

	//��ʱת�����ݵ��ڴ��
	singletonPool_CC<PConvertBuffer> m_dzConvertBuffList;
	//�ǹ������
	int32_t m_IoStream;
};

#endif