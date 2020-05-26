#include "ServerHead.h"

CLConfigManager::CLConfigManager()
{
	memset(m_szKey, 0, sizeof(m_szKey));
	m_fp = NULL;
}
CLConfigManager::~CLConfigManager()
{
	m_Map.clear();
}

INI_RES CLConfigManager::OpenFile(const char* _pathName, const char* _type)
{
	string szLine, szMainKey, szLastMainKey, szSubKey;
	char strLine[CONFIGLEN] = { 0 };
	KEYMAP mLastMap;
	int  nIndexPos = -1;
	int  nLeftPos = -1;
	int  nRightPos = -1;
	m_fp = fopen(_pathName, _type);
	if (m_fp == NULL)
	{
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_ERROR, "open inifile %s error!\n", _pathName);
		
		return INI_OPENFILE_ERROR;
	}
	m_Map.clear();
	while (fgets(strLine, CONFIGLEN, m_fp))
	{
		szLine.assign(strLine);
		//删除字符串中的非必要字符
		nLeftPos = szLine.find("\n");
		if (string::npos != nLeftPos)
		{
			szLine.erase(nLeftPos, 1);
		}
		nLeftPos = szLine.find("\r");
		if (string::npos != nLeftPos)
		{
			szLine.erase(nLeftPos, 1);
		}
		//判断是否是主键
		nLeftPos = szLine.find("[");
		nRightPos = szLine.find("]");
		if (nLeftPos != string::npos && nRightPos != string::npos)
		{
			szLine.erase(nLeftPos, 1);
			nRightPos--;
			szLine.erase(nRightPos, 1);
			m_Map[szLastMainKey] = mLastMap;
			mLastMap.clear();
			szLastMainKey = szLine;
		}
		else
		{
			//是否是子键
			if (nIndexPos = szLine.find("="), string::npos != nIndexPos)
			{
				string szSubKey, szSubValue;
				szSubKey = szLine.substr(0, nIndexPos);
				szSubValue = szLine.substr(nIndexPos + 1, szLine.length() - nIndexPos - 1);
				Trim(szSubKey);
				Trim(szSubValue);
				mLastMap[szSubKey] = szSubValue;
			}
			else
			{
				//TODO:不符合ini键值模板的内容 如注释等

			}
		}
	}
	//插入最后一次主键
	m_Map[szLastMainKey] = mLastMap;
	return INI_SUCCESS;

}
//去掉所有空格
void CLConfigManager::Trim(string &s)
{
	int index = 0;
	if (!s.empty())
	{
		while ((index = s.find(' ', index)) != string::npos)
		{
			s.erase(index, 1);
		}
	}

}


INI_RES CLConfigManager::CloseFile()
{
	if (m_fp != NULL)
	{
		fclose(m_fp);
		m_fp = NULL;
	}
	return INI_SUCCESS;
}
INI_RES CLConfigManager::GetKey(const char* _mAttr, const char* _cAttr, char* _Value)
{
	KEYMAP mKey = m_Map[_mAttr];
	string sTemp = mKey[_cAttr];
	strcpy(_Value, sTemp.c_str());
	return INI_SUCCESS;
}
int CLConfigManager::GetInt(const char* _mAttr, const char* _cAttr)
{
	int nRes = 0;
	memset(m_szKey, 0, sizeof(m_szKey));
	if (INI_SUCCESS == GetKey(_mAttr, _cAttr, m_szKey))
	{
		nRes = atoi(m_szKey);
	}
	return nRes;
}
char *CLConfigManager::GetStr(const char* _mAttr, const char* _cAttr)
{
	memset(m_szKey, 0, sizeof(m_szKey));
	if (INI_SUCCESS != GetKey(_mAttr, _cAttr, m_szKey))
	{
		strcpy(m_szKey, "NULL");
	}
	return m_szKey;
}
