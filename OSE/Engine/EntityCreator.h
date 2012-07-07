#pragma once

#include <map>
#include <string>
#include <vector>

class BaseObject;

class BaseRenderableFactory
{
public:
	virtual BaseObject* Create() = 0;
};

template <class T>
class EntityFactory : public BaseRenderableFactory
{
public:
	T* Create() {return new T;}
};

#define ENTITYCREATOR EntityCreator::Instance()
class EntityCreator
{
private:
	std::map<std::string, BaseRenderableFactory*> FactoryMap;
	std::vector<BaseObject*> mAddQueue;
public:
	static void Cleanup();
	static EntityCreator* Instance();
	EntityCreator(void);
	~EntityCreator();
	void ProcessAddQueue();
	BaseObject* CreateEntity(const char* ID);
	void RegisterEntity(const char* &ID, BaseRenderableFactory* F);
};

