#pragma once
#ifndef _WECHATMANAGER_H_
#define _WECHATMANAGER_H_

#define WECHAT_ERROR_CODE_SUCCESS 0
#define WECHAT_ERROR_CODE_NOLOGIN 1//自己未登录
#define WECHAT_ERROR_CODE_NOLOGIN_TARGET 2//对方未登录
#define WECHAT_ERROR_CODE_NOUSERORROOM 4//未找到改用户或者房间非法
using namespace std;
class CWechatManager
{
public:
	//初始化
	void Init();
	//添加用户
	static bool WeChatLoginFace(std::string _data, mg_connection *_Con);
	void WeChatLogin(const char * _Phone, mg_connection *_Con);

	//申请视频
	static bool WeChatVedioFace(std::string _data, mg_connection *_Con);
	void WeChatVedio(const char * _SrcPhone, mg_connection *_SrcCon, const char * _DrcPhone);
	//加入视频
	static bool JoinWeChatVedioFace(std::string _data, mg_connection *_Con);
	void JoinWeChatVedio(const char * _Phone, mg_connection *_Con, uint32_t _RoomID);

	//发送开门授权
	static bool WeChatOauthLockFace(std::string _data, mg_connection *_Con);
	void WeChatOauthLock(const char * _Phone, const char * _LockKey, const char * _DeviceName, mg_connection *_Con);
	void SendLockKey(const char * _Phone, const char * _LockKey, const char * _DeviceName, mg_connection *_Con);

	static bool WeChatLogoutFace(std::string _data, mg_connection *_Con);
	void WeChatLogout(const char * _Phone);
	void SendLogout(mg_connection *_Con);

	
	void SendRoomID(uint32_t _RoomID, mg_connection *_Con, const char * _Phone);
	void SendErrorCode(uint32_t _Code, mg_connection *_Con);


	//加密slg
	void CrySlg(const char *AppKey, const char *SecretKey );
private:
	//内存池
	objectPool_CC<CWechatObject> m_dzWechatPool;
	//数据队列
	map<string, CWechatObject*> m_dzWechatData;
};
#endif