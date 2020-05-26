/*            _________           _________ 
*          / /________/        / /________/ 
*        / /                 / /     
*      / /                 / /
*    / /                 / /
*	/ /                  / /
*	/ /                  / /
*	 / /                  / /
*	  / / _________         / / _________
*	   / /________/          / /________/ 
* 
* mairycal@163.com
*/
#ifndef __CC_MUTEX_H__
#define __CC_MUTEX_H__

namespace CC
{

class Mutex
{
	friend class Guard;
private:
	virtual void Lock() = 0;
	virtual void UnLock() = 0;
};


class ThreadMutex : public Mutex
{
	
public:
	ThreadMutex(void);
	~ThreadMutex(void);
private:

	pthread_mutex_t m_Mutex;
	virtual void Lock();
	virtual void UnLock();
};

//class ProcessMutex : public Mutex
//{
//public:
//	ProcessMutex(char* szMutexName);
//	~ProcessMutex();
//private:
//	void virtual Lock();
//	void virtual UnLock();
//	HANDLE hMutex;
//};

class Guard
{
public:
	Guard(Mutex* pMutex, bool bAutoLock = true);
	~Guard();
public:
	void Lock();
	void UnLock();
private:
	Mutex*		pMutex_;
	bool		bLock;
};

class SharedMutexPool
{
public:
	SharedMutexPool(int32_t size)
	{
		m_size = size;
		m_counter = 0;
		m_pMutex = new ThreadMutex[size];
	}
	~SharedMutexPool()
	{
		delete[] m_pMutex;
	}
	ThreadMutex* operator & ()
	{
		uint32_t index = m_counter++;
		index %= m_size;
		return &m_pMutex[index];
	}
private:
	ThreadMutex* m_pMutex;
	uint32_t m_size;
	uint32_t m_counter;
};

}// end namespace CC

#endif//__CC_MUTEX_H__
