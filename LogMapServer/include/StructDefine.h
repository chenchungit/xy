#pragma once
#ifndef _STRUCTDEFINE_H_
#define _STRUCTDEFINE_H_


//////////////////////////////////////////////////////////////////////////
typedef enum _DeviceCMD
{
	CMD_TIME_HEART = 0x00,//心跳
	CMD_DEVICE_ALARM =0x02,//红外报警
	CMD_DEVICE_HUMANCNT = 0x03,//人数计数器
	CMD_LOCK_LOG = 0x05,//锁日志
	CMD_XS = 0x0D,//校时
	CMD_DEVICE_VERSION = 0x44,//硬件版本比对
	CMD_DEVICE_VERSION_DATA = 0x45,//硬件版本比对返回
	CMD_FILE_UP_DOWNLOAD = 0x46,//文件下载
	CMD_VERSION_DOWN_FILE = 0x47,//下发文件数据
	CMD_VERSION_NUMBER_UPDATE = 0x48,//更新设备版本号吗
	CMD_GET_QUEUE_DEVICE = 0x20,//查询设备
}DeviceCMD;

////临时数据转换
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
//回调函数返回类型
typedef enum  _ProtocolVer
{
	VERSION_1 = 1, //协议版本1.0
	VERSION_2 = 2,//协议版本2.0
}ProtocolVer;
//using namespace ServerProtocol;
//using namespace NetProtocol;
//using namespace MySqlpool;
//服务器类型
typedef enum _ServerType
{
	YD_CONNECT_UNKNOW = 0,//未知
	YD_CONNECT_DB = 1,//数据库连接
	YD_CONNECT_ALLOWBATCH = 2,//群发服务器
	YD_CONNECT_NETSERVER = 3,//网关服务器
	YD_CONNECT_LOG = 4,//日志服务器
	YD_CONNECT_LOGIC = 5,//逻辑服务器
	YD_CONNECT_COORD = 6,//协调服务器
	YD_CONNECT_DATALOGIC = 7,//数据服务器
	YD_CONNECT_RPC = 8,//RPC服务器
}ServerType;
/////
//服务器类型
typedef enum _DBType
{
	DB_STATE_UNKOWN = 0,//无
	DB_STATE_ADD = 1,//增加
	DB_STATE_UPDATE = 2,//更新
	DB_STATE_DELETE = 3,//删除
}DBType;

//账户状态
typedef enum _DataState
{
	DATA_STATE_DEL = 0,//删除
	DATA_STATE_QIYONG = 1,//正常
	DATA_STATE_TINGYONG = 2,//停用
	DATA_STATE_APPLOGIN = 3,//app登陆
	DATA_STATE_NETLOGIN = 4,//后台
	DATA_STATE_BAOFEI = 5,//报废
}DataState;
//存储过程操作类型
typedef enum _DBUseType
{
	DB_USE_UNKOWN = 0,
	DB_USE_QUEUE = 1,//查询
	DB_USE_ADD = 2,//新增
	DB_USE_UPDATE = 3,//更新
	DB_USE_DELETE = 4,//删除
}DBUseType;
//写入日志类型
typedef enum _LogType
{
	LOG_WRITE_NOMOL = 0,//不输出
	LOG_WRITE_IO = 1,//输出到IO
	LOG_WRITE_FILE = 2,//输出到文件
	LOG_WRITE_ERROR = 4,//错误日志
	LOG_WRITE_SYSTEM = 8,//系统日志
	LOG_WRITE_GENERAL = 16,//一般日志
	LOG_WRITE_BOTH = 32,//既要写日志又要输出
	
}LogType;
typedef enum _NetStart
{
	NET_START_ON = 0,//初始连接
	NET_START_OFF = 1,//连接关闭
	NET_START_REON = 2,//重连
}NetStart;
//服务器状态
typedef enum _ServiceState
{
	YD_SERVICE_STATE_ON = 1,//活跃状态
	YD_SERVICE_STATE_OFF = 2,//待机状态
	YD_SERVICE_STATE_UNKNOW = 3,//未知状态
	YD_SERVICE_STATE_UNREGISTOR = 4,//未注册只是连接
}ServiceState;
//服务器类型
typedef enum _ReturnValue
{
	YD_RETURN_OK = 0,//成功
	YD_RETURN_EXP = 1,//异常
	YD_RETURN_FAIL = 2,//失败
}ReturnValue;
//数据发送对象
typedef enum  _ServiceType
{
	SERVICE_SEND_APP = 1, //发送给app
	SERVICE_SEND_SERVER = 2, //发送给server
}ServiceType;


