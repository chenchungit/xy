#ifndef _SERVICEOBJECT_H_
#define _SERVICEOBJECT_H_
#pragma once

class CServiceObject
{
public:
	CServiceObject(int32_t _iType,int32_t _ServerID,TcpSocketID _TcpSocketID,int32_t _State/*,const char *_ip,int32_t _Port*/);
	~CServiceObject();

	 /****************
	*�������Ӳ���
	******************/
	void UpdateParameter(int32_t _iType,int32_t _ServerID,TcpSocketID _TcpSocketID,int32_t _State/*,const char *_ip,int32_t _Port*/);

public:
	//����������
	int32_t m_iType;
	//������ ID
	int32_t m_ServerID;
	//��ǰ״̬
	int32_t m_State;
	//����ID
	TcpSocketID m_TcpSocketID;
	//��ǰ����������
	int32_t m_CurrentUser;
	//�������
	int32_t m_MaxUser;
	//IP��ַ
	//char m_ip[32];
	//�˿�
	//int32_t m_port;
};


#endif