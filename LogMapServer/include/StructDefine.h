#pragma once
#ifndef _STRUCTDEFINE_H_
#define _STRUCTDEFINE_H_


//////////////////////////////////////////////////////////////////////////
typedef enum _DeviceCMD
{
	CMD_TIME_HEART = 0x00,//����
	CMD_DEVICE_ALARM =0x02,//���ⱨ��
	CMD_DEVICE_HUMANCNT = 0x03,//����������
	CMD_LOCK_LOG = 0x05,//����־
	CMD_XS = 0x0D,//Уʱ
	CMD_DEVICE_VERSION = 0x44,//Ӳ���汾�ȶ�
	CMD_DEVICE_VERSION_DATA = 0x45,//Ӳ���汾�ȶԷ���
	CMD_FILE_UP_DOWNLOAD = 0x46,//�ļ�����
	CMD_VERSION_DOWN_FILE = 0x47,//�·��ļ�����
	CMD_VERSION_NUMBER_UPDATE = 0x48,//�����豸�汾����
	CMD_GET_QUEUE_DEVICE = 0x20,//��ѯ�豸
}DeviceCMD;

////��ʱ����ת��
typedef struct ConvertBuffer
{
public:
	ConvertBuffer() {
		RtlZeroMemory(dzPSocketBuff, MAX_BUFF_NET_SIZE);
	};
	unsigned char dzPSocketBuff[MAX_BUFF_NET_SIZE];
}*LPConvertBuffer, PConvertBuffer;

/////
typedef enum _MqttQOS
{
	QOS_0 = 0,
	QOS_1 = 1,
	QOS_2 = 2,
}MqttQOS;
//�ص�������������
typedef enum  _ProtocolVer
{
	VERSION_1 = 1, //Э��汾1.0
	VERSION_2 = 2,//Э��汾2.0
}ProtocolVer;
//using namespace ServerProtocol;
//using namespace NetProtocol;
//using namespace MySqlpool;
//����������
typedef enum _ServerType
{
	YD_CONNECT_UNKNOW = 0,//δ֪
	YD_CONNECT_DB = 1,//���ݿ�����
	YD_CONNECT_ALLOWBATCH = 2,//Ⱥ��������
	YD_CONNECT_NETSERVER = 3,//���ط�����
	YD_CONNECT_LOG = 4,//��־������
	YD_CONNECT_LOGIC = 5,//�߼�������
	YD_CONNECT_COORD = 6,//Э��������
	YD_CONNECT_DATALOGIC = 7,//���ݷ�����
	YD_CONNECT_RPC = 8,//RPC������
}ServerType;
/////
//����������
typedef enum _DBType
{
	DB_STATE_UNKOWN = 0,//��
	DB_STATE_ADD = 1,//����
	DB_STATE_UPDATE = 2,//����
	DB_STATE_DELETE = 3,//ɾ��
}DBType;

//�˻�״̬
typedef enum _DataState
{
	DATA_STATE_DEL = 0,//ɾ��
	DATA_STATE_QIYONG = 1,//����
	DATA_STATE_TINGYONG = 2,//ͣ��
	DATA_STATE_APPLOGIN = 3,//app��½
	DATA_STATE_NETLOGIN = 4,//��̨
	DATA_STATE_BAOFEI = 5,//����
}DataState;
//�洢���̲�������
typedef enum _DBUseType
{
	DB_USE_UNKOWN = 0,
	DB_USE_QUEUE = 1,//��ѯ
	DB_USE_ADD = 2,//����
	DB_USE_UPDATE = 3,//����
	DB_USE_DELETE = 4,//ɾ��
}DBUseType;
//д����־����
typedef enum _LogType
{
	LOG_WRITE_NOMOL = 0,//�����
	LOG_WRITE_IO = 1,//�����IO
	LOG_WRITE_FILE = 2,//������ļ�
	LOG_WRITE_ERROR = 4,//������־
	LOG_WRITE_SYSTEM = 8,//ϵͳ��־
	LOG_WRITE_GENERAL = 16,//һ����־
	LOG_WRITE_BOTH = 32,//��Ҫд��־��Ҫ���
	
}LogType;
typedef enum _NetStart
{
	NET_START_ON = 0,//��ʼ����
	NET_START_OFF = 1,//���ӹر�
	NET_START_REON = 2,//����
}NetStart;
//������״̬
typedef enum _ServiceState
{
	YD_SERVICE_STATE_ON = 1,//��Ծ״̬
	YD_SERVICE_STATE_OFF = 2,//����״̬
	YD_SERVICE_STATE_UNKNOW = 3,//δ֪״̬
	YD_SERVICE_STATE_UNREGISTOR = 4,//δע��ֻ������
}ServiceState;
//����������
typedef enum _ReturnValue
{
	YD_RETURN_OK = 0,//�ɹ�
	YD_RETURN_EXP = 1,//�쳣
	YD_RETURN_FAIL = 2,//ʧ��
}ReturnValue;
//���ݷ��Ͷ���
typedef enum  _ServiceType
{
	SERVICE_SEND_APP = 1, //���͸�app
	SERVICE_SEND_SERVER = 2, //���͸�server
}ServiceType;


