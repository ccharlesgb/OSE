#include "EntityCreator.h"
#include <iostream>
#include "BaseObject.h"

EntityCreator* Inst = NULL;

EntityCreator* EntityCreator::Instance()
{
	if (Inst == NULL)
	{
		Inst = new EntityCreator;
	}
	return Inst;
}

EntityCreator::EntityCreator(void)
{
}

EntityCreator::~EntityCreator()
{
	std::map<std::string, BaseRenderableFactory*>::iterator i;
	for (i = FactoryMap.begin(); i != FactoryMap.end(); i++)
	{
		delete i->second;
	}
	FactoryMap.clear();
}

void EntityCreator::Cleanup()
{
	delete Inst;
}

BaseObject* EntityCreator::CreateEntity(const char* ID)
{
	BaseRenderableFactory* F = FactoryMap[ID];
	if (F != NULL)
	{
		BaseObject* ent = F->Create();
		ent->SetClassName(ID);
		return ent;
	}
	return NULL;
}

void EntityCreator::RegisterEntity(char* &ID, BaseRenderableFactory* F)
{
	std::cout << "Registering: " << ID << "\n";
	FactoryMap[ID] = F;
}
