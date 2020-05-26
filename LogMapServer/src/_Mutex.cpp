
#include "ServerHead.h"

namespace CC
{
//---------------------------------------------------------
ThreadMutex::ThreadMutex(void)
{
	pthread_mutex_init(&m_Mutex, NULL);
	//::InitializeCriticalSection(&cs_);
}

ThreadMutex::~ThreadMutex(void)
{
	pthread_mutex_destroy(&m_Mutex);
}

void ThreadMutex::Lock()
{
	pthread_mutex_lock(&m_Mutex); /*»ñÈ¡»¥³âËø*/
	
}
void ThreadMutex::UnLock()
{
	pthread_mutex_unlock(&m_Mutex); /*ÊÍ·Å»¥³âËø*/
}

//---------------------------------------------------------

//ProcessMutex::ProcessMutex(char* szMutexName)
//{
//	hMutex = ::CreateMutexA(NULL, FALSE, szMutexName);
//}
//
//ProcessMutex::~ProcessMutex()
//{
//	::CloseHandle(hMutex);
//}
//
//void ProcessMutex::Lock()
//{
//	WaitForSingleObject(hMutex, INFINITE);
//}
//
//void ProcessMutex::UnLock()
//{
//	::SetEvent(hMutex);
//}

//---------------------------------------------------------

Guard::Guard(Mutex* pMutex, bool bAutoLock)
:pMutex_(NULL)
,bLock(false)
{
	pMutex_ = pMutex;
	if(bAutoLock)
		Lock();
}

Guard::~Guard()
{
	UnLock();
}

void Guard::Lock()
{
	if(pMutex_)
	{
		pMutex_->Lock();
		bLock = true;
	}
}
void Guard::UnLock()
{
	if(bLock)
	{
		pMutex_->UnLock();
		bLock = false;
	}
}
//---------------------------------------------------------


}// end namespace CC
