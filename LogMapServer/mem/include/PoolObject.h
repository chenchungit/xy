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

#ifndef _POOLOBJECT_H_
#define _POOLOBJECT_H_
#pragma once
#include "_Mutex.h"
#include "pool.hpp"
#include "object_pool.hpp"
#include <singleton_pool.hpp>
using namespace boost;

namespace CC
{
//µ¥¼þÄÚ´æ³Ø
template <class T>
class singletonPool_CC
{
public:
	singletonPool_CC(){};
	~singletonPool_CC(){
		m_dzSingletonPoolList::release_memory();
	};

public:
	T *_malloc_boost()
	{
		T * ret = (T *)m_dzSingletonPoolList::malloc();
		return ret;
	};
	void _free_boost(T *dzPtr)
	{
		if (m_dzSingletonPoolList::is_from(dzPtr))
		{
			m_dzSingletonPoolList::free(dzPtr);
			return;
		}
		else
		{
			delete dzPtr;
			dzPtr = NULL;
		}
	};

private:
	typedef boost::singleton_pool<DWORD,(uint32_t)sizeof( T)>	m_dzSingletonPoolList;
};
template <class T>
class objectPool_CC
{
public:
	T *_malloc_boost()
	{
		Guard guard(&PoolMutex, true);
		T * ret = m_dzObjectPoolList.construct();
		return ret;
	};
	void _free_boost(T *dzPtr)
	{
		Guard guard(&PoolMutex, true);
		if (m_dzObjectPoolList.is_from(dzPtr))
		{
			m_dzObjectPoolList.destroy(dzPtr);
			return;
		}
		else
		{
			delete dzPtr;
			dzPtr = NULL;
		}
	};

private:
	boost::object_pool<T>m_dzObjectPoolList;
	CC::ThreadMutex PoolMutex;

};
}
#endif