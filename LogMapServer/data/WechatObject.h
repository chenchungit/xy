#pragma once
#ifndef _WECHATOBJECT_H_
#define _WECHATOBJECT_H_

class CWechatObject
{
public:
	inline void setCon(mg_connection *_Con) {
		m_Con = _Con;
	};
	inline mg_connection* getCon() {
		return m_Con;
	};

	inline void setRoomID(uint32_t _RoomID) {
		m_RoomID = _RoomID;
	};
	inline uint32_t getRoomID() {
		return m_RoomID;
	};

	inline void setPhone(const  char* _Phone) {
		RtlZeroMemory(m_Phone, 12);
		RtlCopyMemory(m_Phone, _Phone, __FUCK___STR___(_Phone, 11));
	};
	inline void setPhone(const  char* _Phone, int32_t _size) {
		RtlZeroMemory(m_Phone, 12);
		RtlCopyMemory(m_Phone, _Phone, _size);
	};
	inline  char* getDevState() {
		return m_Phone;
	};

private:
	//用户电话
	char m_Phone[12];
	//WebSocket connect
	mg_connection *m_Con;

	//房间号
	uint32_t m_RoomID;

};


#endif