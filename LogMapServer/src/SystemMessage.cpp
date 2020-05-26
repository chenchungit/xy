#include "ServerHead.h"

CSystemMessage::CSystemMessage()
{
	/*GetModuleFileName(NULL, m_SysPath, MAX_PATH);
	(_tcsrchr(m_SysPath, ('\\')))[1] = 0;
	pIUnknown = CreateILogInterFace();*/
	m_IoStream = LOG_WRITE_IO;
}
CSystemMessage::~CSystemMessage()
{

}
/*************************Parameter
*初始化接口
*************************/
void CSystemMessage::InitLogInterFace(string _FileName)
{
	
	//日志
	g_ILogInterFace = CreateILogInterFace();
	if (g_ILogInterFace)
		printf("Create ILogInterFace Seccuss!\r\n");
	g_ILogInterFace->InitLog((char*)_FileName.c_str(), "\0", "Log/");
	
}
/*************************Parameter
*系统输出
*************************/
void CSystemMessage::SystemCharacter(int32_t _type,char *_txt,...)
{
	Guard guard(&m_LogMutex, true);
	if (!g_ILogInterFace)
	{
		return;
	}
	char *szCharacter = (char *)m_dzConvertBuffList._malloc_boost();
	if (!szCharacter) return;

	va_list marker;
	va_start(marker, _txt);
	int32_t szContextSize = vsnprintf(szCharacter, MAX_BUFF_LOG_SIZE, _txt, marker);
	va_end(marker);
	if (strstr(szCharacter, "%") == NULL)
	{
		
		if (m_IoStream != LOG_WRITE_NOMOL)
			g_ILogInterFace->WaitingLocal_IO_FILE(_type,
				szCharacter);
		
	}
	else
		if (m_IoStream != LOG_WRITE_NOMOL)
		{
			
			g_ILogInterFace->WaitingLocal_Context_FILE(_type,
				szCharacter, szContextSize);
		}
			
	
	m_dzConvertBuffList._free_boost((LPConvertBuffer)szCharacter);
}

void CSystemMessage::SystemCharacter(char *_txt,...)
{
	Guard guard(&m_LogMutex, true);
	if (!g_ILogInterFace)
	{
		return;
	}
	char *szCharacter = (char *)m_dzConvertBuffList._malloc_boost();
	if (!szCharacter) return;

	va_list marker;	
	va_start(marker,_txt);
	vsnprintf(szCharacter,MAX_BUFF_LOG_SIZE, _txt, marker); 
	va_end(marker);

	if(m_IoStream != LOG_WRITE_NOMOL)
		g_ILogInterFace->WaitingLocal_IO_FILE(LOG_WRITE_IO,
		szCharacter);
	m_dzConvertBuffList._free_boost((LPConvertBuffer)szCharacter);
}

