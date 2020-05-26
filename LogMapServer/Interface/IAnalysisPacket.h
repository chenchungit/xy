#ifndef _IANALYSISPACKET_H_
#define _IANALYSISPACKET_H_
#pragma once

class IAnalysisSystemMessgae
{
public:
	/*==============================
	*�������־��Ϣ
	*id��������Ϣ������ID
	*_EMessage��������Ϣ
	*=================================*/
	virtual int  OnProtocolErrorMessage(TcpSocketID id, char* _EMessage) = 0;
};
struct AnalysisItem
{
	IAnalysisSystemMessgae* pASMessage;		//����ص��ӿ�
};
class IAnalysisPacket
{
public:
	/*==============================
	*����Ⱥ����ϢЭ�飨��ͷ+Ⱥ������+Ⱥ������+��������+���ݳ���+���ݣ�
	*id������ID
	*_data����������
	*_len:����
	*=================================*/
	virtual bool  DecryptMassData(TcpSocketID id, const unsigned char * _data, const uint32_t _len) = 0;
	/*==============================
	*������ϢЭ�飨��ͷ+��������+���ݳ���+���ݣ�
	*id������ID
	*_data����������
	*_len:����
	*=================================*/
	virtual bool  DecryptData(TcpSocketID id, const unsigned char * _data, const uint32_t _len, TcpSocketID* _Token = 0) = 0;
	/*==============================
	*������ϢЭ�飨��ͷ+Token+��������+���ݳ���+���ݣ�
	*id������ID
	*_data����������
	*_len:����
	*=================================*/
	virtual bool  DecryptTokenData(TcpSocketID id, const unsigned char * _data, const uint32_t _len) = 0;
	/*==============================
	 *������ϢЭ��
	 *id������ID
	 *_data����������
	 *_len:����
	 *=================================*/
	virtual bool  DecryptData(const unsigned char * _data,
		const uint32_t _len, char* _drcdata, uint32_t &_type, uint32_t &_drclen) = 0;

	/*==============================
	 *������ϢЭ��   ��Ϣ����+protobuf
	 *id������ID
	 *_data����������
	 *_len:����
	*=================================*/
	virtual bool  DecryptData(const unsigned char * _data, uint32_t _Srclen) = 0;

	/*==============================
	 *MQTT�����������ϢЭ��   ��Ϣ����+protobuf
	 *id������ID
	 *_data����������
	 *_len:����
	*=================================*/
	virtual bool  DecryptTopicData(string _Topic, const unsigned char * _data, uint32_t _Srclen, uint32_t _ProtoVer = VERSION_1) = 0;

	/*==============================
   *����������Ϣ��Ӧ�����亯��
   *_Type����Ϣ����
   *_ProcessFunc��������
   *=================================*/
	virtual bool  BindFunToMessage(uint32_t _Type, ProcessMQTTFunc _ProcessFunc) = 0;
	virtual bool  BindFunToMessage(uint32_t _Type, ProcessMqttTopicFunc _ProcessFunc) = 0;
	virtual bool  BindFunToMessage(string _NameType, ProcessITopicFunc _ProcessFunc) = 0;

	/*==============================
	*����������Ϣ��Ӧ�����亯��
	*_Type����Ϣ����
	*_ProcessFunc��������
	*=================================*/
	virtual bool  BindFunToMessage(uint32_t _Type, ProcessFunc _ProcessFunc) = 0;
	/*==============================
   *����������Ϣ��Ӧ�����亯��������
   *_Type����Ϣ����
   *_ProcessFunc��������
   *=================================*/
	virtual bool  BindFunToMessage(uint32_t _Type, ProcessFuncToken _ProcessFunc) = 0;
	/*==============================
	*�����л�protobuf
	*_DrcData��protobufЭ��Ķ���
	*_len�����л����ַ����鳤��
	*_SrcData�������л����ַ�����
	*=================================*/
	virtual uint8_t  ParsePartialData(void* _DrcData, const unsigned char * _SrcData, const uint32_t _len) = 0;
	/*==============================
	*���л�protobuf
	*_DrcData�����л�����ַ�����
	*_len�����л�����ַ����鳤��
	*_SrcData��protobufЭ��Ķ���
	*=================================*/
	virtual uint8_t  SerializeData(unsigned char* _DrcData, uint32_t &_len, void * _SrcData) = 0;

	/*==============================
	*��ʼ���ӿں���
	*�ӿ�����_Item
	*=================================*/
	virtual int  InitAnalsysProtocol(AnalysisItem *_Item) = 0;

};
extern "C"
{
	IAnalysisPacket* CreateAnalsysNetDataImpl();
	void DeleteAnalsysNetDataImpl(IAnalysisPacket* pServer);
}
#endif