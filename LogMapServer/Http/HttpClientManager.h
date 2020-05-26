#pragma once
#ifndef _HTTPCLIENTMANAGER_H_
#define _HTTPCLIENTMANAGER_H_

class CHttpClientManager
{
public:
	CHttpClientManager() {}
	~CHttpClientManager() {}
	//========================================================================
	//�ļ����ػص�����
	//_SrcData������Դ
	//_size�����������ͳ�����int 4���ֽ�
	//_nmemb ���ٸ�_size����������
	//_stream�ļ���
	static size_t HttpDownLoadCallback(void *_SrcData, size_t _size, size_t _nmemb, FILE *_stream);

	//========================================================================
	//�����ļ�����·��
	//_FilePath ·��
	inline void setFilePath(string _FilePath) { m_FilePath = _FilePath; };
	inline string getFilePath() { return m_FilePath; };
	//========================================================================
	//��ʼ��
	bool Init();

	//========================================================================
	//�ļ�����
	//���ص��ļ���
	size_t DownLoadFile( string _FileName,uint32_t _FileCode);

	

	
private:
	string m_FilePath;
	CURL *m_curl;
	FILE *m_fp;
};
#endif