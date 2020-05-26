#pragma once
#ifndef _WECHATMANAGER_H_
#define _WECHATMANAGER_H_

#define WECHAT_ERROR_CODE_SUCCESS 0
#define WECHAT_ERROR_CODE_NOLOGIN 1//�Լ�δ��¼
#define WECHAT_ERROR_CODE_NOLOGIN_TARGET 2//�Է�δ��¼
#define WECHAT_ERROR_CODE_NOUSERORROOM 4//δ�ҵ����û����߷���Ƿ�
using namespace std;
class CWechatManager
{
public:
	//��ʼ��
	void Init();
	//����û�
	static bool WeChatLoginFace(std::string _data, mg_connection *_Con);
	void WeChatLogin(const char * _Phone, mg_connection *_Con);

	//������Ƶ
	static bool WeChatVedioFace(std::string _data, mg_connection *_Con);
	void WeChatVedio(const char * _SrcPhone, mg_connection *_SrcCon, const char * _DrcPhone);
	//������Ƶ
	static bool JoinWeChatVedioFace(std::string _data, mg_connection *_Con);
	void JoinWeChatVedio(const char * _Phone, mg_connection *_Con, uint32_t _RoomID);

	//���Ϳ�����Ȩ
	static bool WeChatOauthLockFace(std::string _data, mg_connection *_Con);
	void WeChatOauthLock(const char * _Phone, const char * _LockKey, const char * _DeviceName, mg_connection *_Con);
	void SendLockKey(const char * _Phone, const char * _LockKey, const char * _DeviceName, mg_connection *_Con);

	static bool WeChatLogoutFace(std::string _data, mg_connection *_Con);
	void WeChatLogout(const char * _Phone);
	void SendLogout(mg_connection *_Con);

	
	void SendRoomID(uint32_t _RoomID, mg_connection *_Con, const char * _Phone);
	void SendErrorCode(uint32_t _Code, mg_connection *_Con);


	//����slg
	void CrySlg(const char *AppKey, const char *SecretKey );
private:
	//�ڴ��
	objectPool_CC<CWechatObject> m_dzWechatPool;
	//���ݶ���
	map<string, CWechatObject*> m_dzWechatData;
};
#endif