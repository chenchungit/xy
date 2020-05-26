#pragma once
#ifndef _VERSIONOBJECT_H_
#define _VERSIONOBJECT_H_

class CVersionObject
{
public:
	CVersionObject() {};
	~CVersionObject() {};

	inline void setIndex(UINT _Index) {
		m_Index = _Index;
	};
	inline UINT getIndex() {
		return m_Index;
	};

	inline void setSumSize(UINT _SumSize) {
		m_SumSize = _SumSize;
	};
	inline UINT getSumSize() {
		return m_SumSize;
	};


	inline void setDescribeFlag(string _DescribeFlag) {
		RtlZeroMemory(m_DescribeFlag, MAX_PATH);
		RtlCopyMemory(m_DescribeFlag, _DescribeFlag.c_str(), _DescribeFlag.length());
	};
	inline char* getDescribeFlag() {
		return m_DescribeFlag;
	};

	inline vector<UChar>& getFileVersion() {
		return m_FileVersion;
	};

private:
	//�ļ�����
	vector<UChar>m_FileVersion;
	//�ļ�������
	char m_DescribeFlag[MAX_PATH];
	//���
	UINT m_Index;
	//�ļ��ܴ�С
	UINT m_SumSize;
};
#endif