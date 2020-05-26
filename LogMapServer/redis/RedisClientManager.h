#pragma once
#ifndef _REDISCLIENTMANAGER_H_
#define _REDISCLIENTMANAGER_H_


class CRedisClientManager
{
public:
	CRedisClientManager();
	virtual ~CRedisClientManager();

	

	bool InitRedis(std::string ip, int port, int DBIndex, std::string Key, int timeout = 2000);


	bool channel(const std::string &key, const std::stringstream &ValueStream);
	bool set(const std::string &key, const std::stringstream &ValueStream);
	bool get(const std::string &key, std::stringstream &ValueStream);
	bool rpush(const std::string &key, const std::stringstream &ValueStream);
	bool lpop(const std::string &key, std::stringstream &ValueStream);
	bool select(const std::string &key, std::stringstream &ValueStream);

	bool hget(const std::string &key, const std::string& field, std::stringstream &ValueStream);
	bool hget(const std::string &key, int field, std::stringstream &ValueStream);
	bool lrange(const std::string& key, int start, int end, std::vector<std::string>& Values);
	bool zrange(const std::string& key, int start, int end, std::vector<std::string>& Values);

	void JsonProtocol(std::stringstream &ValueStream);
	static void* RedisThreadFuntion(void* para);
	pthread_t m_ThreadID;
private:
	int m_timeout;
	int m_serverPort;
	DWORD m_RedisID;
	int m_DBIndex;
	std::string m_key;
	std::string m_setverIp;


	std::mutex m_mutex;
	std::queue<redisContext *> m_clients;

	time_t m_beginInvalidTime;
	static const int m_maxReconnectInterval = 3;

	redisContext* CreateContext();
	void ReleaseContext(redisContext *ctx, bool active);
	bool CheckStatus(redisContext *ctx);

	
};

typedef boost::shared_ptr<CRedisClientManager> RedisClientPtr;
typedef boost::shared_ptr<const CRedisClientManager> const_RedisClientPtr;

#endif // !_REDISCLIENTMANAGER_H_