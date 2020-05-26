#ifndef _IANALYSISPACKET_H_
#define _IANALYSISPACKET_H_
#pragma once

class IAnalysisSystemMessgae
{
public:
	/*==============================
	*输出的日志消息
	*id：错误消息的网络ID
	*_EMessage：错误消息
	*=================================*/
	virtual int  OnProtocolErrorMessage(TcpSocketID id, char* _EMessage) = 0;
};
struct AnalysisItem
{
	IAnalysisSystemMessgae* pASMessage;		//服务回调接口
};
class IAnalysisPacket
{
public:
	/*==============================
	*解析群发消息协议（包头+群播类型+群播长度+数据类型+数据长度+数据）
	*id：网络ID
	*_data：网络数据
	*_len:长度
	*=================================*/
	virtual bool  DecryptMassData(TcpSocketID id, const unsigned char * _data, const uint32_t _len) = 0;
	/*==============================
	*解析消息协议（包头+数据类型+数据长度+数据）
	*id：网络ID
	*_data：网络数据
	*_len:长度
	*=================================*/
	virtual bool  DecryptData(TcpSocketID id, const unsigned char * _data, const uint32_t _len, TcpSocketID* _Token = 0) = 0;
	/*==============================
	*解析消息协议（包头+Token+数据类型+数据长度+数据）
	*id：网络ID
	*_data：网络数据
	*_len:长度
	*=================================*/
	virtual bool  DecryptTokenData(TcpSocketID id, const unsigned char * _data, const uint32_t _len) = 0;
	/*==============================
	 *解析消息协议
	 *id：网络ID
	 *_data：网络数据
	 *_len:长度
	 *=================================*/
	virtual bool  DecryptData(const unsigned char * _data,
		const uint32_t _len, char* _drcdata, uint32_t &_type, uint32_t &_drclen) = 0;

	/*==============================
	 *解析消息协议   消息类型+protobuf
	 *id：网络ID
	 *_data：网络数据
	 *_len:长度
	*=================================*/
	virtual bool  DecryptData(const unsigned char * _data, uint32_t _Srclen) = 0;

	/*==============================
	 *MQTT带主题解析消息协议   消息类型+protobuf
	 *id：网络ID
	 *_data：网络数据
	 *_len:长度
	*=================================*/
	virtual bool  DecryptTopicData(string _Topic, const unsigned char * _data, uint32_t _Srclen, uint32_t _ProtoVer = VERSION_1) = 0;

	/*==============================
   *设置网络消息对应的隐射函数
   *_Type：消息类型
   *_ProcessFunc：处理函数
   *=================================*/
	virtual bool  BindFunToMessage(uint32_t _Type, ProcessMQTTFunc _ProcessFunc) = 0;
	virtual bool  BindFunToMessage(uint32_t _Type, ProcessMqttTopicFunc _ProcessFunc) = 0;
	virtual bool  BindFunToMessage(string _NameType, ProcessITopicFunc _ProcessFunc) = 0;

	/*==============================
	*设置网络消息对应的隐射函数
	*_Type：消息类型
	*_ProcessFunc：处理函数
	*=================================*/
	virtual bool  BindFunToMessage(uint32_t _Type, ProcessFunc _ProcessFunc) = 0;
	/*==============================
   *设置网络消息对应的隐射函数带令牌
   *_Type：消息类型
   *_ProcessFunc：处理函数
   *=================================*/
	virtual bool  BindFunToMessage(uint32_t _Type, ProcessFuncToken _ProcessFunc) = 0;
	/*==============================
	*反序列化protobuf
	*_DrcData：protobuf协议的对象
	*_len：序列化的字符数组长度
	*_SrcData：反序列化的字符数组
	*=================================*/
	virtual uint8_t  ParsePartialData(void* _DrcData, const unsigned char * _SrcData, const uint32_t _len) = 0;
	/*==============================
	*序列化protobuf
	*_DrcData：序列化后的字符数组
	*_len：序列化后的字符数组长度
	*_SrcData：protobuf协议的对象
	*=================================*/
	virtual uint8_t  SerializeData(unsigned char* _DrcData, uint32_t &_len, void * _SrcData) = 0;

	/*==============================
	*初始化接口函数
	*接口数据_Item
	*=================================*/
	virtual int  InitAnalsysProtocol(AnalysisItem *_Item) = 0;

};
extern "C"
{
	IAnalysisPacket* CreateAnalsysNetDataImpl();
	void DeleteAnalsysNetDataImpl(IAnalysisPacket* pServer);
}
#endif