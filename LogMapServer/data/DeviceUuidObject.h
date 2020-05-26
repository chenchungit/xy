#pragma once
#ifndef _DEVICEUUIDOBJECT_H_
#define _DEVICEUUIDOBJECT_H_
class CDeviceUuidObject
{
public:
	inline void setMac(const char* _Mac) {
		RtlZeroMemory(m_DevMac, 33);
		RtlCopyMemory(m_DevMac, _Mac, __FUCK___STR___(_Mac, 32));
	};
	inline void setMac(const char* _Mac, int32_t _len) {
		RtlZeroMemory(m_DevMac, 33);
		RtlCopyMemory(m_DevMac, _Mac, _len);
	};
	inline void setMac(string _Mac) {
		RtlZeroMemory(m_DevMac, 33);
		RtlCopyMemory(m_DevMac, _Mac.c_str(), _Mac.length());
	};
	inline char* getMac() {
		return m_DevMac;
	};


	inline void setDevUuid(const  char* _DevUuid) {
		RtlZeroMemory(m_DevUuid, 16);
		RtlCopyMemory(m_DevUuid, _DevUuid, __FUCK___STR___(_DevUuid, 16));
	};
	inline void setDevUuid(const  char* _DevUuid, int32_t _size) {
		RtlZeroMemory(m_DevUuid, 16);
		RtlCopyMemory(m_DevUuid, _DevUuid, _size);
	};
	inline  char* getDevUuid() {
		return m_DevUuid;
	};
private:
	//MAC 
	char m_DevMac[33];
	//UUID
	char m_DevUuid[16];
};

#endif
