#pragma once
#ifndef _WECHATROOMOBJECT_H_
#define _WECHATROOMOBJECT_H_

class CWechatRoomObject
{
public:
	inline void setRoomID(uint32_t _RoomID) {
		m_RoomID = _RoomID;
	};
	inline uint32_t getRoomID() {
		return m_RoomID;
	};

	inline void setRecvUser(CWechatObject * _RecvUser) {
		m_RecvUser = _RecvUser;
	};
	inline CWechatObject * getRecvUser() {
		return m_RecvUser;
	};

	inline void setSendUser(CWechatObject * _SendUser) {
		m_SendUser = _SendUser;
	};
	inline CWechatObject * getSendUser() {
		return m_SendUser;
	};

private:
	//房间号
	uint32_t m_RoomID;
	//视频接受方
	CWechatObject *m_RecvUser;
	//视频发送方
	CWechatObject *m_SendUser;
};
#endif