enum ProtocolType {
	PROTOCOL_TYPE_OK = 1,
	PROTOCOL_TYPE_EXCEPTION = 2,
	PROTOCOL_TYPE_ISFULL = 3,
	PROTOCOL_TYPE_UNKNOW = 4
};
//д����־����
typedef enum _DeviceLogType
{
	DEVICE_LOG_TYPE_0 = 0,//������־
	DEVICE_LOG_TYPE_1 = 1,//������־
	DEVICE_LOG_TYPE_2 = 2,//������־
	DEVICE_LOG_TYPE_3 = 3,//״̬��־
	DEVICE_LOG_TYPE_4 = 4,//һ����־
	DEVICE_LOG_TYPE_5 = 5,//��Ҫд��־��Ҫ���
}DeviceLogType;
//������������
typedef struct _ClientData
{
public:
	_ClientData() {
		memset(dzPSocketBuff, 0, MAX_BUFF_NET_SIZE);
		
		dzlen = 0;
		dzSocketID.key = 0L;
	};
	UChar dzPSocketBuff[MAX_BUFF_NET_SIZE];
	string dzTopic;
	int32_t dzlen;
	TcpSocketID dzSocketID;
}*LPClient, PClient;

typedef enum _DeviceFeatureType {

	/**
	 * BASE TYPE������
	 */
	IFT_ON_OFF=0x01, //"1λͨ������"),
	IFT_BYTE = 0x02, //"1�ֽ�ͨ������"),
	IFT_WORD = 0x03, //"2�ֽ�ͨ������"),
	IFT_DWORD = 0x04,// "4�ֽ�ͨ������"),
	IFT_BLOCK=0x05,// "���ݿ�"),
	// mac���ԣ��豸���ڵ����ԣ����ܶ����豸��mac��ַ
	IFT_MAC=0x06, //"MAC"),
	IFT_VD=0x0A, //"���Ȿ���豸"),
	IFT_FEATURE=0x0B, //"һ�����ԣ���ָ������"),
	IFT_DEVICE=0x0C, //"һ���豸����ָ������"),
	IFT_GATEWAY=0x0D,// "һ�����أ���ָ������"),
	IFT_COMBIN=0x0E, //"�����豸=����+�豸)����ָ������"),
	IFT_GROUP=0x0F, //"�����豸"),

	// FUNCTION TYPE=������)
	IFT_DATE_TIME=0x10,// "����ʱ��"),
	IFT_POWER_MEASURE=0x11,// "��������"),
	IFT_MOTO=0x12,// "���"),
	IFT_LIGHTING=0x13,// "�ƾ�"),
	IFT_SOCKET=0x14,// "����"),
	IFT_MULTISWITCH=0x15, //"��·����"),
	IFT_SWITCH=0x16,// "����"),
	IFT_LOCK=0x17,// "����"),
	IFT_ALARM=0x18,// "���ͺ���/΢��������"),
	//IFT_DOOR_BELL=0x19,// "����"),
	IFT_DOOR_ALARM = 0x19,// "�Ŵ�"),
	IFT_SMOKE_ALARM = 0x1A,// "��������"),
	//IFT_SPEAKER=0x1A, //"���ⱨ����"),
	IFT_SMART_REMOTE=0x1B,// "����ң����"),
	IFT_KEYBOARD=0x1C, //"����=�龰��塢�Ŵš�����ң������)"),
	IFT_RELAY_ARRAY=0x1D, //"�̵������к�"),
	IFT_MJ_HUB=0x1E,// "�Ž�������"),
	IFT_HUMAN_CNT=0x1F,// "����������"),
	// SENSOR TYPE=��������)
	IFT_GAS_SENSOR=0x30,// "���崫����"),
	IFT_ENV_SENSOR=0x31,// "����������"),
	// HOME_APPLIANCES_FUNC=���ܼҵ���)
	IFT_FAN=0x50,// "����"),
	IFT_AC=0x51,// "�յ�"),
	IFT_REFRIGERATOR=0x52,// "����"),
	IFT_WASH_MACHINE=0x53,// "ϴ�»�"),
	IFT_VACUUM_CCLEANER=0x54,// "������"),
	IFT_WATER_DISPENNSER=0x55,// "��ˮ��"),
	IFT_AIR_CLEANER=0x56,// "�·��"),
	IFT_CURTAIN=0x57,// "����"),
	IFT_OSIM=0x58,// "��Ħ��"),
	IFT_AC_PANEL=0x59,// "����յ��������"),
	//�յ�ϸ��
	IFT_AC_KEY=0x60,// "�յ�ң�ذ���"),
	IFT_AC_CMD=0x61,// "�յ�����"),
	IFT_AC_STATUS=0x62,// "�յ�״̬"),
	//����ʶ��
	IFT_VOICE_ID=0x63,// "����ʶ��װ��"),
	IFT_VOICE_CMD=0x64,// "��������"),
	IFT_VOICE_SETUP=0x65,// "�����������ã��������رգ�"),
	IFT_VOICE_STATUS=0x66,// "��������״̬���������رգ�"),
	//����ϸ��
	IFT_CT_STATUS=0x67,// "����״̬"),
	IFT_OPEN=0x68,// "��"),
	IFT_CLOSE=0x69,// "�ر�"),
	IFT_STOP=0x6A,// "ֹͣ"),
	// HOME_APPLIANCES_AMUSEMENT=���ֵ�����)
	IFT_TV=0x70,// "����"),
	IFT_HIFI=0x71,// "����"),
	IFT_RADIO=0x72,// "������"),
	IFT_DVD=0x73,// "Ӱ����"),
	IFT_STB=0x74, //"������"),
	IFT_VCR=0x75,// "¼Ӱ��"),
	IFT_GAME=0x76, //"��Ϸ��"),
	//����ϸ��
	IFT_TV_KEY=0x80,// "����ң�ذ���"),
	IFT_TV_CMD=0x81, //"��������"),
	IFT_TV_STATUS=0x82,// "����״̬"),
	// HOME_APPLIANCES_KITCHEN=����������)
	IFT_RANGE_HOOD=0x90,// "���̻�"),
	IFT_MICROWAVE_OVEN=0x91,// "΢��¯"),
	IFT_COFFEE_MACHINE=0x92,// "���Ȼ�"),
	IFT_TOASTER=0x93, //"�����"),
	IFT_BLENDER=0x94, //"������"),
	IFT_DISIN_TANK=0x95, //"�������"),
	IFT_ELECT_OVEN=0x96,// "����"),
	IFT_ELECT_STEAMER=0x97,// "����"),
	IFT_DISHWASHER=0x98,// "ϴ���"),
	// ͨ�õƹ��������
	IFT_COLOR=0xB0,// "��ɫ"),
	IFT_BRIGHT=0xB1,// "����"),
	IFT_BIBO=0xB2,// "����"),
	// ͨ����������
	IFT_STATUS=0xC0,// "״̬"),
	IFT_REC=0xC1,// "��¼"),
	IFT_LOG=0xC2,// "��־"),
	IFT_MODE=0xC3,// "ģʽ"),
	IFT_CONFIG=0xC4,// "����"),
	IFT_MODIFY=0xC5,// "����/�޸�"),
	IFT_RESET=0xC6,// "��λ/����"),
	IFT_CALIB=0xC7,// "У׼"),
	IFT_SETUP=0xC8,// "����"),
	IFT_USER=0xC9,// "�û���Ϣ"),
	IFT_NOTIFY=0xCA, //"֪ͨ"),

	//ͨ�����ݿ�����
	IFT_UPDATE_MCU=0xD0, //"MCU�̼�����"),
	//    IFT_UPDATE_BT=0xD1, "�����̼�����"),
	IFT_IRIS_TEMPLATE=0xD2,// "��Ĥģ��"),
	IFT_FACE_TEMPLATE=0xD3,// "����ģ��"),
	IFT_FINGERPRINT_TEMPLATE=0xD4,// "ָ��ģ��"),
	//ͨ��������
	IFT_CURRENT=0xE0,// "����"),
	IFT_VOLT=0xE1,// "��ѹ"),
	IFT_RES=0xE2, //"����"),
	IFT_CAP=0xE3,// "����"),
	IFT_IND=0xE4,// "���"),
	IFT_POWER=0xE5,// "����"),
	IFT_ENERGY=0xE6, //"����"),
	IFT_FREQ=0xE7,// "Ƶ��"),
	IFT_LEVEL=0xE8,// "ˮƽ"),
	IFT_PERCENT=0xE9,// "�ٷֱ�"),
	IFT_PPM=0xEA,// "����ֱ�"),
	IFT_TEMP=0xEB,// "�¶�"),
	IFT_HUMIDITY=0xEC,// "ʪ��"),
	IFT_LUX=0xED, //"����"),
	IFT_SPEED=0xEE,// "�ٶ�"),
	IFT_FORCE=0xEF, //"��/����"),
	IFT_POSITION=0xF0,// "λ��/��λ"),
	IFT_RANGE=0xF1,// "����"),
	IFT_NULL=0,// "��"),
}DeviceFeatureType;

