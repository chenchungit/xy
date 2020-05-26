#pragma once
#ifndef _HTTPCLIENTMANAGER_H_
#define _HTTPCLIENTMANAGER_H_

class CHttpClientManager
{
public:
	CHttpClientManager() {}
	~CHttpClientManager() {}
	//========================================================================
	//文件下载回调函数
	//_SrcData数据来源
	//_size单个数据类型长度如int 4个字节
	//_nmemb 多少个_size这样得数据
	//_stream文件流
	static size_t HttpDownLoadCallback(void *_SrcData, size_t _size, size_t _nmemb, FILE *_stream);

	//========================================================================
	//设置文件保存路径
	//_FilePath 路径
	inline void setFilePath(string _FilePath) { m_FilePath = _FilePath; };
	inline string getFilePath() { return m_FilePath; };
	//========================================================================
	//初始化
	bool Init();

	//========================================================================
	//文件下载
	//下载得文件名
	size_t DownLoadFile( string _FileName,uint32_t _FileCode);

	

	
private:
	string m_FilePath;
	CURL *m_curl;
	FILE *m_fp;
};
#endif