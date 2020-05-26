
//RedisClient.cpp
#include "ServerHead.h"
using namespace std;
using namespace boost;

CRedisClientManager::CRedisClientManager()
{
	
}

CRedisClientManager::~CRedisClientManager()
{
	lock_guard<mutex> lock(m_mutex);
	while (!m_clients.empty())
	{
		redisContext *ctx = m_clients.front();
		redisFree(ctx);
		m_clients.pop();
	}
}
bool CRedisClientManager::InitRedis(std::string ip, int port, int DBIndex, std::string Key, int timeout)
{
	m_timeout = timeout;
	m_serverPort = port;
	m_setverIp = ip;
	m_DBIndex = DBIndex;
	m_beginInvalidTime = 0;
	m_key = Key;
	m_ThreadID = 0L;
	//pthread_create(&m_ThreadID, NULL, RedisThreadFuntion, this);
	return true;
}
void* CRedisClientManager::RedisThreadFuntion(void* para)
{
	CRedisClientManager *pWnd = (CRedisClientManager*)para;

	std::stringstream szTableStream, szKeyStream;

	std::stringstream szValueStr;

	szTableStream << pWnd->m_DBIndex;
	szKeyStream << pWnd->m_key;
	while (g_ThreadRun)
	{
		szValueStr.str("");
		if (pWnd->select(szTableStream.str(), szValueStr))
		{
			szValueStr.str("");
			if (pWnd->lpop(szKeyStream.str(), szValueStr))
			{
				pWnd->JsonProtocol(szValueStr);
			}
			else
			{
				//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "lpop Fail ");
				sleep(2);
			}
		}
		else
		{
			//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "select Fail ");
			sleep(2);
		}
			

		
	}
}

void CRedisClientManager::JsonProtocol(std::stringstream &ValueStream)
{
	//MessageBox(NULL, _Body.c_str(),"",MB_OK);
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "JsonProtocol msg =%s ", ValueStream.str().c_str());
	int szActionType = 0;
	Json::Reader  reader;
	Json::Value   szRoot, szData;
	bool isParse = reader.parse(ValueStream.str(), szRoot, false);
	//{"actionType":1,"data":{"name":"wangkun1","mobile":"19922824005"}}
	g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "JsonProtocol  =%d   %d", isParse, szRoot["actionType"].isInt());
	if (isParse && szRoot["actionType"].isInt())
	{
		/*if (szRoot["actionType"].asInt())
		{*/
		g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "HttpInterface type =%d ", szRoot["actionType"].asInt());
		//获取类型

		//}

	}
}
bool CRedisClientManager::get(const string& key, std::stringstream &ValueStream) {
	redisContext *ctx = CreateContext();
	if (ctx == NULL) {
		return false;
	}

	redisReply *r = (redisReply*)redisCommand(ctx, "GET %s", key.c_str());
	boost::shared_ptr<redisReply> autoFree(r, freeReplyObject);
	ReleaseContext(ctx, r != NULL);

	if (r == NULL) {
		return false;
	}

	if (r->type != REDIS_REPLY_STRING) {
		return false;
	}
	
	ValueStream << string(r->str);

	return true;
}

bool CRedisClientManager::set(const string& key, const std::stringstream &ValueStream) {
	redisContext *ctx = CreateContext();
	if (ctx == NULL) {
		return false;
	}


	redisReply *r = (redisReply*)redisCommand(ctx, "SET %s %s", key.c_str(), ValueStream.str().c_str());
	boost::shared_ptr<redisReply> autoFree(r, freeReplyObject);
	ReleaseContext(ctx, r != NULL);

	if (r == NULL) {
		return false;
	}
	if (!(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str, "OK") == 0)) {
		return false;
	}
	return true;
}

bool CRedisClientManager::rpush(const string& key, const std::stringstream &ValueStream)
{
	std::stringstream szTableStream, szKeyStream;
	szTableStream << m_DBIndex;
	szKeyStream << m_key;
	std::stringstream szValueStr;
	if (select(szTableStream.str(), szValueStr))
	{
		redisContext *ctx = CreateContext();
		if (ctx == NULL) {
			return false;
		}

		redisReply *r = (redisReply*)redisCommand(ctx, "RPUSH %s %s", key.c_str(), ValueStream.str().c_str());
		boost::shared_ptr<redisReply> autoFree(r, freeReplyObject);
		ReleaseContext(ctx, r != NULL);

		if (r == NULL) {
			return false;
		}
		if (r->type != REDIS_REPLY_INTEGER) {
			return false;
		}
		return true;
	}
	
	return false;
}

bool CRedisClientManager::select(const std::string &key, std::stringstream &ValueStream)
{
	redisContext *ctx = CreateContext();
	if (ctx == NULL) {
		return false;
	}
	//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "select  =%s ", key.c_str());
	redisReply *r = (redisReply*)redisCommand(ctx, "SELECT %s", key.c_str());
	boost::shared_ptr<redisReply> autoFree(r, freeReplyObject);
	ReleaseContext(ctx, r != NULL);

	if (r == NULL) {
		return false;
	}
	//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "select type =%d ", r->type);
	if (r->type != REDIS_REPLY_STATUS) {
		return false;
	}

	ValueStream << string(r->str);
	return true;
}

bool CRedisClientManager::lpop(const string& key, std::stringstream &ValueStream)
{
	redisContext *ctx = CreateContext();
	if (ctx == NULL) {
		return false;
	}
	//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "lpop  =%s ", key.c_str());
	redisReply *r = (redisReply*)redisCommand(ctx, "LPOP %s", key.c_str());
	boost::shared_ptr<redisReply> autoFree(r, freeReplyObject);
	ReleaseContext(ctx, r != NULL);

	if (r == NULL) {
		return false;
	}
	//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "lpop type =%d ", r->type);
	if (r->type != REDIS_REPLY_STRING) {
		return false;
	}
	
	ValueStream << r->str;
	//g_ServerManager->SystemCharacter(LOG_WRITE_BOTH | LOG_WRITE_GENERAL, "lpop data =%s ", ValueStream.str().c_str());
	return true;
}

