
/*********************************
 * @time : 2011.3.2
 * @author :�´�
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
		*�������־��Ϣ
		*id��������Ϣ������ID
		*_EMessage��������Ϣ
		*=================================*/
		virtual int32_t  OnDBErrorMessage(const char* _EMessage) = 0;

	};
	struct DBInitItem
	{
		char nSqlIP[32];//IP��ַ
		unsigned short nSqlPort;//���Ӷ˿�
		char nSqlName[32];//���ݿ�����
		char nSqlUser[32];//��½��
		char nSqlPassword[32];//����
		uint32_t nSqlKey;//���ݿ�KEy
		uint32_t nSqlConCount;//���ӳظ���
		IMysqlInterFace* pMysqlInterFace;		//����ص��ӿ�
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
		*��ʼ�����ݿ�����
		** _Item:���ò�����
		*�ɹ�����true��ʧ��false
		*****************/
		virtual bool StartDBConnet(DBInitItem * _Item) = 0;
		/****************
		*ִ�в�ѯ���������������ݼ�
		*_Sql:���
		*�ɹ�����true��ʧ��false
		*_Key���ݿ�ؼ���
		*****************/
		virtual MYSQL_RES * QuerySql(std::string _Sql,int32_t _Key = 1) = 0;
		/****************
		*ִ��sql������ִ�еĽ��
		*_SqlVector:�����飬һ����sql��ִ���顣
		*�ɹ�����true��ʧ��false
		*_Key���ݿ�ؼ���
		*****************/
		virtual bool ExcelSql(std::vector<string> &_SqlVector,int32_t _Key = 1) = 0;
		/****************
		*ִ��sql������ִ�еĽ��
		*_SqlStr:ִ�е�����
		*�ɹ�����true��ʧ��false
		*_Key���ݿ�ؼ���
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