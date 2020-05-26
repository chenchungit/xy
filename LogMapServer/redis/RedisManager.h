#pragma once
class CRedisManager : public CBaseOperationObject
{
public:
	CRedisManager();
	~CRedisManager();
	virtual void Init() {};
	virtual void InitSynData(TcpSocketID id, TcpSocketID _Token, ::google::protobuf::Message& _Protocol) {};
	virtual void QueueData(TcpSocketID id, TcpSocketID _Token, ::google::protobuf::Message& _Protocol) { };
	virtual void ModifyData(TcpSocketID id, TcpSocketID _Token, ::google::protobuf::Message& _Protocol) { };
	virtual void CreateData(TcpSocketID id, TcpSocketID _Token, ::google::protobuf::Message& _Protocol) { };
	virtual bool OnTimer(unsigned short _MajorID, unsigned short _MinorID, int _Delay) { return 1; };
public:
	bool HashSetInner(DWORD _RedisID,std::stringstream& _Stream);

	template<class T, class... Args>
	bool HashSetInner(DWORD _RedisID, std::stringstream& _Stream, T head, Args... rest);

	bool Set(DWORD _RedisID, std::string _Data);
	redisReply * getString(DWORD _RedisID, const string & _key);
	redisReply * getString(DWORD _RedisID, const string & _Tableindex,const string & _key);
	void getString(DWORD _RedisID, const string & _key, string & _value);
	void getString(DWORD _RedisID, const string & _key, int & _value);
	void getString(DWORD _RedisID, const string & _key, float & _value);
	bool isError(DWORD _RedisID, redisReply *_Reply);
	void freeReply(redisReply *_Reply);
	redisContext* CreateRedis(char* _RedisIP, int _Port, int _Seconds, DWORD _RedisID, int _DBIndex,char* _Key);

	//json
	void JsonProtocol(string _Body);

	//void ReleaseContext(redisContext *ctx, bool active);
	bool CheckStatus(redisContext *ctx);
	static void* RedisThreadFuntion(void* para);
	//static void RedisThreadFuntion(LPVOID para);
private:
	map<DWORD, CRedisObject*>m_dzRedisConn;

	pthread_t m_ThreadID;
	//线程相关的变量
	/*void*	m_ThreadIO;
	HANDLE m_CompletionPort;
	unsigned long m_MSeconds;*/
};

