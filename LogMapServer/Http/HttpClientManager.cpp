
#include "ServerHead.h"

// �ͻ��˵�����������Ӧ
size_t CHttpClientManager::HttpDownLoadCallback(void *_SrcData, size_t _size, size_t _nmemb, FILE *_stream)
{
	size_t written = fwrite(_SrcData, _size, _nmemb, _stream);
	g_ServerManager->SystemCharacter(LOG_WRITE_IO | LOG_WRITE_GENERAL, "size = %d nmemb = %d", _size, _nmemb);
	return written;
}
bool CHttpClientManager::Init()
{
	m_curl = curl_easy_init();
	if (m_curl) {
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "***********Init Curl Success**********");
		return true;
	}
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "***********Init Curl Fail**********");
	return false;

}
size_t CHttpClientManager::DownLoadFile( string _FileName, uint32_t _FileCode)
{
	stringstream szSavePath,szDownFileUrl;
	size_t szFileSize = 0;
	szSavePath << m_FilePath << _FileCode<<"_" << _FileName;
#ifdef VIANS
	szDownFileUrl << SERVER_VIANS_CURL_DOWNLOADURL << _FileName;
#else
	szDownFileUrl << SERVER_CURL_DOWNLOADURL << _FileName;
#endif
	CURLcode szRes;
	m_fp = fopen(szSavePath.str().c_str(), "wb");
	if (m_fp)
	{
		//#define CINIT(na,t,nu) CURLOPT_ ## na = CURLOPTTYPE_ ## t + nu
		curl_easy_setopt(m_curl, CURLOPT_URL, szDownFileUrl.str().c_str());
		curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, HttpDownLoadCallback);
		curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, m_fp);
		szRes = curl_easy_perform(m_curl);
		if (CURLE_OK == szRes)
		{
			fseek(m_fp, 0, SEEK_END);  //����fseek���ļ�ָ���Ƶ��ļ�ĩβ
			szFileSize = ftell(m_fp);    //����ftell��ȡ�ļ���ָ�뵱ǰ���ļ�λ�á�
			g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "curl_easy_perform file success!");
		}
		/* always cleanup */
		// curl_easy_cleanup(curl);
		fclose(m_fp);
		return szFileSize;
	}
	else
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "open or create file fail");
	return 0;
}