#include "VariantMap.h"
#include <iostream>

VariantMap::VariantMap(void)
{

}

VariantMap::~VariantMap(void)
{
	Clear();
}

void VariantMap::Clear()
{
	std::map<std::string, VariantBase*>::iterator i;
	for (i = mDataMap.begin(); i != mDataMap.end(); ++i)
	{
		delete i->second;
	}
	mDataMap.clear();
}

void VariantMap::AddData(const char* ID, float dat)
{
	VariantType<float> *var = new VariantType<float>;
	var->SetData(dat);
	mDataMap[ID] = var;
}

void VariantMap::AddData(const char* ID, int dat)
{
	VariantType<int> *var = new VariantType<int>();
	var->SetData(dat);
	mDataMap[ID] = var;
}

void VariantMap::AddData(const char* ID, BaseObject* dat)
{
	VariantType<BaseObject*> *var = new VariantType<BaseObject*>();
	var->SetData(dat);
	mDataMap[ID] = var;
}

void VariantMap::AddData(const char* ID, std::string &dat)
{
	VariantType<std::string> *var = new VariantType<std::string>();
	var->SetData(dat);
	mDataMap[ID] = var;
}

bool VariantMap::ValidData(const char* ID)
{
	if (mDataMap[ID] == NULL)
	{
		std::cout << "Error invalid data requested: " << ID << "\n";
		return false;
	}
	return true;
}

float VariantMap::ReadFloat(const char* ID)
{
	if (!ValidData(ID))
		return 0.f;
	VariantType<float> var = *dynamic_cast<VariantType<float>*>(mDataMap[ID]);
	return var.GetData();
}

int VariantMap::ReadInt(const char* ID)
{
	if (!ValidData(ID))
		return 0;
	VariantType<int> var = *dynamic_cast<VariantType<int>*>(mDataMap[ID]);
	return var.GetData();
}

BaseObject* VariantMap::ReadEntity(const char* ID)
{
	if (!ValidData(ID))
		return NULL;
	VariantType<BaseObject*> var = *dynamic_cast<VariantType<BaseObject*>*>(mDataMap[ID]);
	return var.GetData();
}

std::string VariantMap::ReadString(const char* ID)
{
	if (!ValidData(ID))
		return NULL;
	VariantType<std::string> var = *dynamic_cast<VariantType<std::string>*>(mDataMap[ID]);
	return var.GetData();
}

