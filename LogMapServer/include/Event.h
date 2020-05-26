#pragma once
#ifndef _EVENT_H_
#define _EVENT_H_

const int32_t INFINITE = -1;
class CEvent
{
public:
	CEvent(bool bIsManualReset, bool bInitialSignaled);
	~CEvent();
	bool CreateEvent();
	bool Set();
	bool Reset();
	bool Wait(int32_t cms);
private:
	bool EnsureInitialized();
	bool m_bIsManualReset;
	bool m_bEventStatus;
	bool m_bMutexInitialized;
	pthread_mutex_t m_mutex;
	bool m_bCondInitialized;
	pthread_cond_t m_cond;
};
#endif