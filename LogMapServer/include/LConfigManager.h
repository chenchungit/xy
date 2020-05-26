#pragma once
#include <stdio.h>
#ifndef _LCONFIGMANAGER_H_
#define _LCONFIGMANAGER_H_

#define CONFIGLEN           256 
enum INI_RES
{
	INI_SUCCESS,            //�ɹ�
	INI_ERROR,              //��ͨ����
	INI_OPENFILE_ERROR,     //���ļ�ʧ��
	INI_NO_ATTR            //�޶�Ӧ�ļ�ֵ
};
//�Ӽ�����    �Ӽ�ֵ 
typedef map<std::string, std::string> KEYMAP;
//              �������� ����ֵ  
typedef map<std::string, KEYMAP> MAINKEYMAP;
class CLConfigManager
{
public:
	CLConfigManager();
	~CLConfigManager();

public:
	//��ȡ���εļ�ֵ
	int  GetInt(const char* _mAttr, const char* _cAttr);
	//��ȡ��ֵ���ַ���
	char *GetStr(const char* _mAttr, const char* _cAttr);
	// ��config �ļ�
	INI_RES OpenFile(const char* _pathName, const char* _type);
	// �ر�config �ļ�
	INI_RES CloseFile();
	//ȥ�����пո�
	void Trim(string &s);
protected:
	// ��ȡconfig�ļ�
	INI_RES GetKey(const char* _mAttr, const char* _cAttr, char* _value);
protected:
	// ���򿪵��ļ��ֱ�
	FILE* m_fp;
	char  m_szKey[CONFIGLEN];
	MAINKEYMAP m_Map;

};

#endif