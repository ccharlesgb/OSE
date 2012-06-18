#pragma once

#include <map>
#include <string>

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
	BaseObject* Create() {return new T;}
};

#define ENTITYCREATOR EntityCreator::Instance()
class EntityCreator
{
private:
	std::map<std::string, BaseRenderableFactory*> FactoryMap;
public:
	static void Cleanup();
	static EntityCreator* Instance();
	EntityCreator(void);
	~EntityCreator();
	BaseObject* CreateEntity(const char* ID);
	void RegisterEntity(char* &ID, BaseRenderableFactory* F);
};

