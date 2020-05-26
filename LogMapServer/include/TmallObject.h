#pragma once
#ifndef _TMALLOBJECT_H_
#define _TMALLOBJECT_H_

class CTmallObject :public XCode
{
public:

	inline void setGwMac(string _GwMac) {
		m_GwMac = _GwMac;
	};
	inline string getGwMac() {
		return m_GwMac;
	};

	inline void setPosition(string _Position) {
		m_Position = _Position;
	};
	inline string getPosition() {
		return m_Position;
	};

	inline void setPosIdx(string _PosIdx) {
		m_PosIdx = _PosIdx;
	};
	inline string getPosIdx() {
		return m_PosIdx;
	};

	inline void setType(string _Type) {
		m_Type = _Type;
	};
	inline string getType() {
		return m_Type;
	};

	inline void setIndex(string _Index) {
		m_Index = _Index;
	};
	inline string getIndex() {
		return m_Index;
	};


	inline void setFeature(string _Feature) {
		m_Feature = _Feature;
	};
	inline string getFeature() {
		return m_Feature;
	};
private:
	string m_GwMac;
	string m_Position;
	string m_PosIdx;
	string m_Type;
	string m_Index;
	string m_Feature;

};

#endif