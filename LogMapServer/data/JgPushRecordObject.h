#pragma once

#ifndef _JGPUSHRECORDOBJECT_H_
#define _JGPUSHRECORDOBJECT_H_
class CJgPushRecordObject:public XCode
{
public:
	inline void setContent(const char* _Content) {
		RtlZeroMemory(m_Content, 129);
		RtlCopyMemory(m_Content, _Content, __FUCK___STR___(_Content, 128));
	};
	inline char* getContent() {
		return m_Content;
	};

	inline void setContentTitle(const char* _ContentTitle) {
		RtlZeroMemory(m_ContentTitle, 129);
		RtlCopyMemory(m_ContentTitle, _ContentTitle, __FUCK___STR___(_ContentTitle, 128));
	};
	inline char* getContentTitle() {
		return m_ContentTitle;
	};

	inline void setTagAlias(const char* _TagAlias) {
		RtlZeroMemory(m_TagAlias, 129);
		RtlCopyMemory(m_TagAlias, _TagAlias, __FUCK___STR___(_TagAlias, 128));
	};
	inline char* getTagAlias() {
		return m_TagAlias;
	};
	
private:
	
	//��������
	char m_Content[129];
	//���ͱ���
	char m_ContentTitle[129];
	//���ͱ�ǩ��������ע��ID
	char m_TagAlias[129];
	

};

#endif