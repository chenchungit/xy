#include "ServerHead.h"

CEvent::CEvent(bool bIsManualReset, bool bInitialSignaled)
	: m_bIsManualReset(bIsManualReset)
	, m_bEventStatus(bInitialSignaled)
	, m_bMutexInitialized(false)
	, m_bCondInitialized(false)
{
}
CEvent::~CEvent()
{
	if (m_bMutexInitialized)
	{
		pthread_mutex_destroy(&m_mutex);
		m_bMutexInitialized = false;
	}
	if (m_bCondInitialized)
	{
		pthread_cond_destroy(&m_cond);
		m_bCondInitialized = false;
	}
}





bool CEvent::CreateEvent()
{
	if (!m_bMutexInitialized)
	{
		if (0 == pthread_mutex_init(&m_mutex, NULL))
		{
			m_bMutexInitialized = true;
		}
	}
	if (!m_bCondInitialized)
	{
		if (0 == pthread_cond_init(&m_cond, NULL))
		{
			m_bCondInitialized = true;
		}
	}
	return (m_bMutexInitialized && m_bCondInitialized);
}



bool CEvent::EnsureInitialized()
{
	return (m_bMutexInitialized && m_bCondInitialized);
}



bool CEvent::Set()
{
	if (!EnsureInitialized())
	{
		return false;
	}
	pthread_mutex_lock(&m_mutex);
	m_bEventStatus = true;
	pthread_cond_broadcast(&m_cond);
	pthread_mutex_unlock(&m_mutex);
	return true;
}



bool CEvent::Reset()
{
	if (!EnsureInitialized())
	{
		return false;
	}
	pthread_mutex_lock(&m_mutex);
	m_bEventStatus = false;
	pthread_mutex_unlock(&m_mutex);
	return true;
}



bool CEvent::Wait(int32_t cms)
{
	if (!EnsureInitialized())
	{
		return false;
	}
	pthread_mutex_lock(&m_mutex);
	int32_t error = 0;
	if (cms != INFINITE)
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		struct timespec ts;
		ts.tv_sec = tv.tv_sec + (cms / 1000);
		ts.tv_nsec = tv.tv_usec * 1000 + (cms % 1000) * 1000000;
		if (ts.tv_nsec >= 1000000000)
		{
			ts.tv_sec++;
			ts.tv_nsec -= 1000000000;
		}
		while ((!m_bEventStatus) && (error == 0))
		{
			error = pthread_cond_timedwait(&m_cond, &m_mutex, &ts);
		}
	}
	else
	{
		while ((!m_bEventStatus) && (error == 0))
		{
			error = pthread_cond_wait(&m_cond, &m_mutex);
		}
	}
	if (0 == error && !m_bIsManualReset)
	{
		m_bEventStatus = false;
	}
	pthread_mutex_unlock(&m_mutex);
	return (0 == error);
}


