#pragma once

#include <vector>
#include <string>
#include <map>

class BaseObject;

class VariantBase
{
public:
	VariantBase() {};
	virtual ~VariantBase() {};
};

class VariantMap
{
private:
	std::map<std::string, VariantBase*> mDataMap;
	VariantMap(VariantMap &map);
public:
	VariantMap(void);
	~VariantMap(void);

	void Clear();

	void AddData(const char* ID, float dat);
	void AddData(const char* ID, int dat);
	void AddData(const char* ID, BaseObject* dat);
	void AddData(const char* ID, std::string &dat);

	bool ValidData(const char* ID);

	float ReadFloat(const char* ID);
	int ReadInt(const char* ID);
	BaseObject* ReadEntity(const char* ID);
	std::string ReadString(const char* ID);

	int GetVarCount() {return mDataMap.size();};
};

template<class T>
class VariantType : public VariantBase
{
private:
	T mData;
public:
	VariantType();
	VariantType(T dat);
	~VariantType();
	void SetData(T d) {mData = d;};
	T GetData() {return mData;};
};

template<class T>
VariantType<T>::VariantType()
{
}

template<class T>
VariantType<T>::VariantType(T dat)
{
	mData = dat;
}

template<class T>
VariantType<T>::~VariantType()
{
}