typedef enum _DeviceType
{
	DEFNULL = 0x00,//δ֪�豸
	LOCAL_DEV = 0x01,//�����豸
	PANEL = 0x02,//���
	MODULE = 0x03,//ģ��
	SOCKET = 0x10,//����
	CURTAIN = 0x11,//����
	SWITCH1 = 0x12,//������
	SWITCH2 = 0x13,//����2
	SWITCH3 = 0x14,//����3
	REMOTE_SWITCH1 = 0x15,//���ߵ�����
	REMOTE_SWITCH2 = 0x16,//����2����
	REMOTE_SWITCH3 = 0x17,//����3����
	MJ_LOCK = 0x20,//�Ž�
	CORE_LOCK = 0x21,//��о��
	DOOR_LOCK = 0x22,//����
	HAND_LOCK = 0x23,//������
	PARKING_LOCK = 0x24,//��λ�� 
	CABINET_LOCK = 0x25,//������
	NB = 0x26,//NB�豸
	SREONG_BOX = 0x26,//���չ�
	SENSOR = 0x30,//������
	ALARM = 0x31,//������ 
	IR_COUNT = 0x32,//�˿ڳ���ͳ����
	IRIS = 0x33,// ��Ĥ
	FACE = 0x34,//����ʶ���
	U_LOCK = 0x35,//U����
	LUGGAGE_LOCK = 0x36,//�����
	SOUND = 0x37,//����
	VCU = 0x38,//�������Ƶ�Ԫ
	REMT = 0x40,//ң����
	CENT_AC = 0x41,//����յ�������
	LAMP = 0x50,//��
	COLOR_LAMP = 0x51,//��ɫ��
	LDP = 0x52,// LED�����Դ
	GATEWAY = 0x60,//������
}DeviceType;


//2�ֽڵİ汾��
#define MAKE_VER_556(major, minor, ver) (((uint16_t)((major& 0x1F)<< 11))|\
 ((uint16_t)((minor& 0x1F)<< 6))| (ver& 0x3F))
//#define HW_VERSION   MAKE_VER_556(HW_MAJOR,HW_MINOR,HW_REV)
//#define FW_VERSION   MAKE_VER_556(FW_MAJOR,FW_MINOR,FW_REV)

#endif