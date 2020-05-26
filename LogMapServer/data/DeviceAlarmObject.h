#ifndef _DEVICEALARMOBJECT_H_
#define _DEVICEALARMOBJECT_H_
#pragma once
class CDeviceAlarmObject:public XCode
{
public:
	
	inline void setCurTime(int64_t _CurTime) {
		m_CurTime = _CurTime;
	};
	inline int64_t getCurTime() {
		return m_CurTime;
	};

	inline void setHeppenTime(int64_t _HeppenTime) {
		m_HeppenTime = _HeppenTime;
	};
	inline int64_t getHeppenTime() {
		return m_HeppenTime;
	};

	inline void setDBState(int32_t _DBState) {
		m_DBState = _DBState;
	};
	inline int32_t getDBState() {
		return m_DBState;
	};

	inline void setState(int32_t _State) {
		m_State = _State;
	};
	inline int32_t getState() {
		return m_State;
	};

	inline void setMac(const char* _Mac) {
		RtlZeroMemory(m_Mac, 33);
		RtlCopyMemory(m_Mac, _Mac, __FUCK___STR___(_Mac, 32));
	};
	inline void setMac(const char* _Mac,int32_t _len) {
		RtlZeroMemory(m_Mac, 33);
		RtlCopyMemory(m_Mac, _Mac, _len);
	};
	inline void setMac(string _Mac) {
		RtlZeroMemory(m_Mac, 33);
		RtlCopyMemory(m_Mac, _Mac.c_str(), _Mac.length());
	};
	inline char* getMac() {
		return m_Mac;
	};

	inline void setDevSort(int32_t _DevSort) {
		m_DevSort = _DevSort;
	};
	inline int32_t getDevSort() {
		return m_DevSort;
	};

	inline void setDevState(const  char* _DevState) {
		RtlZeroMemory(m_DevState, 65);
		RtlCopyMemory(m_DevState, _DevState, __FUCK___STR___(_DevState, 64));
	};
	inline void setDevState(const  char* _DevState,int32_t _size) {
		RtlZeroMemory(m_DevState, 65);
		RtlCopyMemory(m_DevState, _DevState, _size);
	};
	inline  char* getDevState() {
		return m_DevState;
	};

	inline void setDevType(int32_t _DevType) {
		m_DevType = _DevType;
	};
	inline int32_t getDevType() {
		return m_DevType;
	};

private:
	
	//当前时间
	int64_t m_CurTime;
	//操作时间
	int64_t m_HeppenTime;
	//状态1启用，2金庸
	int32_t m_State;
	//数据状态
	int32_t m_DBState;
	//设备特性序号
	int32_t m_DevSort;
	//设备状态数据
	 char m_DevState[65];
	//数据类型
	int32_t m_DevType;
	//设备MAc
	char m_Mac[33];
};
#endif