#include "ServerHead.h"


CRedisObject::CRedisObject()
{
}
CRedisObject::CRedisObject(char* _RedisIP, int _Port, int _Seconds, DWORD _RedisID, int _DBIndex, char*_Key) :
	
	m_Port(_Port),
	m_Timeout(_Seconds),
	m_RedisID(_RedisID),
	m_DBIndex(_DBIndex)
	
{
	m_RedisCon = NULL;
	m_Reply = NULL;
	RtlZeroMemory(m_RedisIP,32);
	RtlZeroMemory(m_Key, 32);
	RtlCopyMemory(m_RedisIP, _RedisIP, strlen(_RedisIP));
	RtlCopyMemory(m_Key, _Key, strlen(_Key));
}

CRedisObject::~CRedisObject()
{
	freeReply();
	if (nullptr == m_RedisCon) {
		redisFree(m_RedisCon);
		m_RedisCon = nullptr;
	}
}

redisContext* CRedisObject::InitRedis()
{
	if (nullptr != m_RedisCon) {
		return m_RedisCon;
	}
	m_RedisCon = redisConnect(m_RedisIP, m_Port);
	if (m_RedisCon->err) {
		std::cerr << "error code : " << m_RedisCon->err << ". " << m_RedisCon->errstr << std::endl;
		return m_RedisCon;
	}
	return NULL;
}
redisContext* CRedisObject::InitRedis(int _Seconds)
{
	if (NULL != m_RedisCon) {
		return m_RedisCon;
	}
	struct timeval tv;
	tv.tv_sec = _Seconds;
	tv.tv_usec = 0;
	m_RedisCon = redisConnectWithTimeout(m_RedisIP, m_Port, tv);
	if (m_RedisCon->err) {
		std::cerr << "error code : " << m_RedisCon->err << ". " << m_RedisCon->errstr << std::endl;
		return NULL;
	}
	return m_RedisCon;
}
void CRedisObject::freeReply()
{
	if (nullptr != m_Reply)
	{
		::freeReplyObject(m_Reply);
		m_Reply = nullptr;
	}
}
template<class T, class... Args>
bool CRedisObject::HashSet(const std::string command, T head, Args... rest) {
	std::stringstream ss;
	ss << command << " " << head << " ";
	return HashSetInner(ss, rest...);
}
template<typename T>
bool CRedisObject::Set(const std::string & key, const T& value)
{
	/*bool bret = false;
	std::stringstream ss;
	ss << "SET " << key << " " << value;
	std::string s;
	getline(ss, s);
	return Set(s);*/
	return false;
}
void CRedisObject::disConnect()
{
	::redisFree(m_RedisCon);
	cout << "redis disConnect success" << endl;
}