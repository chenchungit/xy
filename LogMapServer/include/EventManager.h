#pragma once
#ifndef _EVENTMANAGER_H_
#define _EVENTMANAGER_H_

class CEventManager
{
public:
	CEvent * CreateEvent(bool bManualReset, bool bInitialState);
	uint32_t WaitForSingleObject(CEvent *pEvent, int32_t cms);
	void CloseHandle(CEvent *pEvent);
	void SetEvent(CEvent *pEvent);
	void ResetEvent(CEvent *pEvent);
	void *pFunc1(void *pEvent);
	void *pFunc2(void *pEvent);
private:
	CEvent * m_Event;
};
#endif