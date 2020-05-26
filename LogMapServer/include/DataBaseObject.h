#ifndef _DATABASEOBJECT_H_
#define _DATABASEOBJECT_H_
#pragma once
namespace __gnu_cxx {
	template<> struct hash<std::string>
	{
		size_t operator()(const std::string& x)const
		{
			return hash<const char*>()(x.c_str());
		}
	};
}
class CDataBaseObject:public XCode
{
public:
	CDataBaseObject() {
		//g_TimerService = NULL;
		m_IAnalysisPacket = NULL;
	};
	virtual ~CDataBaseObject() {};

	/*********************
	*��ʼ��
	************************/
	virtual void Initialize() = 0;

	/*********************
	*��ȡ���ݿ�
	************************/
	virtual bool ReadDB() = 0;

	/*********************
	*����
	************************/
	virtual bool JgPush(Json::Value &_Root) { return false; };


	void SendMQTTProtocolData(int32_t _Type, ::google::protobuf::Message& _Protocol, string _Topic, const char * _Context);
	/*********************
	*���ʹ�Token������
	************************/
	virtual void SendProtocolData(TcpSocketID id, ::google::protobuf::Message& _Protocol, int32_t _Cmd, const char * _Context);
	/*==============================
	*�������־��Ϣ
	*id��������Ϣ������ID
	*_EMessage��������Ϣ
	*=================================*/
	virtual int32_t  OnXCoreErrorMessage(char* _EMessage);

	/*==============================
	*���������ַ�
	*id��������Ϣ������ID
	*_EMessage��������Ϣ
	*=================================*/
	virtual char *  SpecialCharacters(char * _OutChar,char* _Characters, ...);
	
	template<class out_type, class in_value>
	out_type Convert(const in_value & t);

	void McuEnCrypt(UChar* pucBuff, uint32_t wLen, UChar* pucKey);

public:
	//���ݿ�ID
	uint32_t m_MaxID;
	//����
	uint32_t m_MaxNumber;

	//��ʱת�����ݵ��ڴ��
	objectPool_CC<PConvertBuffer> m_dzConvertBuffList;
	//�ں�
	//ITimerService *g_TimerService;
	//
	IAnalysisPacket* m_IAnalysisPacket;
	//
	CC::ThreadMutex m_DataMutex;
};
#endif