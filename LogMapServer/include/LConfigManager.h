#pragma once
#include <stdio.h>
#ifndef _LCONFIGMANAGER_H_
#define _LCONFIGMANAGER_H_

#define CONFIGLEN           256 
enum INI_RES
{
	INI_SUCCESS,            //成功
	INI_ERROR,              //普通错误
	INI_OPENFILE_ERROR,     //打开文件失败
	INI_NO_ATTR            //无对应的键值
};
//子键索引    子键值 
typedef map<std::string, std::string> KEYMAP;
//              主键索引 主键值  
typedef map<std::string, KEYMAP> MAINKEYMAP;
class CLConfigManager
{
public:
	CLConfigManager();
	~CLConfigManager();

public:
	//获取整形的键值
	int  GetInt(const char* _mAttr, const char* _cAttr);
	//获取键值的字符串
	char *GetStr(const char* _mAttr, const char* _cAttr);
	// 打开config 文件
	INI_RES OpenFile(const char* _pathName, const char* _type);
	// 关闭config 文件
	INI_RES CloseFile();
	//去掉所有空格
	void Trim(string &s);
protected:
	// 读取config文件
	INI_RES GetKey(const char* _mAttr, const char* _cAttr, char* _value);
protected:
	// 被打开的文件局柄
	FILE* m_fp;
	char  m_szKey[CONFIGLEN];
	MAINKEYMAP m_Map;

};

#endif