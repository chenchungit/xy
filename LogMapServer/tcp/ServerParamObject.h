#pragma once
#ifndef _SERVERPARAMOBJECT_H_
#define _SERVERPARAMOBJECT_H_

class CServerParamObject
{
public:
	/*************************Parameter
	*��ȡ������ID
	*************************/
	inline int32_t getServerID() { return m_ServerID; };
	/*************************Parameter
	*��ȡ����������
	*************************/
	inline int32_t getServerType() { return m_ServerType; };

	/*************************Parameter
	*��ȡ�������ݼ���
	*************************/
	inline int32_t getCrypt() { return m_Crypt; };
public:
	//��ǰ������ID
	int32_t m_ServerType;
	//��ǰ������ID
	int32_t m_ServerID;
	//����ͨ���Ƿ����
	int32_t m_Crypt;
};

#endif