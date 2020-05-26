#ifndef _USERECORDOBJECT_H_
#define _USERECORDOBJECT_H_
#pragma once
class CUseRecordObject :public XCode
{
public:
	inline void setAreaDeviceID(uint64_t _AreaDeviceID) {
		m_AreaDeviceID = _AreaDeviceID;
	};
	inline uint64_t getAreaDeviceID() {
		return m_AreaDeviceID;
	};

	inline void setUser(const char* _User) {
		RtlZeroMemory(m_User,33);
		RtlCopyMemory(m_User,_User, __FUCK___STR___(_User,32));
	};
	inline void setUser(string _User) {
		RtlZeroMemory(m_User, 33);
		RtlCopyMemory(m_User, _User.c_str(), __FUCK_STR_LEN__(_User.length(), 32));
	};
	inline char* getUser() {
		return m_User;
	};
	

	inline void setMac(const char* _Mac) {
		RtlZeroMemory(m_Mac,33);
		RtlCopyMemory(m_Mac, _Mac, __FUCK___STR___(_Mac,32));
	};
	inline void setMac(string _Mac) {
		RtlZeroMemory(m_Mac, 33);
		RtlCopyMemory(m_Mac, _Mac.c_str(), __FUCK_STR_LEN__(_Mac.length(), 32));
	};
	inline char* getMac() {
		return m_Mac;
	};

	inline void setGwMac(const char* _GwMac) {
		RtlZeroMemory(m_GwMac, 33);
		RtlCopyMemory(m_GwMac, _GwMac, __FUCK___STR___(_GwMac,32));
	};
	inline void setGwMac(string _GwMac) {
		RtlZeroMemory(m_GwMac, 33);
		RtlCopyMemory(m_GwMac, _GwMac.c_str(), __FUCK_STR_LEN__(_GwMac.length(), 32));
	};
	inline char* getGwMac() {
		return m_GwMac;
	};

	inline void setUseType(int32_t _UseType) {
		m_UseType = _UseType;
	};
	inline int32_t getUseType() {
		return m_UseType;
	}
	;

	inline void setUseTime(int64_t _UseTime) {
		m_UseTime = _UseTime;
	};
	inline int64_t getUseTime() {
		return m_UseTime;
	};

	inline void setUpTime(int64_t _UpTime) {
		m_UpTime = _UpTime;
	};
	inline int64_t getUpTime() {
		return m_UpTime;
	};

	inline void setDBState(short _DBState) {
		m_DBState = _DBState;
	};
	inline short getDBState() {
		return m_DBState;
	};

	inline void setRemark(const char* _Remark) {
		RtlZeroMemory(m_Remark, 256);
		RtlCopyMemory(m_Remark, _Remark, __FUCK___STR___(_Remark,256));
	};
	inline void setRemark(string _Remark) {
		RtlZeroMemory(m_Remark, 256);
		RtlCopyMemory(m_Remark, _Remark.c_str(), __FUCK_STR_LEN__(_Remark.length(), 256));
	};
	inline char* getRemark() {
		return m_Remark;
	};

	inline void setLastTime(int64_t _LastTime) {
		m_LastTime = _LastTime;
	};
	inline int64_t getLastTime() {
		return m_LastTime;
	};

	inline void setLogType(int32_t _LogType) {
		m_LogType = _LogType;
	};
	inline int32_t getLogType() {
		return m_LogType;
	};

	inline void setCarType(int32_t _CarType) {
		m_CarType = _CarType;
	};
	inline int32_t getCarType() {
		return m_CarType;
	};

	
private:
	
	//使用者
	char m_User[33];
	//设备ID
	char m_Mac[33];
	//设备ID
	char m_GwMac[33];
	//行为类型
	int32_t m_UseType;
	//操作时间
	int64_t m_UseTime;
	//上传时间
	int64_t m_UpTime;
	//备注
	char m_Remark[256];
	//数据状态
	short m_DBState;
	//最后活跃时间
	int64_t m_LastTime;
	//设备ID
	int32_t m_LogType;
	//设备ID
	int32_t m_CarType;

	//区域设备ID
	uint64_t m_AreaDeviceID;
};
#endif
