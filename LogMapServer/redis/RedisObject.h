#pragma once

class CRedisObject
{
public:
	CRedisObject();
	CRedisObject(char* _RedisIP,int _Port, int _Seconds, DWORD _RedisID,int _DBIndex,char * _Key);
	~CRedisObject();


public:
	inline redisContext* getRedisContext() {
		return m_RedisCon;
	}
	inline redisReply* getrRedisReply() {
		return m_Reply;
	}
	inline DWORD getRedisID() {
		return m_RedisID;
	}
	inline int getDBIndex() {
		return m_DBIndex;
	}
	inline string getKey() {
		return m_Key;
	}

	redisContext* InitRedis();
	redisContext* InitRedis(int _Seconds);
	void freeReply();
	void disConnect();


	template<class T, class... Args>
	bool HashSet(const std::string command, T head, Args... rest);
	template<typename T>
	bool Set(const std::string & key, const T& value);
private:
	redisContext* m_RedisCon;
	redisReply * m_Reply;
	char m_RedisIP[32];
	int m_Port;
	int m_Timeout;
	DWORD m_RedisID;
	int m_DBIndex;
	char m_Key[32];
};

