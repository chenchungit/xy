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
	*初始化
	************************/
	virtual void Initialize() = 0;

	/*********************
	*读取数据库
	************************/
	virtual bool ReadDB() = 0;

	/*********************
	*推送
	************************/
	virtual bool JgPush(Json::Value &_Root) { return false; };


	void SendMQTTProtocolData(int32_t _Type, ::google::protobuf::Message& _Protocol, string _Topic, const char * _Context);
	/*********************
	*发送带Token的数据
	************************/
	virtual void SendProtocolData(TcpSocketID id, ::google::protobuf::Message& _Protocol, int32_t _Cmd, const char * _Context);
	/*==============================
	*输出的日志消息
	*id：错误消息的网络ID
	*_EMessage：错误消息
	*=================================*/
	virtual int32_t  OnXCoreErrorMessage(char* _EMessage);

	/*==============================
	*检索特殊字符
	*id：错误消息的网络ID
	*_EMessage：错误消息
	*=================================*/
	virtual char *  SpecialCharacters(char * _OutChar,char* _Characters, ...);
	
	template<class out_type, class in_value>
	out_type Convert(const in_value & t);

	void McuEnCrypt(UChar* pucBuff, uint32_t wLen, UChar* pucKey);

public:
	//数据库ID
	uint32_t m_MaxID;
	//编码
	uint32_t m_MaxNumber;

	//临时转换数据的内存池
	objectPool_CC<PConvertBuffer> m_dzConvertBuffList;
	//内核
	//ITimerService *g_TimerService;
	//
	IAnalysisPacket* m_IAnalysisPacket;
	//
	CC::ThreadMutex m_DataMutex;
};
#endif