enum ProtocolType {
	PROTOCOL_TYPE_OK = 1,
	PROTOCOL_TYPE_EXCEPTION = 2,
	PROTOCOL_TYPE_ISFULL = 3,
	PROTOCOL_TYPE_UNKNOW = 4
};
//写入日志类型
typedef enum _DeviceLogType
{
	DEVICE_LOG_TYPE_0 = 0,//开门日志
	DEVICE_LOG_TYPE_1 = 1,//错误日志
	DEVICE_LOG_TYPE_2 = 2,//报警日志
	DEVICE_LOG_TYPE_3 = 3,//状态日志
	DEVICE_LOG_TYPE_4 = 4,//一般日志
	DEVICE_LOG_TYPE_5 = 5,//既要写日志又要输出
}DeviceLogType;
//网络来的数据
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
	 * BASE TYPE基本类
	 */
	IFT_ON_OFF=0x01, //"1位通用类型"),
	IFT_BYTE = 0x02, //"1字节通用类型"),
	IFT_WORD = 0x03, //"2字节通用类型"),
	IFT_DWORD = 0x04,// "4字节通用类型"),
	IFT_BLOCK=0x05,// "数据块"),
	// mac特性，设备根节点特性，功能读出设备的mac地址
	IFT_MAC=0x06, //"MAC"),
	IFT_VD=0x0A, //"虚拟本地设备"),
	IFT_FEATURE=0x0B, //"一般特性，不指定功能"),
	IFT_DEVICE=0x0C, //"一般设备，不指定功能"),
	IFT_GATEWAY=0x0D,// "一般网关，不指定功能"),
	IFT_COMBIN=0x0E, //"复合设备=网关+设备)，不指定功能"),
	IFT_GROUP=0x0F, //"分组设备"),

	// FUNCTION TYPE=功能类)
	IFT_DATE_TIME=0x10,// "日期时间"),
	IFT_POWER_MEASURE=0x11,// "电力计量"),
	IFT_MOTO=0x12,// "电机"),
	IFT_LIGHTING=0x13,// "灯具"),
	IFT_SOCKET=0x14,// "插座"),
	IFT_MULTISWITCH=0x15, //"多路开关"),
	IFT_SWITCH=0x16,// "开关"),
	IFT_LOCK=0x17,// "锁具"),
	IFT_ALARM=0x18,// "热释红外/微波报警器"),
	//IFT_DOOR_BELL=0x19,// "门铃"),
	IFT_DOOR_ALARM = 0x19,// "门磁"),
	IFT_SMOKE_ALARM = 0x1A,// "烟雾报警器"),
	//IFT_SPEAKER=0x1A, //"声光报警器"),
	IFT_SMART_REMOTE=0x1B,// "智能遥控器"),
	IFT_KEYBOARD=0x1C, //"键盘=情景面板、门磁、按键遥控器等)"),
	IFT_RELAY_ARRAY=0x1D, //"继电器阵列盒"),
	IFT_MJ_HUB=0x1E,// "门禁管理器"),
	IFT_HUMAN_CNT=0x1F,// "人数计数器"),
	// SENSOR TYPE=传感器类)
	IFT_GAS_SENSOR=0x30,// "气体传感器"),
	IFT_ENV_SENSOR=0x31,// "环境传感器"),
	// HOME_APPLIANCES_FUNC=功能家电类)
	IFT_FAN=0x50,// "风扇"),
	IFT_AC=0x51,// "空调"),
	IFT_REFRIGERATOR=0x52,// "冰箱"),
	IFT_WASH_MACHINE=0x53,// "洗衣机"),
	IFT_VACUUM_CCLEANER=0x54,// "吸尘器"),
	IFT_WATER_DISPENNSER=0x55,// "净水器"),
	IFT_AIR_CLEANER=0x56,// "新风机"),
	IFT_CURTAIN=0x57,// "窗帘"),
	IFT_OSIM=0x58,// "按摩器"),
	IFT_AC_PANEL=0x59,// "中央空调控制面板"),
	//空调细项
	IFT_AC_KEY=0x60,// "空调遥控按键"),
	IFT_AC_CMD=0x61,// "空调命令"),
	IFT_AC_STATUS=0x62,// "空调状态"),
	//语音识别
	IFT_VOICE_ID=0x63,// "语音识别装置"),
	IFT_VOICE_CMD=0x64,// "语音命令"),
	IFT_VOICE_SETUP=0x65,// "语音功能设置（开启、关闭）"),
	IFT_VOICE_STATUS=0x66,// "语音功能状态（开启、关闭）"),
	//窗帘细项
	IFT_CT_STATUS=0x67,// "窗帘状态"),
	IFT_OPEN=0x68,// "打开"),
	IFT_CLOSE=0x69,// "关闭"),
	IFT_STOP=0x6A,// "停止"),
	// HOME_APPLIANCES_AMUSEMENT=娱乐电器类)
	IFT_TV=0x70,// "电视"),
	IFT_HIFI=0x71,// "音响"),
	IFT_RADIO=0x72,// "收音机"),
	IFT_DVD=0x73,// "影碟机"),
	IFT_STB=0x74, //"机顶盒"),
	IFT_VCR=0x75,// "录影机"),
	IFT_GAME=0x76, //"游戏机"),
	//电视细项
	IFT_TV_KEY=0x80,// "电视遥控按键"),
	IFT_TV_CMD=0x81, //"电视命令"),
	IFT_TV_STATUS=0x82,// "电视状态"),
	// HOME_APPLIANCES_KITCHEN=厨房电器类)
	IFT_RANGE_HOOD=0x90,// "油烟机"),
	IFT_MICROWAVE_OVEN=0x91,// "微波炉"),
	IFT_COFFEE_MACHINE=0x92,// "咖啡机"),
	IFT_TOASTER=0x93, //"面包机"),
	IFT_BLENDER=0x94, //"搅拌器"),
	IFT_DISIN_TANK=0x95, //"消毒碗柜"),
	IFT_ELECT_OVEN=0x96,// "烤箱"),
	IFT_ELECT_STEAMER=0x97,// "蒸箱"),
	IFT_DISHWASHER=0x98,// "洗碗机"),
	// 通用灯光控制特性
	IFT_COLOR=0xB0,// "颜色"),
	IFT_BRIGHT=0xB1,// "亮度"),
	IFT_BIBO=0xB2,// "渐变"),
	// 通用属性特性
	IFT_STATUS=0xC0,// "状态"),
	IFT_REC=0xC1,// "记录"),
	IFT_LOG=0xC2,// "日志"),
	IFT_MODE=0xC3,// "模式"),
	IFT_CONFIG=0xC4,// "配置"),
	IFT_MODIFY=0xC5,// "修正/修改"),
	IFT_RESET=0xC6,// "复位/清零"),
	IFT_CALIB=0xC7,// "校准"),
	IFT_SETUP=0xC8,// "设置"),
	IFT_USER=0xC9,// "用户信息"),
	IFT_NOTIFY=0xCA, //"通知"),

	//通用数据块特性
	IFT_UPDATE_MCU=0xD0, //"MCU固件升级"),
	//    IFT_UPDATE_BT=0xD1, "蓝牙固件升级"),
	IFT_IRIS_TEMPLATE=0xD2,// "虹膜模板"),
	IFT_FACE_TEMPLATE=0xD3,// "人脸模板"),
	IFT_FINGERPRINT_TEMPLATE=0xD4,// "指纹模板"),
	//通用物理量
	IFT_CURRENT=0xE0,// "电流"),
	IFT_VOLT=0xE1,// "电压"),
	IFT_RES=0xE2, //"电阻"),
	IFT_CAP=0xE3,// "电容"),
	IFT_IND=0xE4,// "电感"),
	IFT_POWER=0xE5,// "功率"),
	IFT_ENERGY=0xE6, //"电能"),
	IFT_FREQ=0xE7,// "频率"),
	IFT_LEVEL=0xE8,// "水平"),
	IFT_PERCENT=0xE9,// "百分比"),
	IFT_PPM=0xEA,// "百万分比"),
	IFT_TEMP=0xEB,// "温度"),
	IFT_HUMIDITY=0xEC,// "湿度"),
	IFT_LUX=0xED, //"亮度"),
	IFT_SPEED=0xEE,// "速度"),
	IFT_FORCE=0xEF, //"力/力矩"),
	IFT_POSITION=0xF0,// "位置/档位"),
	IFT_RANGE=0xF1,// "距离"),
	IFT_NULL=0,// "无"),
}DeviceFeatureType;

