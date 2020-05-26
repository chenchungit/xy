#include "ServerHead.h"

CEvent *CEventManager::CreateEvent(bool bManualReset, bool bInitialState)
{
	CEvent *pEvent = new CEvent(bManualReset, bInitialState);
	bool bRet = pEvent->CreateEvent();
	if (!bRet)
		return NULL;
	return pEvent;
}
uint32_t CEventManager::WaitForSingleObject(CEvent *pEvent, int32_t cms)
{
	if (pEvent && pEvent->Wait(cms))
	{
		return 0;
	}
	return 1;
}
void CEventManager::CloseHandle(CEvent *pEvent)
{
	if(pEvent)
	  delete pEvent;
	pEvent = NULL;
}



void CEventManager::SetEvent(CEvent *pEvent)
{
	if(pEvent)
	   pEvent->Set();
}

void CEventManager::ResetEvent(CEvent *pEvent)
{
	if(pEvent)
	   pEvent->Reset();
}
void *CEventManager::pFunc1(void *pEvent)
{
	while (true)
	{
		WaitForSingleObject(m_Event, INFINITE);
		sleep(1);
		ResetEvent(m_Event);

	}
	return NULL;
}



void *CEventManager::pFunc2(void *pEvent)
{
	while (true)
	{
		sleep(5);
		SetEvent(m_Event);
	}
	return NULL;
}