bool CRedisClientManager::lrange(const std::string& key, int start, int end, std::vector<std::string>& Values)
{
	redisContext *ctx = CreateContext();
	if (ctx == NULL) {
		return false;
	}

	redisReply *r = (redisReply*)redisCommand(ctx, "LRANGE %s %d %d", key.c_str(), start, end);
	boost::shared_ptr<redisReply> autoFree(r, freeReplyObject);
	ReleaseContext(ctx, r != NULL);

	if (r == NULL) {
		return false;
	}
	if (r->type != REDIS_REPLY_ARRAY) {
		return false;
	}
	for (int i = 0; i < (int)r->elements; i++) {
		redisReply *cur = r->element[i];
		if (cur->type != REDIS_REPLY_STRING) {
			return false;
		}
		Values.push_back(cur->str);
	}
	return true;
}

redisContext* CRedisClientManager::CreateContext()
{
	{
		lock_guard<mutex> lock(m_mutex);
		if (!m_clients.empty())
		{
			redisContext *ctx = m_clients.front();
			m_clients.pop();

			return ctx;
		}
	}

	time_t now = time(NULL);
	if (now < m_beginInvalidTime + m_maxReconnectInterval) return NULL;

	struct timeval tv;
	tv.tv_sec = m_timeout / 1000;
	tv.tv_usec = (m_timeout % 1000) * 1000;;
	redisContext *ctx = redisConnectWithTimeout(m_setverIp.c_str(), m_serverPort, tv);
	if (ctx == NULL || ctx->err != 0)
	{
		if (ctx != NULL) redisFree(ctx);

		m_beginInvalidTime = time(NULL);

		return NULL;
	}

	return ctx;
}
bool CRedisClientManager::channel(const std::string &key, const std::stringstream &ValueStream)
{
	std::stringstream szTableStream, szKeyStream;
	szTableStream << m_DBIndex;
	szKeyStream << m_key;
	std::stringstream szValueStr;
	if (select(szTableStream.str(), szValueStr))
	{
		redisContext *ctx = CreateContext();
		if (ctx == NULL) {
			return false;
		}

		redisReply *r = (redisReply*)redisCommand(ctx, "PUBLISH %s %s", key.c_str(), ValueStream.str().c_str());
		boost::shared_ptr<redisReply> autoFree(r, freeReplyObject);
		ReleaseContext(ctx, r != NULL);

		if (r == NULL) {
			return false;
		}
		if (r->type != REDIS_REPLY_INTEGER) {
			return false;
		}
		return true;
	}

	return false;
}

void CRedisClientManager::ReleaseContext(redisContext *ctx, bool active)
{
	if (ctx == NULL) return;
	if (!active) { redisFree(ctx); return; }

	lock_guard<mutex> lock(m_mutex);
	m_clients.push(ctx);
}

bool CRedisClientManager::CheckStatus(redisContext *ctx)
{
	redisReply *reply = (redisReply*)redisCommand(ctx, "ping");
	if (reply == NULL) return false;

	boost::shared_ptr<redisReply> autoFree(reply, freeReplyObject);

	if (reply->type != REDIS_REPLY_STATUS) return false;
	if (strcasecmp(reply->str, "PONG") != 0) return false;

	return true;
}

bool CRedisClientManager::hget(const string& key, int field, std::stringstream &ValueStream)
{
	redisContext *ctx = CreateContext();
	if (ctx == NULL) {
		return false;
	}

	redisReply *r = (redisReply*)redisCommand(ctx, "HGET %s %d", key.c_str(), field);
	boost::shared_ptr<redisReply> autoFree(r, freeReplyObject);
	ReleaseContext(ctx, r != NULL);

	if (r == NULL) {
		return false;
	}
	if (r->type != REDIS_REPLY_STRING) {
		return false;
	}

	ValueStream << string(r->str);
	return true;
}

bool CRedisClientManager::hget(const string& key, const string& field, std::stringstream &ValueStream)
{
	redisContext *ctx = CreateContext();
	if (ctx == NULL) {
		return false;
	}

	redisReply *r = (redisReply*)redisCommand(ctx, "HGET %s %s", key.c_str(), field.c_str());
	boost::shared_ptr<redisReply> autoFree(r, freeReplyObject);
	ReleaseContext(ctx, r != NULL);

	if (r == NULL) {
		return false;
	}
	if (r->type != REDIS_REPLY_STRING) {
		return false;
	}

	ValueStream << string(r->str);
	return true;
}

bool CRedisClientManager::zrange(const std::string& key, int start, int end, std::vector<std::string>& Values)
{
	redisContext *ctx = CreateContext();
	if (ctx == NULL) {
		return false;
	}

	redisReply *r = (redisReply*)redisCommand(ctx, "ZRANGE %s %d %d", key.c_str(), start, end);
	boost::shared_ptr<redisReply> autoFree(r, freeReplyObject);
	ReleaseContext(ctx, r != NULL);

	if (r == NULL) {
		return false;
	}
	if (r->type != REDIS_REPLY_ARRAY) {
		return false;
	}
	for (int i = 0; i < (int)r->elements; i++) {
		redisReply *cur = r->element[i];
		if (cur->type != REDIS_REPLY_STRING) {
			return false;
		}
		Values.push_back(cur->str);
	}
	return true;
}