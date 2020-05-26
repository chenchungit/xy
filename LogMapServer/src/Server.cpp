#include "ServerHead.h"

char DeviceFeatureName[][128] = {
	"NULL",
	"ON_OFF",
	"BYTE",
	"WORD",
	"DWORD",
	"BLOCK",
	"MAC",
	"",
	"",
	"",
	"VD",
	"FEATURE",
	"DEVICE",
	"GATEWAY",
	"COMBIN",
	"GROUP",
	"DATE_TIME",
	"POWER_MEASURE",
	"MOTO",
	"LIGHTING",
	"SOCKET",
	"MULTISWITCH",
	"SWITCH",
	"LOCK",
	"ALARM",
	"DOOR_ALARM",
	"SMOKE_ALARM",
	"SMART_REMOTE",
	"KEYBOARD",
	"RELAY_ARRAY",
	"MJ",
	"HUMAN_CNT",
	"GAS_ALARM",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"GAS_SENSOR",
	"ENV_SENSOR",
	"TEMP_HUM_SENSOR",
	"","","","","","","","","","","","","","","","","","","","","","","","","","","","","",
	"FAN",
	"AC",
	"REFRIGERATOR",
	"WASH_MACHINE",
	"VACUUM_CCLEANER",
	"WATER_DISPENNSER",
	"AIR_CLEANER",
	"CURTAIN",
	"OSIM",
	"AC_PANEL",
	"","","","","","",
	"AC_KEY",
	"AC_CMD",
	"AC_STATUS",
	"VOICE_ID",
	"VOICE_CMD",
	"VOICE_SETUP",
	"VOICE_STATUS",
	"CT_STATUS",
	"OPEN",
	"CLOSE",
	"STOP",
	"","","","","",
	"TV",
	"HIFI",
	"RADIO",
	"DVD",
	"STB",
	"VCR",
	"GAME",
	"","","","","","","","","",
	"TV_KEY",
	"TV_CMD",
	"TV_STATUS",
	"RANGE_HOOD",
	"MICROWAVE_OVEN",
	"COFFEE_MACHINE",
	"TOASTER",
	"BLENDER",
	"DISIN_TANK",
	"ELECT_OVEN",
	"ELECT_STEAMER",
	"DISHWASHER",
	"","","","",
	"COLOR",
	"BRIGHT",
	"BIBO",
	"","","","","","","","","","","","","",
	"STATUS",
	"REC",
	"LOG",
	"MODE",
	"CONFIG",
	"MODIFY",
	"RESET",
	"CALIB",
	"SETUP",
	"USER",
	"NOTIFY",
	"CLEAR",
	"RUN",
	"CREATE",
	"DATA",
	"DELETE",
	"UPDATE_MCU",
	"UPDATE_BT",
	"IRIS_TEMPLATE",
	"FACE_TEMPLATE",
	"FINGERPRINT_TEMPLATE",
	"", "", "", "", "", "", "", "", "", "", "",
	"CURRENT",
	"VOLT",
	"RES",
	"CAP",
	"IND",
	"POWER",
	"ENERGY",
	"FREQ",
	"LEVEL",
	"PERCENT",
	"PPM",
    "TEMP",
	"HUMIDITY",
	"LUX",
	"SPEED",
	"FORCE",
	"POSITION",
	"RANGE",
	"DIR"
};

char TableName[][128] = {
	"xy_alarmproperty",           //0
	"xy_approlecontrast",         //1
	"xy_approlefun",              //2
	"xy_approlefunbaseinfo",      //3
	"xy_approletype",             //4
	"xy_areabaseinfo",            //5
	"xy_areadeviceinfo",          //6
	"xy_areauserecord",           //7
	"xy_buildpsw",                //8
	"xy_cardrecord",              //9
	"xy_cardsarea",               //10
	"xy_countrycode",             //11
	"xy_developer",               //12
	"xy_devfetnickname",          //13
	"xy_devicealarm",             //14
	"xy_devicebaseinfo",          //15
	"xy_devicedescribeinfo",      //16
	"xy_devicefet",               //17
	"xy_devicegroup",             //18
	"xy_devicepassword",          //19
	"xy_deviceproperty",          //20
	"xy_devtypecribeinfo",        //21
	"xy_elesendrecord",           //22
	"xy_foldertree",              //23
	"xy_folderuploadrecord",      //24
	"xy_gwdevicebindtemp",        //25
	"xy_gwdevicecontrast",        //26
	"xy_hmrecord",                //27
	"xy_item",                    //28
	"xy_jgpushrecord",            //29
	"xy_keysharerecord",          //30
	"xy_managersrolecontrast",    //31
	"xy_mechanism",               //32
	"xy_nocreditequip",           //33
	"xy_pswkeyrecord",            //34
	"xy_pswsendrecord",           //35
	"xy_roledevicerecord",        //36
	"xy_roleinfo",                //37
	"xy_roletype",                //38
	"xy_toolauthority",           //39
	"xy_typeapicontrast",         //40
	"xy_userecord",               //41
	"xy_userinfo",                //42
	"xy_version",                 //43
	"xy_versionrangle",           //44
	"xy_webinterface",            //45
};      //

char ErrorChinaContext[][128] = {
	"Log engine ==>",
	"Protocol engine ==>",
	"Network engine ==>",
	"Database engine ==>",
};

char ErrorEnglishContext[][128] = {
	"Log Engine==>",
	"Protocol Engine==>",
	"Network Engine==>",
	"Database Engine==>",
};
char SysTemContext[][128] = {
	"Successful database operation",
	"unknown error ",
	"Primary keys are not unique ",
	"unknown error ",
	"Key parameters are empty ",
	"Unable to perform data operations ",
};

CServerManager *g_ServerManager = NULL;
bool g_ThreadRun = false;
int32_t main()
{
	/*std::stringstream szCharacters;
	Json::Value szRoot, szHeader, szPayLoad;
	szCharacters.str("");
	szCharacters << "face.back";
	szHeader["name"] = szCharacters.str();
	szHeader["messageId"] = "8a699c72-866b-4d7f-8cad-8fd090ec0501";
	szHeader["namespace"] = "nexless.hj.face.discern";
	szHeader["payLoadVersion"] = "1.0.1";


	szCharacters.str("");
	szCharacters << "12345";
	szPayLoad["ActionID"] = szCharacters.str();

	szCharacters.str("");
	szCharacters << "1";
	szPayLoad["State"] = szCharacters.str();

	szRoot["header"] = szHeader;
	szRoot["payload"] = szPayLoad;




	Json::FastWriter fast_writer;
	string szJson = fast_writer.write(szRoot);

	printf("%s", szJson.c_str());*/

	g_ServerManager = new CServerManager();

	g_ServerManager->InitServer();
	while (true)
	{
		sleep(1);
		//printf("LDataServer Start..........");
	}
	return 0;
}