#ifndef _LOGINTERFACE_H_
#define _LOGINTERFACE_H_
#pragma once

class ILogInterFace
{
public:
	virtual void  InitLog(char* _FileName,char* _FilePath,char* _subPath) = 0;
	virtual bool  WaitingLocal_IO_FILE(int32_t _Type,char *_txt,...) = 0;
	virtual bool  WaitingLocal_Context_FILE(int32_t _Type, char *_txt, int32_t _len) = 0;

};

extern "C"
{
	ILogInterFace * CreateILogInterFace();
	 void DeleteILogInterFace(ILogInterFace * pLog);
}
#endif