
/*********************************
 * @time : 2011.3.2
 * @author :陈春
 *******************************/
#ifndef _IMYSQLINTERFACE_H_
#define _IMYSQLINTERFACE_H_
#pragma once
namespace MySqlpool
{
	class IMysqlInterFace
	{
	public:
		virtual ~IMysqlInterFace() {};
		virtual bool OnConnected() = 0;
		/*==============================
		*输出的日志消息
		*id：错误消息的网络ID
		*_EMessage：错误消息
		*=================================*/
		virtual int32_t  OnDBErrorMessage(const char* _EMessage) = 0;

	};
	struct DBInitItem
	{
		char nSqlIP[32];//IP地址
		unsigned short nSqlPort;//连接端口
		char nSqlName[32];//数据库名字
		char nSqlUser[32];//登陆名
		char nSqlPassword[32];//密码
		uint32_t nSqlKey;//数据库KEy
		uint32_t nSqlConCount;//连接池个数
		IMysqlInterFace* pMysqlInterFace;		//服务回调接口
		DBInitItem()
		{
			memset(nSqlIP, 0, 32);
			memset(nSqlName, 0, 32);
			memset(nSqlUser, 0, 32);
			memset(nSqlPassword, 0, 32);
		}
	};

	class IMysql
	{
	public:
		virtual ~IMysql() {};
		/****************
		*初始化数据库引擎
		** _Item:配置参数。
		*成功返回true，失败false
		*****************/
		virtual bool StartDBConnet(DBInitItem * _Item) = 0;
		/****************
		*执行查询操作，并返回数据集
		*_Sql:命令。
		*成功返回true，失败false
		*_Key数据库关键字
		*****************/
		virtual MYSQL_RES * QuerySql(std::string _Sql,int32_t _Key = 1) = 0;
		/****************
		*执行sql并返回执行的结果
		*_SqlVector:命令组，一般是sql的执行组。
		*成功返回true，失败false
		*_Key数据库关键字
		*****************/
		virtual bool ExcelSql(std::vector<string> &_SqlVector,int32_t _Key = 1) = 0;
		/****************
		*执行sql并返回执行的结果
		*_SqlStr:执行的命令
		*成功返回true，失败false
		*_Key数据库关键字
		*****************/
		virtual MYSQL_RES * ExcelSql(std::string _SqlStr,int32_t _Key = 1) = 0;
	};

	extern "C"
	{
		 IMysql* CreateMysqlImpl();
		 void DeleteMysqlImpl(IMysql* pServer);
	}
}
#endif