typedef enum _DeviceType
{
	DEFNULL = 0x00,//未知设备
	LOCAL_DEV = 0x01,//本地设备
	PANEL = 0x02,//面板
	MODULE = 0x03,//模块
	SOCKET = 0x10,//插座
	CURTAIN = 0x11,//窗帘
	SWITCH1 = 0x12,//单开关
	SWITCH2 = 0x13,//开关2
	SWITCH3 = 0x14,//开关3
	REMOTE_SWITCH1 = 0x15,//无线单开关
	REMOTE_SWITCH2 = 0x16,//无线2开关
	REMOTE_SWITCH3 = 0x17,//无线3开关
	MJ_LOCK = 0x20,//门禁
	CORE_LOCK = 0x21,//锁芯锁
	DOOR_LOCK = 0x22,//门锁
	HAND_LOCK = 0x23,//拉手锁
	PARKING_LOCK = 0x24,//车位锁 
	CABINET_LOCK = 0x25,//机柜锁
	NB = 0x26,//NB设备
	SREONG_BOX = 0x26,//保险柜
	SENSOR = 0x30,//传感器
	ALARM = 0x31,//报警器 
	IR_COUNT = 0x32,//人口出入统计器
	IRIS = 0x33,// 虹膜
	FACE = 0x34,//人脸识别机
	U_LOCK = 0x35,//U型锁
	LUGGAGE_LOCK = 0x36,//箱包锁
	SOUND = 0x37,//音响
	VCU = 0x38,//声音控制单元
	REMT = 0x40,//遥控器
	CENT_AC = 0x41,//中央空调控制器
	LAMP = 0x50,//灯
	COLOR_LAMP = 0x51,//彩色灯
	LDP = 0x52,// LED调光电源
	GATEWAY = 0x60,//集中器
}DeviceType;


//2字节的版本号
#define MAKE_VER_556(major, minor, ver) (((uint16_t)((major& 0x1F)<< 11))|\
 ((uint16_t)((minor& 0x1F)<< 6))| (ver& 0x3F))
//#define HW_VERSION   MAKE_VER_556(HW_MAJOR,HW_MINOR,HW_REV)
//#define FW_VERSION   MAKE_VER_556(FW_MAJOR,FW_MINOR,FW_REV)

#endif